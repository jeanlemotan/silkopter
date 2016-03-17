#pragma once

#include <QObject>
#include "Comms.h"

class CommsQMLProxy : public QObject
{
    Q_OBJECT
public:
    enum class ConnectionType
    {
        NONE = 0,
        RF_MON,
        UDP
    };
    Q_ENUMS(ConnectionType);

    enum class ConnectionStatus
    {
        NOT_CONNECTED = 0,
        CONNECTING,
        CONNECTED,
        FAILED
    };
    Q_ENUMS(ConnectionStatus);

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)
    Q_PROPERTY(ConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(ConnectionType connectionType READ getConnectionType)
    Q_PROPERTY(QList<QString> rfmonInterfaces READ getRFMONInterfaces)
    Q_PROPERTY(uint8_t rfmonEndpointId READ getRFMONEndpointId)
    Q_PROPERTY(QList<QString> enumerateRFMONInterfaces READ enumerateRFMONInterfaces)

    explicit CommsQMLProxy(QObject *parent = 0);
    void init(silk::Comms& comms);

    bool isConnected() const;
    ConnectionStatus getConnectionStatus() const;

    ConnectionType getConnectionType() const;
    QList<QString> getRFMONInterfaces() const;
    uint8_t getRFMONEndpointId() const;

    QList<QString> enumerateRFMONInterfaces() const;

//    Q_INVOKABLE void connectUDP(uint16_t txPort, uint16_t rxPort);
//    Q_INVOKABLE void connectRFMON(uint8_t endpointId);


signals:
    void connectionStatusChanged(ConnectionStatus);

public slots:

private:
    silk::Comms* m_comms = nullptr;
    ConnectionType m_connectionType = ConnectionType::NONE;
    ConnectionStatus m_connectionStatus = ConnectionStatus::NOT_CONNECTED;
    uint8_t m_rfmonEndpointId = 0;
    QList<QString> m_rfmonInterfaces;
};

