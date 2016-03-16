#include "Main.h"

Main::Main(QObject *parent) : QObject(parent)
{
    // Using QQuickView
    m_view.setResizeMode(QQuickView::SizeRootObjectToView);
    m_view.show();

    showSplash();
}

void Main::showSplash()
{
    m_view.setSource(QUrl("qrc:/forms/splash.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(startPressed()), this, SLOT(showMM()), Qt::QueuedConnection);
}

void Main::showMM()
{
    m_view.setSource(QUrl("qrc:/forms/mm.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(showSplash()), Qt::QueuedConnection);
    item->connect(item, SIGNAL(setupPressed()), this, SLOT(showSetup()), Qt::QueuedConnection);
}

void Main::showSetup()
{
    m_view.setSource(QUrl("qrc:/forms/setup.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(showMM()), Qt::QueuedConnection);
    item->connect(item, SIGNAL(multiConfigPressed()), this, SLOT(showMultiConfig()), Qt::QueuedConnection);
}
void Main::showMultiConfig()
{
    m_view.setSource(QUrl("qrc:/forms/MultiConfig.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(showSetup()), Qt::QueuedConnection);
}
