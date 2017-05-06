#include <QGuiApplication>
#include <QtGui>
#include <QtQuick>
#include <QQmlApplicationEngine>

#include "QBase.h"
#include "VideoSurface.h"
#include "Comms.h"
#include "Telemetry.h"
#include "OS.h"
#include "Menus.h"

#include <thread>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    QLOG_TOPIC("silk");

    Q_INIT_RESOURCE(res);

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

    OS os;

    Menus menus;
    menus.init(view);

    Comms comms;
    comms.init();

    qmlRegisterType<Telemetry>("com.silk.Telemetry", 1, 0, "Telemetry");
    qmlRegisterType<Comms>("com.silk.Comms", 1, 0, "Comms");
    view.engine()->rootContext()->setContextProperty("s_comms", &comms);
    view.engine()->rootContext()->setContextProperty("s_telemetry", &comms.getTelemetry());
    view.engine()->rootContext()->setContextProperty("s_os", &os);
    view.engine()->rootContext()->setContextProperty("s_menus", &menus);
    qmlRegisterType<VideoSurface>("com.silk.VideoSurface", 0, 1, "VideoSurface");

    QSurfaceFormat format = view.format();
    format.setAlphaBufferSize(0);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setSamples(1);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(0);
    view.setFormat(format);

    //https://github.com/jeanleflambeur/silkopter/issues/40
    //to prevent the app reverting to 60 FPS after focus lost
    QObject::connect(&app, &QGuiApplication::applicationStateChanged, [](Qt::ApplicationState state)
    {
        if (state == Qt::ApplicationInactive)
        {
            QLOGI("Viewer is inactive");
#ifdef Q_ANDROID
            //exit(0); //abort, because QT will default to 60 FPS when coming back and I found no way to fix this
#endif
        }
    });

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    //view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    //menus.push("Splash.qml");
    menus.push("MM.qml");

    while (true)
    {
        app.processEvents();

//        {
//            static FILE* fff = nullptr;
//            if (!fff)
//            {
//                srand(time(nullptr));
//                fff = fopen("/storage/emulated/0/Download/sample.h264", "rb");
//                if (!fff)
//                {
//                    exit(1);
//                }
//            }

//            uint8_t data[32768];
//            size_t size = 100;
//            int r = fread(data, 1, size, fff);
//            if (r == 0)
//            {
//                __android_log_print(ANDROID_LOG_INFO, "Skptr", "DONE, REWIND!!!!!");
//                fseek(fff, 0, SEEK_SET);
//            }
//            if (r > 0)
//            {
//                VideoSurface::decodeVideo(data, r);
//            }
//        }

        comms.process();
        Comms::VideoData const& videoData = comms.getVideoData();
        if (!videoData.data.empty())
        {
            VideoSurface::addVideoData(videoData.data.data(), videoData.data.size(), videoData.resolution);
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    return app.exec();
}
