#pragma once

#include <QObject>
#include "Comms.h"

class CommsQMLProxy : public QObject
{
    Q_OBJECT
public:
    Q_ENUMS(ConnectionType);

    enum class ConnectionType
    {
        NONE,
        RF_MON,
        UDP
    };

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)
    Q_PROPERTY(ConnectionType connectionType READ getConnectionType)
    Q_PROPERTY(QString interface READ getInterface)
    Q_PROPERTY(uint8_t interfaceId READ getInterfaceId)

    explicit CommsQMLProxy(silk::Comms& comms, QObject *parent = 0);

    bool isConnected() const;
    ConnectionType getConnectionType() const;
    QString getInterface() const;
    uint8_t getInterfaceId() const;

signals:
    void connectionChanged();

public slots:

private:
    silk::Comms& m_comms;
};

