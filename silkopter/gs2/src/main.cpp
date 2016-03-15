#include <QGuiApplication>
#include "Main.h"

//boost::asio::io_service s_async_io_service(4);

int main(int argc, char *argv[])
{
	q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));


//    boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(s_async_io_service));
//    boost::thread_group worker_threads;
//    for(int x = 0; x < 4; ++x)
//    {
//        worker_threads.create_thread(boost::bind(&boost::asio::io_service::run, &s_async_io_service));
//    }

    Q_INIT_RESOURCE(res);

    QGuiApplication a(argc, argv);
//    QApplication::setStyle("fusion");

    QCoreApplication::setOrganizationName("Silkopter");
    QCoreApplication::setOrganizationDomain("silkopter.com");
	QCoreApplication::setApplicationName("Silkopter");


    a.setQuitOnLastWindowClosed(true);

    Main main;

    auto res = a.exec();

//    work.reset();
//    worker_threads.join_all();

    return res;
}
