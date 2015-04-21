#include "BrainStdAfx.h"
#include "HAL.h"
#include "Comms.h"

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

void test_mm(math::vec3f const& target, float step)
{
    struct Motor
    {
        math::vec3f torque; //precomputed maximum torque
        float factor;
    };

    //some test configuration with random data
    std::vector<Motor> motors = {{
                                     {{2, 3, 0.1}, 0},
                                     {{5, -1, -0.1}, 0},
                                     {{-1, -7, 0.1}, 0},
                                     {{-4, 2, -0.1}, 0},
                                     {{-0.1, .2, -1}, 0},
                                }};

    size_t iteration = 0;
    while (true)
    {
        //first calculate the crt torque
        math::vec3f crt;
        for (auto& m: motors)
        {
            crt += m.torque * m.factor;
        }

        //check if we're done
        if (math::equals(crt, target, 0.0001f))
        {
            QLOGI("{}: Done in {} iterations", step, iteration);
            break;
        }

        //how far are we for the target?
        //divide by motor count because I want to distribute the difference to all motors
        auto diff = (target - crt) / float(motors.size());

        //distribute the diff to all motors
        for (auto& m: motors)
        {
            //figure out how much each motor can influence the target torque
            //  by doing a dot product with the normalized torque vector
            auto t = math::normalized(m.torque);
            auto f = math::dot(t, diff) * step; //go toqards the difference in small steps so we can converge
            m.factor += f;
        }

        iteration++;
    }
}

int main(int argc, char const* argv[])
{
    signal(SIGINT, signal_handler); // Trap basic signals (exit cleanly)
    signal(SIGKILL, signal_handler);
    signal(SIGUSR1, signal_handler);

    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIME, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

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

    std::string xxx("x");

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

    auto async_thread = boost::thread([]()
    {
        while (!s_exit)
        {
            s_async_io_service.run();
            s_async_io_service.reset();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
        }
    });

//    {
//        int policy = SCHED_FIFO;
//        struct sched_param param;
//        param.sched_priority = 10;

//        if (pthread_setschedparam(pthread_self(), policy, &param) != 0)
//        {
//            perror("main sched_setscheduler");
//            exit(EXIT_FAILURE);
//        }

////        param.sched_priority = 4;
////        if (pthread_setschedparam(io_thread.native_handle(), policy, &param) != 0)
////        {
////            perror("io sched_setscheduler");
////            exit(EXIT_FAILURE);
////        }
//    }

    uint16_t send_port = 52520;
    uint16_t receive_port = 52521;

    //uint16_t stream_port = 52525;

    try
    {
        silk::HAL hal;
        silk::Comms comms(io_service, hal);

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
            auto now = q::Clock::now();
            if (now - last >= PERIOD)
            {
                last = now;

                comms.process();
                hal.process();
            }
            //boost::this_thread::yield();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
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
        hal.shutdown();
    }
    catch (std::exception const& e)
    {
        QLOGE("exception: {}", e.what());
        abort();
    }

    QLOGI("Closing");
}

