#include "BrainStdAfx.h"
#include "UAV.h"
#include "Comms.h"

#include <sys/time.h>
#include <sys/resource.h>

#include <boost/program_options.hpp>
#include <thread>
#include <iostream>

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

void out_of_memory_handler()
{
    QLOGE("Out of memory");
    std::abort();
}

int main(int argc, char const* argv[])
{
    signal(SIGINT, signal_handler); // Trap basic signals (exit cleanly)
    signal(SIGKILL, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGQUIT, signal_handler);
//    signal(SIGABRT, signal_handler);
    signal(SIGTERM, signal_handler);

    //set the new_handler
    std::set_new_handler(out_of_memory_handler);

    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    QLOG_TOPIC("silk");

//    q::util::Rand rnd;
//    while (true)
//    {
//        math::vec3f target(rnd.get_float() * 40.0, rnd.get_float() * 40.0, rnd.get_float() * 10.0);
//        test_mm(target, 0.01);
//        test_mm(target, 0.1);
//        test_mm(target, 0.5);
//    }

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

    boost::shared_ptr<boost::asio::io_service::work> async_work(new boost::asio::io_service::work(s_async_io_service));
    auto async_thread = std::thread([]() { s_async_io_service.run(); });

#if defined RASPBERRY_PI
    {
        int policy = SCHED_FIFO;
        struct sched_param param;
        param.sched_priority = sched_get_priority_max(policy);
        if (pthread_setschedparam(pthread_self(), policy, &param) != 0)
        {
            perror("Failed to set priority for main thread");
            //exit(EXIT_FAILURE);
        }
        policy = SCHED_IDLE;
        param.sched_priority = sched_get_priority_min(policy);
        if (pthread_setschedparam(async_thread.native_handle(), policy, &param) != 0)
        {
            perror("Failed to set priority for async thread");
            //exit(EXIT_FAILURE);
        }
    }
#endif

    try
    {
        silk::UAV uav;
        silk::Comms comms(uav);

        if (!uav.init(comms))
        {
            QLOGE("Hardware failure! Aborting");
            goto exit;
        }

        uav.load_type_system();
        uav.save_settings2();

//#if defined RASPBERRY_PI
//        if (!comms.start_rfmon("mon0", 5))
//        {
//            QLOGE("Cannot start communication channel! Aborting");
//            goto exit;
//        }
//#else
        if (!comms.start_udp(8000, 8001))
        {
            QLOGE("Cannot start communication channel! Aborting");
            goto exit;
        }
//#endif

//        while (!s_exit)
//        {
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//            QLOGI("Waiting for comms to connect...");
//            if (comms.is_connected())
//            {
//                break;
//            }
//        }

        QLOGI("All systems up. Ready to fly...");

        {
            constexpr std::chrono::microseconds PERIOD(5000);
            auto last = q::Clock::now();
            while (!s_exit)
            {
                auto start = q::Clock::now();
                auto dt = start - last;
                last = start;
                if (dt > std::chrono::milliseconds(5))
                {
                    QLOGW("Process Latency of {}!!!!!", dt);
                }
                comms.process();
                uav.process();

                //No sleeping here!!! process as fast as possible as the nodes are not always in the ideal order
                // and out of order nodes will be processes next 'frame'. So the quicker the frames, the smaller the lag between nodes
#ifndef RASPBERRY_PI
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
            }
        }

exit:
        QLOGI("Stopping everything");

        //stop threads
        async_work.reset();
        s_async_io_service.stop();
        if (async_thread.joinable())
        {
            std::this_thread::yield();
            async_thread.join();
        }
        uav.shutdown();
    }
    catch (std::exception const& e)
    {
        QLOGE("exception: {}", e.what());
        abort();
    }

    QLOGI("Closing");
}

