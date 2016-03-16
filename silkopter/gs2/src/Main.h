#pragma once

#include <QQuickView>
#include <QQuickItem>

#include "HAL.h"
#include "Comms.h"
#include "CommsQMLProxy.h"


class Main : public QObject
{
    Q_OBJECT
public:
    explicit Main(QObject *parent = 0);

signals:

private slots:
    void showSplash();
    void showMM();
    void showSetup();
    void showMultiConfig();

private:
    QQuickView m_view;

    silk::HAL m_hal;
    silk::Comms m_comms;

    CommsQMLProxy m_comms_proxy;
};
