#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QTimer>
#include "Comms.h"
#include "Comms_QMLProxy.h"
#include "OS_QMLProxy.h"
#include "Menus_QMLProxy.h"
#include "UAV_QMLProxy.h"

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

    QGuiApplication app(argc, argv);
//    QApplication::setStyle("fusion");

    QCoreApplication::setOrganizationName("Silkopter");
    QCoreApplication::setOrganizationDomain("silkopter.com");
	QCoreApplication::setApplicationName("Silkopter");


    app.setQuitOnLastWindowClosed(true);

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, QColor(0xECF0F1));
    palette.setColor(QPalette::Base, QColor(25,25,25));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, QColor(0xECF0F1));
    palette.setColor(QPalette::ToolTipText, QColor(0xECF0F1));
    palette.setColor(QPalette::Text, QColor(0xECF0F1));
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, QColor(0xECF0F1));
    palette.setColor(QPalette::BrightText, Qt::white);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));

    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(palette);

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();


    silk::Comms comms;

    OS_QMLProxy os_proxy;

    Menus_QMLProxy menus_proxy;
    menus_proxy.init(view);

    Comms_QMLProxy comms_proxy;
    comms_proxy.init(comms);

    UAV_QMLProxy uav_proxy;
    uav_proxy.init(comms);

    qmlRegisterType<Comms_QMLProxy>("com.silk.comms", 1, 0, "Comms");
    view.engine()->rootContext()->setContextProperty("s_comms", &comms_proxy);
    view.engine()->rootContext()->setContextProperty("s_os", &os_proxy);
    view.engine()->rootContext()->setContextProperty("s_menus", &menus_proxy);
    view.engine()->rootContext()->setContextProperty("s_uav", &uav_proxy);


    menus_proxy.push("Splash.qml");


    QTimer timer;
    timer.setInterval(5);
    timer.setSingleShot(false);
    QObject::connect(&timer, &QTimer::timeout, [&comms]()
    {
       comms.process_rcp();
       comms.process();
    });
    timer.start();

    auto res = app.exec();

//    work.reset();
//    worker_threads.join_all();

    return res;
}
