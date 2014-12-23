#include "BrainStdAfx.h"
#include "Video_Server.h"
#include "HAL.h"
#include "common/input/Camera_Input.h"
#include "Comms.h"
#include "UAV.h"

#include <sys/time.h>
#include <sys/resource.h>

size_t s_test = 0;

namespace boost
{
	void throw_exception(std::exception const & e)
	{
		SILK_ERR("boost::exception {}", e.what());
		throw e;
    }
}

static void setup_camera_defaults(silk::HAL_Camera& camera, silk::Video_Server& streamer)
{
//    camera.setup_high_quality(math::vec2u32(1280, 960), 16000000);
//    camera.setup_medium_quality(math::vec2u32(640, 480), 2000000);
//    camera.setup_low_quality(math::vec2u32(320, 240), 160000);

    camera.set_data_callback([&](uint8_t const* data, size_t size)
    {
        streamer.send_frame(silk::Video_Server::Flags(), data, size);
    });

    camera.set_quality(silk::camera_input::Stream_Quality::MEDIUM);
}


int main(int argc, char const* argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));

    namespace po = boost::program_options;

	po::options_description desc("Options");
	desc.add_options()
		("help", "produce help message")
        ("blind", "no camera")
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
    //bool blind = vm.count("blind") != 0;

	boost::asio::io_service io_service;

	SILK_INFO("Creating io_service thread");

    auto io_thread = boost::thread([&io_service]()
	{
		while (true)
		{
            io_service.run();
            io_service.reset();
            boost::this_thread::yield();
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

    uint16_t send_port = 52520;
    uint16_t receive_port = 52521;

    //uint16_t stream_port = 52525;

    try
    {
        silk::HAL hal;
        if (!hal.init())
        {
            SILK_ERR("Hardware failure! Aborting");
            abort();
        }

        silk::UAV uav(hal);
        silk::Comms comms(io_service, hal, uav);
        //start listening for a remote system
        if (!comms.start(send_port, receive_port))
        {
            SILK_ERR("Cannot start communication channel! Aborting");
            abort();
        }

        //create streamer and camera objects
        silk::Video_Server streamer(comms.get_rudp());
        if (hal.camera)
        {
            setup_camera_defaults(*hal.camera, streamer);
        }

//        camera.set_stream_quality(silk::camera_input::Stream_Quality::LOW);


        while (true)
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            SILK_INFO("Waiting for comms to connect...");
            if (comms.is_connected()/* && !streamer.is_started()*/)
            {
                break;
            }
        }

        SILK_INFO("All systems up. Ready to fly...");

        while (true)
        {
            comms.process();

            hal.process();
            uav.process();

            boost::this_thread::yield();
        }

        SILK_INFO("Stopping everything");
    }
    catch (std::exception const& e)
    {
        SILK_ERR("exception: {}", e.what());
        abort();
    }

    io_thread.join();

	SILK_INFO("Closing");
}

