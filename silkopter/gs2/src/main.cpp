#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QTimer>
#include "Comms.h"
#include "Comms_QML_Proxy.h"
#include "OS_QML_Proxy.h"
#include "Menus_QML_Proxy.h"
#include "UAV_QML_Proxy.h"
#include "NodeEditor/UAVNodeEditor.h"

//boost::asio::io_service s_async_io_service(4);

silk::Comms s_comms;

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

    QPalette palette = app.palette();
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


    OS_QML_Proxy os_proxy;

    Menus_QML_Proxy menus_proxy;
    menus_proxy.init(view);

    Comms_QML_Proxy comms_proxy;
    comms_proxy.init(s_comms);

    UAV_QML_Proxy uav_proxy;

    qmlRegisterType<Comms_QML_Proxy>("com.silk.Comms", 1, 0, "Comms");
    qmlRegisterType<UAV_QML_Proxy>("com.silk.UAV", 1, 0, "UAV");
    qmlRegisterType<UAVNodeEditor>("com.silk.UAVNodeEditor", 1, 0, "UAVNodeEditor");
    view.engine()->rootContext()->setContextProperty("s_comms", &comms_proxy);
    view.engine()->rootContext()->setContextProperty("s_os", &os_proxy);
    view.engine()->rootContext()->setContextProperty("s_menus", &menus_proxy);
    view.engine()->rootContext()->setContextProperty("s_uav", &uav_proxy);

    view.resize(800, 480);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    menus_proxy.push("Splash.qml");


    QTimer timer;
    timer.setInterval(0);
    timer.setSingleShot(false);
    QObject::connect(&timer, &QTimer::timeout, []()
    {
       s_comms.process_rcp();
       s_comms.process();
    });
    timer.start();

    auto res = app.exec();

//    work.reset();
//    worker_threads.join_all();

    return res;
}
