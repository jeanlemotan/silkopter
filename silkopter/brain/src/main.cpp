#include "BrainStdAfx.h"
#include "HAL.h"
#include "Comms.h"
#include "UAV.h"

#include <sys/time.h>
#include <sys/resource.h>

size_t s_test = 0;
bool s_exit = false;
boost::asio::io_service s_async_io_service;

namespace boost
{
	void throw_exception(std::exception const & e)
	{
        QLOGE("boost::exception {}", e.what());
		throw e;
    }
}

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_handler(int signum)
{
    if (s_exit)
    {
        QLOGI("Forcing an exit due to signal {}", signum);
        abort();
    }
    s_exit = true;
    QLOGI("Exitting due to signal {}", signum);
}


int main(int argc, char const* argv[])
{
    signal(SIGINT, signal_handler); // Trap basic signals (exit cleanly)
    signal(SIGKILL, signal_handler);
    signal(SIGUSR1, signal_handler);

    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIME, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    QLOG_TOPIC("silk");

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

    QLOGI("Creating io_service thread");

    boost::asio::io_service io_service;
    auto io_thread = boost::thread([&io_service]()
    {
        while (!s_exit)
        {
            io_service.run();
            io_service.reset();
            boost::this_thread::sleep_for(boost::chrono::microseconds(500));
        }
    });

    auto async_thread = boost::thread([&s_async_io_service]()
    {
        while (!s_exit)
        {
            s_async_io_service.run();
            s_async_io_service.reset();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        }
    });

    {
        int policy = SCHED_FIFO;
        struct sched_param param;
        param.sched_priority = 10;

        if (pthread_setschedparam(pthread_self(), policy, &param) != 0)
        {
            perror("main sched_setscheduler");
            exit(EXIT_FAILURE);
        }

//        param.sched_priority = 4;
//        if (pthread_setschedparam(io_thread.native_handle(), policy, &param) != 0)
//        {
//            perror("io sched_setscheduler");
//            exit(EXIT_FAILURE);
//        }
    }

    uint16_t send_port = 52520;
    uint16_t receive_port = 52521;

    //uint16_t stream_port = 52525;

    try
    {
        silk::HAL hal;
        silk::UAV uav(hal);
        silk::Comms comms(io_service, hal, uav);

        if (!hal.init(comms))
        {
            QLOGE("Hardware failure! Aborting");
            abort();
        }

        //start listening for a remote system
        if (!comms.start(send_port, receive_port))
        {
            QLOGE("Cannot start communication channel! Aborting");
            abort();
        }

        if (hal.camera)
        {
            hal.camera->set_data_callback([&](uint8_t const* data, size_t size)
            {
                comms.send_video_frame(silk::Comms::Video_Flags(), data, size);
            });
            hal.camera->set_stream_quality(silk::comms::Camera_Params::Stream_Quality::MEDIUM);
        }

//        camera.set_stream_quality(silk::camera_input::Stream_Quality::LOW);


        while (!s_exit)
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            QLOGI("Waiting for comms to connect...");
            if (comms.is_connected())
            {
                break;
            }
        }

        QLOGI("All systems up. Ready to fly...");

        constexpr std::chrono::milliseconds PERIOD(3);

        auto last = q::Clock::now();
        while (!s_exit)
        {
            auto d = q::Clock::now() - last;
            if (d >= PERIOD)
            {
                comms.process();

                hal.process();
                uav.process();
            }
            boost::this_thread::yield();
        }

        QLOGI("Stopping everything");

        //stop threads
        if (io_thread.joinable())
        {
            boost::this_thread::yield();
            io_thread.join();
        }
        if (async_thread.joinable())
        {
            boost::this_thread::yield();
            async_thread.join();
        }

        if (hal.camera)
        {
            hal.camera->set_data_callback(nullptr);
        }
        hal.shutdown();
    }
    catch (std::exception const& e)
    {
        QLOGE("exception: {}", e.what());
        abort();
    }

    QLOGI("Closing");
}

