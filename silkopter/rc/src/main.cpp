#include "Comms.h"
#include "Menu_System.h"
#include "Splash_Menu_Page.h"

#include "Main_Menu_Page.h"

#include "utils/Clock.h"
#include "HAL.h"

namespace silk
{

int s_version_major = 1;
int s_version_minor = 0;

std::string s_program_path;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* This prints an "Assertion failed" message and aborts.  */
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}


///////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));


    //    boost::shared_ptr<asio::io_service::work> work(new asio::io_service::work(s_async_io_service));
    //    std::thread_group worker_threads;
    //    for(int x = 0; x < 4; ++x)
    //    {
    //        worker_threads.create_thread(boost::bind(&asio::io_service::run, &s_async_io_service));
    //    }

    silk::s_program_path = argv[0];
    size_t off = silk::s_program_path.find_last_of('/');
    if (off != std::string::npos)
    {
        silk::s_program_path = silk::s_program_path.substr(0, off);
    }
    QLOGI("Program path: {}.", silk::s_program_path);


    silk::HAL hal;
    ts::Result<void> result = hal.init();
    if (result != ts::success)
    {
        QLOGE("Cannot init hal: {}.", result.error().what());
        hal.get_display().print(("Cannot init hal: " + result.error().what()).c_str());
        hal.get_display().display();
        exit(1);
    }

    silk::Menu_System menu_system;
    std::unique_ptr<silk::IMenu_Page> mm(new silk::Main_Menu_Page(hal));
    menu_system.push_page(std::unique_ptr<silk::IMenu_Page>(new silk::Splash_Menu_Page(std::move(mm))));

    while (true)
    {
        hal.process();
        menu_system.process(hal.get_input());

        if (hal.render())
        {
            menu_system.render(hal.get_display());
        }

        //std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    hal.shutdown();

    return 0;
}
