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
    item->connect(item, SIGNAL(sig_start()), this, SLOT(showMM()), Qt::QueuedConnection);
}

void Main::showMM()
{
    m_view.setSource(QUrl("qrc:/forms/mm.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(sig_back()), this, SLOT(showSplash()), Qt::QueuedConnection);
}
