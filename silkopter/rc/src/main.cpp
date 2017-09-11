#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QTimer>
#include <QPalette>
#include <QFontDatabase>
#include "Comms.h"
#include "Video_Decoder.h"
#include "QMLMenus.h"
#include "QMLVideoSurface.h"
#include "QMLHUD.h"

//#include "Menu_System.h"
//#include "Splash_Menu_Page.h"

//#include "Main_Menu_Page.h"

#include "utils/Clock.h"
#include "HAL.h"

namespace silk
{

int s_version_major = 1;
int s_version_minor = 0;

std::string s_program_path;

silk::HAL s_hal;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

//This prints an "Assertion failed" message and aborts.
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void signal_callback_handler(int signum)
{
   printf("Caught signal %d\n", signum);
   silk::s_hal.shutdown();
   exit(signum);
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

    signal(SIGHUP, signal_callback_handler);
    signal(SIGINT, signal_callback_handler);
    signal(SIGCONT, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    silk::s_program_path = argv[0];
    size_t off = silk::s_program_path.find_last_of('/');
    if (off != std::string::npos)
    {
        silk::s_program_path = silk::s_program_path.substr(0, off);
    }
    QLOGI("Program path: {}.", silk::s_program_path);


    silk::HAL& hal = silk::s_hal;
    ts::Result<void> result = hal.init();
    if (result != ts::success)
    {
        QLOGE("Cannot init hal: {}.", result.error().what());
        exit(1);
    }

    Q_INIT_RESOURCE(res);

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Silkopter");
    QCoreApplication::setOrganizationDomain("silkopter.com");
    QCoreApplication::setApplicationName("Silkopter");

    QFontDatabase::addApplicationFont(":/fonts/Play-Bold.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/Play-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    QGuiApplication::setFont(font);

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

    QMLMenus menus;
    menus.init(view);

    QMLHUD hud;
    hud.init(hal);

    qmlRegisterType<QMLHUD>("com.silk.HUD", 1, 0, "HUD");
    view.engine()->rootContext()->setContextProperty("s_hud", &hud);
    view.engine()->rootContext()->setContextProperty("s_menus", &menus);
    qmlRegisterType<QMLVideoSurface>("com.silk.VideoSurface", 0, 1, "VideoSurface");

    Video_Decoder decoder;

    size_t render_frames = 0;

    q::Clock::time_point last_swapped_tp = q::Clock::now();
    QObject::connect(&view, &QQuickView::frameSwapped, [&render_frames, &decoder, &last_swapped_tp]()
    {
//        std::chrono::duration<float> dt = q::Clock::now() - last_swapped_tp;
//        last_swapped_tp = q::Clock::now();
//        QLOGI("Frame DT: {}", dt.count());

        decoder.release_buffers();
        render_frames++;
    });

    QObject::connect(&view, &QQuickView::sceneGraphInitialized, [&decoder]()
    {
        decoder.init();
        QMLVideoSurface::init(decoder);
    });

    std::vector<uint8_t> video_data;
    math::vec2u16 resolution;
    QObject::connect(&view, &QQuickView::beforeRendering, [&video_data, &resolution, &decoder]()
    {
        decoder.decode_data(video_data.data(), video_data.size(), resolution);
        video_data.clear();
        QMLVideoSurface::setResolution(resolution);
    });

    QSurfaceFormat format = view.format();
    format.setAlphaBufferSize(0);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setSamples(1);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    //format.setSwapInterval(0);
    view.setFormat(format);

    view.setClearBeforeRendering(false);
    view.resize(800, 480);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setPersistentOpenGLContext(true);
    view.create();
    view.show();

    menus.push("HUD.qml");

    hal.get_comms().get_video_streamer().on_data_received = [&video_data, &resolution](void const* data, size_t size, math::vec2u16 const& res)
    {
        size_t offset = video_data.size();
        video_data.resize(offset + size);
        memcpy(video_data.data() + offset, data, size);
        resolution = res;
    };

    q::Clock::time_point last_tp = q::Clock::now();
    size_t process_frames = 0;

    while (true)
    {
        hal.process();
        hud.process();
        app.processEvents();

        process_frames++;
        if (q::Clock::now() - last_tp >= std::chrono::seconds(1))
        {
            last_tp = q::Clock::now();
            QLOGI("P FPS: {}, R FPS: {}", process_frames, render_frames);
            process_frames = 0;
            render_frames = 0;
        }

        //std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    hal.shutdown();

    return 0;
}
