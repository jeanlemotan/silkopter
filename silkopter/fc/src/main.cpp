#include "FCStdAfx.h"
#include "HAL.h"
#include "RC_Comms.h"
#include "GS_Comms.h"
#include "utils/Clock.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <sys/time.h>
#include <sys/resource.h>

//#include <boost/program_options.hpp>
#include <thread>
#include <iostream>
#include <malloc.h>

size_t s_test = 0;
bool s_exit = false;
boost::asio::io_service s_async_io_service;

struct Memory
{
    size_t allocation_count = 0;
    size_t free_count = 0;
} s_memory;

namespace boost
{
	void throw_exception(std::exception const & e)
	{
        QLOGE("boost::exception {}", e.what());
		throw e;
    }
}

namespace silk
{
void execute_async_call(std::function<void()> f)
{
    s_async_io_service.post(f);
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* This prints an "Assertion failed" message and aborts.  */
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}

//static void* malloc_hook(size_t size, const void* caller)
//{
//    s_memory.allocation_count++;
//    __malloc_hook = nullptr;
//    printf("\n%d, %d", int(s_memory.allocation_count), int(s_memory.free_count));
//    fflush(stdout);
//    void* ptr = malloc(size);
//    __malloc_hook = &malloc_hook;
//    return ptr;
//}
//void free_hook(void* ptr, const void* caller)
//{
//    s_memory.free_count++;
//    __free_hook = nullptr;
//    printf("\n%d, %d", int(s_memory.allocation_count), int(s_memory.free_count));
//    fflush(stdout);
//    free(ptr);
//    __free_hook = &free_hook;
//}


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

    //__malloc_hook = &malloc_hook;
    //__free_hook = &free_hook;

    //set the new_handler
    std::set_new_handler(out_of_memory_handler);

    std::srand(std::time(0));

    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    QLOG_TOPIC("silk");

//    namespace po = boost::program_options;

//	po::options_description desc("Options");
//	desc.add_options()
//		("help", "produce help message")
//        ("blind", "no camera")
//        ("test", po::value<size_t>(), "test");

//	po::variables_map vm;
//	po::store(po::parse_command_line(argc, argv, desc), vm);
//	po::notify(vm);

//	if (vm.count("help"))
//	{
//        std::cout << desc << "\n";
//		return 1;
//	}

//    s_test = vm.count("test") ? vm["test"].as<size_t>() : size_t(0);
//    //bool blind = vm.count("blind") != 0;

    QLOGI("Creating io_service thread");

    boost::shared_ptr<boost::asio::io_service::work> async_work(new boost::asio::io_service::work(s_async_io_service));
    auto async_thread = boost::thread([]() { s_async_io_service.run(); });

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
        silk::HAL hal;
        silk::RC_Comms rc_comms(hal);
        silk::GS_Comms gs_comms(hal, rc_comms);

        if (!hal.init(rc_comms, gs_comms))
        {
            QLOGE("Hardware failure! Aborting");
            goto exit;
        }

        if (!rc_comms.start())
        {
            QLOGW("Cannot start rc communication channel!");
            goto exit;
        }

        if (!gs_comms.start_udp(8005, 8006))
        {
            QLOGE("Cannot start gs communication channel! Aborting");
            goto exit;
        }

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
            auto last = Clock::now();
            while (!s_exit)
            {
                auto start = Clock::now();
                auto dt = start - last;
                last = start;
#ifdef NDEBUG
                if (dt > std::chrono::milliseconds(5))
#else
                if (dt > std::chrono::milliseconds(50))
#endif
                {
                    QLOGW("Process Latency of {}!!!!!", dt);
                }
                gs_comms.process();
                rc_comms.process();
                hal.process();

                //No sleeping here!!! process as fast as possible as the nodes are not always in the ideal order
                // and out of order nodes will be processes next 'frame'. So the quicker the frames, the smaller the lag between nodes
#ifndef RASPBERRY_PI
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif

//                {
//                    static Clock::time_point last_timestamp = Clock::now();
//                    auto now = Clock::now();
//                    auto dt = now - last_timestamp;
//                    last_timestamp = now;
//                    static Clock::duration min_dt, max_dt, avg_dt;
//                    static int xxx = 0;
//                    min_dt = std::min(min_dt, dt);
//                    max_dt = std::max(max_dt, dt);
//                    avg_dt += dt;
//                    xxx++;
//                    static Clock::time_point xxx_timestamp = Clock::now();
//                    if (now - xxx_timestamp >= std::chrono::milliseconds(1000))
//                    {
//                        xxx_timestamp = now;

//                        QLOGI("min {}, max {}, avg {}", min_dt, max_dt, avg_dt/ xxx);
//                        min_dt = dt;
//                        max_dt = dt;
//                        avg_dt = std::chrono::milliseconds(0);

//                        xxx = 0;
//                    }
//                }
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
        hal.shutdown();
    }
    catch (std::exception const& e)
    {
        QLOGE("exception: {}", e.what());
        abort();
    }

    QLOGI("Closing");
}

