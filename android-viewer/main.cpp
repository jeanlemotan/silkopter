#include <QGuiApplication>
#include <QtGui>
#include <QtQuick>
#include <QQmlApplicationEngine>

#include "VideoSurface.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Q_INIT_RESOURCE(res);

    QQuickView view;

    qmlRegisterType<VideoSurface>("com.silkopter.VideoSurface", 0, 1, "VideoSurface");

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
