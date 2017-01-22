#pragma once

#include <QObject>
//#include "Comms.h"

class CommsQmlProxy : public QObject
{
    Q_OBJECT
public:
    enum class ConnectionType
    {
        NONE = 0,
        RFMON,
        UDP
    };
    Q_ENUMS(ConnectionType);

    enum class ConnectionStatus
    {
        DISCONNECTED = 0,
        CONNECTING,
        CONNECTED,
        FAILED
    };
    Q_ENUMS(ConnectionStatus);

    Q_PROPERTY(ConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(ConnectionType connectionType READ getConnectionType WRITE setConnectionType NOTIFY connectionTypeChanged)

    explicit CommsQmlProxy(QObject *parent = 0);
    void init(/*silk::Comms& comms*/);

    ConnectionStatus getConnectionStatus() const;

    ConnectionType getConnectionType() const;
    void setConnectionType(ConnectionType type);

    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();

//    Q_INVOKABLE void connectRFMON(uint8_t endpointId);


signals:
    void connectionStatusChanged(ConnectionStatus);
    void connectionTypeChanged(ConnectionType);

public slots:

private:
    //silk::Comms* m_comms = nullptr;
    ConnectionType m_connectionType = ConnectionType::NONE;
    ConnectionStatus m_connectionStatus = ConnectionStatus::DISCONNECTED;
};

