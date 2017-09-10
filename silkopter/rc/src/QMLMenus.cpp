#include "QMLMenus.h"

#include <QTimer>
#include <QQmlEngine>
#include <QQmlContext>


QMLMenus::QMLMenus(QObject *parent)
    : QObject(parent)
{
}

void QMLMenus::init(QQuickView& view)
{
    m_view = &view;
}

void QMLMenus::pop()
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
            QLOGI("Menu pop: {}", m_stack.back().toString().toUtf8().data());
            m_stack.pop_back();
            QTimer::singleShot(0, this, SLOT(setView()));
        }
    }
}

void QMLMenus::push(const QString& qml)
{
    QASSERT(!m_isLocked);
    if (!m_isLocked)
    {
        m_isLocked = true;

//#ifdef RASPBERRY_PI
        m_stack.push_back(QUrl("qrc:/qml/" + qml));
        QLOGI("Menu push: {}", m_stack.back().toString().toUtf8().data());
//#else
        //m_stack.push_back(QUrl("qml/" + qml));
//#endif
        QTimer::singleShot(0, this, SLOT(setView()));
    }
}

void QMLMenus::setView()
{
    QASSERT(m_isLocked);
    QLOGI("Menu set: {}", m_stack.back().toString().toUtf8().data());
    m_view->setSource(m_stack.back());
    m_isLocked = false;

    //recover from bad screens
    if (m_view->status() == QQuickView::Error && m_stack.size() >= 2)
    {
        pop();
    }
}

