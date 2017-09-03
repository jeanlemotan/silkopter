#include "GS.h"
#include <QtWidgets/QApplication>
#include <QNetworkProxyFactory>
#include <boost/thread.hpp>
#include <asio.hpp>

asio::io_service s_async_io_service(4);

int main(int argc, char *argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    std::shared_ptr<asio::io_service::work> work(new asio::io_service::work(s_async_io_service));
    boost::thread_group worker_threads;
    for(int x = 0; x < 4; ++x)
    {
        worker_threads.create_thread(boost::bind(&asio::io_service::run, &s_async_io_service));
    }

    Q_INIT_RESOURCE(res);

    QApplication a(argc, argv);
//    QApplication::setStyle("fusion");

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    QCoreApplication::setOrganizationName("Silkopter");
    QCoreApplication::setOrganizationDomain("silkopter.com");
    QCoreApplication::setApplicationName("Silkopter");

    a.setQuitOnLastWindowClosed(true);

    QLOGI("Current locale is '{}', changing to 'C'", setlocale(LC_ALL, "C"));

    GS w;
    w.show();
    auto res = a.exec();

    work.reset();
    worker_threads.join_all();

    return res;
}
