#pragma once

#include <QQuickView>
#include <QQuickItem>
#include <functional>

#include "HAL.h"
#include "Comms.h"
#include "CommsQMLProxy.h"
#include "OSQMLProxy.h"


class Main : public QObject
{
    Q_OBJECT
public:
    explicit Main(QObject *parent = 0);

signals:

private slots:
    void poweroffSystem();
    void back();
    void showSplash();
    void showMM();
    void showSetup();
    void showMulticopterSetup();
    void showCommsSetup();

private:
    QQuickView m_view;

    silk::HAL m_hal;
    silk::Comms m_comms;

    CommsQMLProxy m_comms_proxy;
    OSQMLProxy m_os_proxy;

    std::vector<std::function<void()>> m_stack;
};
