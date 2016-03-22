#include "Menus_QML_Proxy.h"

#include <QQmlEngine>
#include <QQmlContext>


Menus_QML_Proxy::Menus_QML_Proxy(QObject *parent)
    : QObject(parent)
{
}

void Menus_QML_Proxy::init(QQuickView& view)
{
    m_view = &view;
}

void Menus_QML_Proxy::pop()
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

void Menus_QML_Proxy::push(const QString& qml)
{
    QASSERT(!m_isLocked);
    if (!m_isLocked)
    {
        m_isLocked = true;

        m_stack.push_back(QUrl("qrc:/qml/" + qml));
        QTimer::singleShot(0, this, SLOT(setView()));
    }
}

void Menus_QML_Proxy::setView()
{
    QASSERT(m_isLocked);
    m_view->setSource(m_stack.back());
    m_isLocked = false;
}

