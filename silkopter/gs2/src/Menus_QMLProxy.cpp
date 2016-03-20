#include "Menus_QMLProxy.h"

#include <QQmlEngine>
#include <QQmlContext>


Menus_QMLProxy::Menus_QMLProxy(QObject *parent)
    : QObject(parent)
{
}

void Menus_QMLProxy::init(QQuickView& view)
{
    m_view = &view;
}

void Menus_QMLProxy::pop()
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

void Menus_QMLProxy::push(const QString& qml)
{
    QASSERT(!m_isLocked);
    if (!m_isLocked)
    {
        m_isLocked = true;

        m_stack.push_back(QUrl("qrc:/qml/" + qml));
        QTimer::singleShot(0, this, SLOT(setView()));
    }
}

void Menus_QMLProxy::setView()
{
    QASSERT(m_isLocked);
    m_view->setSource(m_stack.back());
    m_isLocked = false;
}

