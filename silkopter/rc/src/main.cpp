#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QTimer>
#include "Comms.h"
#include "Comms_QML_Proxy.h"
#include "OS_QML_Proxy.h"
#include "Menus_QML_Proxy.h"
#include "HAL_QML_Proxy.h"
#include "Video_Renderer.h"
#include "Video_Decoder.h"

#include <QtGui/QOpenGLTexture>

//boost::asio::io_service s_async_io_service(4);

silk::Comms s_comms;
Video_Decoder s_video_decoder;


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

    HAL_QML_Proxy hal_proxy;

    qmlRegisterType<Comms_QML_Proxy>("com.silk.Comms", 1, 0, "Comms");
    qmlRegisterType<HAL_QML_Proxy>("com.silk.HAL", 1, 0, "HAL");
    qmlRegisterType<Video_Renderer>("com.silk.VideoRenderer", 1, 0, "VideoRenderer");
    view.engine()->rootContext()->setContextProperty("s_comms", &comms_proxy);
    view.engine()->rootContext()->setContextProperty("s_os", &os_proxy);
    view.engine()->rootContext()->setContextProperty("s_menus", &menus_proxy);
    view.engine()->rootContext()->setContextProperty("s_hal", &hal_proxy);

    size_t render_frames = 0;

    QObject::connect(&view, &QQuickView::frameSwapped, [&render_frames]()
    {
        s_video_decoder.release_buffers();
        render_frames++;
    });

    QObject::connect(&view, &QQuickView::sceneGraphInitialized, []()
    {
        bool res = s_video_decoder.init();
        QASSERT(res);
    });

    QObject::connect(&view, &QQuickView::beforeRendering, []()
    {
        s_video_decoder.decode_samples(s_comms.get_video_samples());
    });

    QSurfaceFormat format = view.format();
    format.setAlphaBufferSize(0);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setSamples(1);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    format.setSwapInterval(0);
    view.setFormat(format);

    view.setClearBeforeRendering(false);
    view.resize(800, 480);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setPersistentOpenGLContext(true);
    view.create();
    view.show();

    menus_proxy.push("Splash.qml");

    q::Clock::time_point last_tp = q::Clock::now();
    size_t process_frames = 0;

    while (true)
    {
        view.update();
        s_comms.process_rcp();
        s_comms.process();

        app.processEvents();

        process_frames++;
        if (q::Clock::now() - last_tp >= std::chrono::seconds(1))
        {
            last_tp = q::Clock::now();
            QLOGI("P FPS: {}, R FPS: {}", process_frames, render_frames);
            process_frames = 0;
            render_frames = 0;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    return 0;
}
