#include "Main.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QProcess>


Main::Main(QObject *parent)
    : QObject(parent)
    , m_hal(m_comms)
    , m_comms(m_hal)
{
    // Using QQuickView
    m_view.setResizeMode(QQuickView::SizeRootObjectToView);
    m_view.show();

    m_comms_proxy.init(m_comms);

    qmlRegisterType<CommsQMLProxy>("com.silk.comms", 1, 0, "Comms");
    m_view.engine()->rootContext()->setContextProperty("s_comms", &m_comms_proxy);

    m_view.engine()->rootContext()->setContextProperty("s_os", &m_os_proxy);

    showSplash();
}

void Main::back()
{
    QASSERT(m_stack.size() >= 2);
    if (m_stack.size() < 2)
    {
        m_stack.clear();
        showSplash();
    }
    else
    {
        m_stack.pop_back(); //pop back the current level
        auto func = m_stack.back(); //this is the desired level
        m_stack.pop_back(); //pop back the desired level as it will push itself again
        func(); //call the desired level
    }
}

void Main::showSplash()
{
    m_stack.push_back(std::bind(&Main::showSplash, this));

    m_view.setSource(QUrl("qrc:/forms/Splash.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(startPressed()), this, SLOT(showMM()), Qt::QueuedConnection);
    item->connect(item, SIGNAL(poweroffPressed()), this, SLOT(poweroffSystem()), Qt::QueuedConnection);
}

void Main::poweroffSystem()
{
#ifdef RASPBERRY_PI
    QProcess process;
    process.startDetached("shutdown -P now");
#endif
}

void Main::showMM()
{
    m_stack.push_back(std::bind(&Main::showMM, this));

    m_view.setSource(QUrl("qrc:/forms/MM.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(back()), Qt::QueuedConnection);
    item->connect(item, SIGNAL(setupPressed()), this, SLOT(showSetup()), Qt::QueuedConnection);
}

void Main::showSetup()
{
    m_stack.push_back(std::bind(&Main::showSetup, this));

    m_view.setSource(QUrl("qrc:/forms/Setup.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(back()), Qt::QueuedConnection);
    item->connect(item, SIGNAL(multicopterSetupPressed()), this, SLOT(showMulticopterSetup()), Qt::QueuedConnection);
    item->connect(item, SIGNAL(commsSetupPressed()), this, SLOT(showCommsSetup()), Qt::QueuedConnection);
}
void Main::showMulticopterSetup()
{
    m_stack.push_back(std::bind(&Main::showMulticopterSetup, this));

    m_view.setSource(QUrl("qrc:/forms/MulticopterSetup.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(back()), Qt::QueuedConnection);
}
void Main::showCommsSetup()
{
    m_stack.push_back(std::bind(&Main::showCommsSetup, this));

    m_view.setSource(QUrl("qrc:/forms/CommsSetup.qml"));
    QObject* item = m_view.rootObject();
    item->connect(item, SIGNAL(backPressed()), this, SLOT(back()), Qt::QueuedConnection);
}
