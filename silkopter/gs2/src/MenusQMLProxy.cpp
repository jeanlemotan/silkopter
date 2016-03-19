#include "MenusQMLProxy.h"

#include <QQmlEngine>
#include <QQmlContext>


MenusQMLProxy::MenusQMLProxy(QObject *parent)
    : QObject(parent)
{
}

void MenusQMLProxy::init(QQuickView& view)
{
    m_view = &view;
}

void MenusQMLProxy::pop()
{
    QASSERT(m_stack.size() >= 1);
    QASSERT(!m_isLocked);
    if (!m_isLocked)
    {
        m_isLocked = true;
        if (m_stack.size() < 1)
        {
            m_stack.clear();
        }
        else
        {
            m_stack.pop_back();
            QTimer::singleShot(0, this, SLOT(setView()));
        }
    }
}

void MenusQMLProxy::push(const QString& qml)
{
    QASSERT(!m_isLocked);
    if (!m_isLocked)
    {
        m_isLocked = true;

        m_stack.push_back(QUrl("qrc:/forms/" + qml));
        QTimer::singleShot(0, this, SLOT(setView()));
    }
}

void MenusQMLProxy::setView()
{
    QASSERT(m_isLocked);
    m_view->setSource(m_stack.back());
    m_isLocked = false;
}

