#include "BrainStdAfx.h"
#include "HAL.h"
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

//void test_mm(math::vec3f const& target, float step)
//{
//    struct Motor
//    {
//        math::vec3f torque; //precomputed maximum torque
//        float factor;
//    };

//    //some test configuration with random data
//    std::vector<Motor> motors = {{
//                                     {{2, 3, 0.1}, 0},
//                                     {{5, -1, -0.1}, 0},
//                                     {{-1, -7, 0.1}, 0},
//                                     {{-4, 2, -0.1}, 0},
//                                     {{-0.1, .2, -1}, 0},
//                                }};

//    size_t iteration = 0;
//    while (true)
//    {
//        //first calculate the crt torque
//        math::vec3f crt;
//        for (auto& m: motors)
//        {
//            crt += m.torque * m.factor;
//        }

//        //check if we're done
//        if (math::equals(crt, target, 0.0001f))
//        {
//            QLOGI("{}: Done in {} iterations", step, iteration);
//            break;
//        }

//        //how far are we for the target?
//        //divide by motor count because I want to distribute the difference to all motors
//        auto diff = (target - crt) / float(motors.size());

//        //distribute the diff to all motors
//        for (auto& m: motors)
//        {
//            //figure out how much each motor can influence the target torque
//            //  by doing a dot product with the normalized torque vector
//            auto t = math::normalized(m.torque);
//            auto f = math::dot(t, diff) * step; //go toqards the difference in small steps so we can converge
//            m.factor += f;
//        }

//        iteration++;
//    }
//}

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
//    signal(SIGQUIT, signal_handler);
//    signal(SIGABRT, signal_handler);
//    signal(SIGSTOP, signal_handler);

    //set the new_handler
    std::set_new_handler(out_of_memory_handler);

    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    QLOG_TOPIC("silk");

//    q::util::Rand rnd;
//    while (true)
//    {
//        math::vec3f target(rnd.get_float() * 40.f, rnd.get_float() * 40.f, rnd.get_float() * 10.f);
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
            perror("main sched_setscheduler");
            exit(EXIT_FAILURE);
        }
        policy = SCHED_IDLE;
        param.sched_priority = sched_get_priority_min(policy);
        if (pthread_setschedparam(async_thread.native_handle(), policy, &param) != 0)
        {
            perror("async_thread sched_setscheduler");
            exit(EXIT_FAILURE);
        }
    }
#endif

    try
    {
        silk::HAL hal;
        silk::Comms comms(hal);

        if (!hal.init(comms))
        {
            QLOGE("Hardware failure! Aborting");
            abort();
        }

#if defined RASPBERRY_PI
        if (!comms.start_rfmon("mon0", 5))
        {
            QLOGE("Cannot start communication channel! Aborting");
            abort();
        }
#else
        if (!comms.start_udp(8000, 8001))
        {
            QLOGE("Cannot start communication channel! Aborting");
            abort();
        }
#endif

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

        constexpr std::chrono::milliseconds PERIOD(3);

        auto last = q::Clock::now();
        while (!s_exit)
        {
            auto start = q::Clock::now();
            if (start - last >= PERIOD)
            {
                last = start;

                comms.process();
                hal.process();
            }

            //don't sleep too much if we're late
            if (q::Clock::now() - start < PERIOD)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            else
            {
                std::this_thread::yield();
            }
        }

        QLOGI("Stopping everything");

        //stop threads
        async_work.reset();
        s_async_io_service.stop();
        if (async_thread.joinable())
        {
            std::this_thread::yield();
            async_thread.join();
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

