#include "BrainStdAfx.h"
#include "Video_Server.h"
#include "Camera.h"
#include "IO_Board_RPi.h"
#include "IO_Board_Sim.h"
#include "Comms.h"
#include "UAV.h"

#include <sys/time.h>
#include <sys/resource.h>

#if defined RASPBERRY_PI
extern "C"
{
    #include "bcm_host.h"
}
#endif

size_t s_test = 0;

namespace boost
{
	void throw_exception(std::exception const & e)
	{
		SILK_ERR("boost::exception {}", e.what());
		throw e;
    }
}

static void setup_camera_defaults(silk::Camera& camera, silk::Video_Server& streamer)
{
    camera.setup_high_quality(math::vec2u32(1280, 960), 16000000);
    camera.setup_medium_quality(math::vec2u32(640, 480), 2000000);
    camera.setup_low_quality(math::vec2u32(320, 240), 160000);

    camera.set_stream_callback([&](uint8_t const* data, size_t size)
    {
        streamer.send_frame(silk::Video_Server::Flags(), data, size);
    });
}


int main(int argc, char const* argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));

#if defined RASPBERRY_PI
    SILK_INFO("initializing bcm_host");
    bcm_host_init();
#endif

    namespace po = boost::program_options;

	po::options_description desc("Options");
	desc.add_options()
		("help", "produce help message")
        ("mtu", po::value<size_t>(), "packet size")
        ("test", po::value<size_t>(), "test");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) 
	{
		std::cout << desc << "\n";
		return 1;
	}

    s_test = vm.count("test") ? vm["test"].as<size_t>() : size_t(0);

	boost::asio::io_service io_service;

	SILK_INFO("Creating io_service thread");

    auto io_thread = boost::thread([&io_service]()
	{
		while (true)
		{
            io_service.run();
            io_service.reset();
            boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
        }
 	});

//    {
//        int policy = SCHED_FIFO;
//        struct sched_param param;
//        param.sched_priority = 2;

//        if (pthread_setschedparam(pthread_self(), policy, &param) != 0)
//        {
//            perror("main sched_setscheduler");
//            exit(EXIT_FAILURE);
//        }

//        param.sched_priority = 4;
//        if (pthread_setschedparam(io_thread.native_handle(), policy, &param) != 0)
//        {
//            perror("io sched_setscheduler");
//            exit(EXIT_FAILURE);
//        }
//    }

    uint16_t comm_port = 52524;
    uint16_t stream_port = 52525;

    try
    {
#ifdef RASPBERRY_PI
        silk::IO_Board_RPi io_board;
#else
        silk::IO_Board_Sim io_board(io_service);
#endif
        {
            auto res = io_board.connect();
            if (res != silk::IO_Board::Connection_Result::OK)
            {
                SILK_ERR("Cannot connect to the io_board! Aborting");
                abort();
            }
        }

        silk::UAV uav(io_board);
        silk::Camera camera(30);
        silk::Comms comms(io_service, io_board, uav, camera);
        //start listening for a remote system
        {
            auto res = comms.start_listening(comm_port);
            if (res != silk::Comms::Result::OK)
            {
                SILK_ERR("Cannot start communication channel! Aborting");
                abort();
            }
        }

        //create streamer and camera objects
        silk::Video_Server streamer(io_service);
        setup_camera_defaults(camera, streamer);

        {
            auto res = camera.start();
            if (res != silk::Camera::Result::OK)
            {
                SILK_ERR("Cannot start camera! Aborting");
                abort();
            }
        }

        while (true)
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            SILK_INFO("Waiting for comms to connect...");
            if (comms.is_connected() && !streamer.is_started())
            {
                auto res = streamer.start(comms.get_remote_address(), stream_port);
                if (res != silk::Video_Server::Result::OK)
                {
#if defined RASPBERRY_PI
                    SILK_ERR("Video Server failed to start! Aborting");
                    abort();
#else
                    //in simulation mode, the video server is allowed to fail because of UDP
#endif
                }
                break;
            }
        }

        SILK_INFO("All systems up. Ready to fly...");

        while (true)
        {
#if defined RASPBERRY_PI
            if (comms.is_connected() && !streamer.is_started())
            {
                streamer.start(comms.get_remote_address(), stream_port);
            }
#else
            //in simulation mode, the video server is allowed to fail because of UDP
#endif

            comms.process();

            io_board.process();
            uav.process();

            boost::this_thread::sleep_for(boost::chrono::nanoseconds(1));
            //boost::this_thread::yield();
        }

        SILK_INFO("Stopping everything");

        streamer.stop();
        camera.stop();
        comms.disconnect();
        io_board.disconnect();
    }
    catch (std::exception const& e)
    {
        SILK_ERR("exception: {}", e.what());
        abort();
    }

    io_thread.join();

	SILK_INFO("Closing");
}

