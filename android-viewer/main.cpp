#include <QGuiApplication>
#include <QtGui>
#include <QtQuick>
#include <QQmlApplicationEngine>

#include "VideoSurface.h"
#include "CommsQmlProxy.h"
#include "OSQmlProxy.h"
#include "MenusQmlProxy.h"

#include <thread>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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

    OSQmlProxy osProxy;

    MenusQmlProxy menusProxy;
    menusProxy.init(view);

    CommsQmlProxy comms_proxy;
    comms_proxy.init(/*s_comms*/);

    qmlRegisterType<CommsQmlProxy>("com.silk.Comms", 1, 0, "Comms");
    view.engine()->rootContext()->setContextProperty("s_comms", &comms_proxy);
    view.engine()->rootContext()->setContextProperty("s_os", &osProxy);
    view.engine()->rootContext()->setContextProperty("s_menus", &menusProxy);
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

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    //view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    menusProxy.push("Splash.qml");

    while (true)
    {
        app.processEvents();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    return app.exec();
}
