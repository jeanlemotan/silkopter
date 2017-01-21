#include <QGuiApplication>
#include <QtGui>
#include <QtQuick>
#include <QQmlApplicationEngine>

#include "VideoSurface.h"

#include <thread>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Q_INIT_RESOURCE(res);

    QQuickView view;

    qmlRegisterType<VideoSurface>("com.silkopter.VideoSurface", 0, 1, "VideoSurface");

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
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    while (true)
    {
        app.processEvents();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    return app.exec();
}
