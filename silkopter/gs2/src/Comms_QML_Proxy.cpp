#include "Comms_QML_Proxy.h"

Comms_QML_Proxy::Comms_QML_Proxy(QObject *parent)
    : QObject(parent)
{
    m_connectionType = ConnectionType::UDP;
}

void Comms_QML_Proxy::init(silk::Comms& comms)
{
    m_comms = &comms;
}

Comms_QML_Proxy::ConnectionStatus Comms_QML_Proxy::getConnectionStatus() const
{
    return m_comms->is_connected() ? ConnectionStatus::CONNECTED : ConnectionStatus::DISCONNECTED;
}

Comms_QML_Proxy::ConnectionType Comms_QML_Proxy::getConnectionType() const
{
    return m_connectionType;
}
void Comms_QML_Proxy::setConnectionType(ConnectionType type)
{
    if (m_connectionType != type)
    {
        disconnect();
        m_connectionType = type;
        emit connectionTypeChanged(m_connectionType);
    }
}

void Comms_QML_Proxy::connect()
{
    disconnect();

    if (m_connectionType == ConnectionType::UDP)
    {
        //m_comms->start_udp(boost::asio::ip::address::from_string("127.0.0.1"), 8001, 8000);
        m_comms->start_udp(boost::asio::ip::address::from_string("192.168.1.39"), 8001, 8000);
    }
    else if (m_connectionType == ConnectionType::RFMON)
    {
        m_comms->start_rfmon("wlan1", 5);
    }

    emit connectionStatusChanged(getConnectionStatus());
}
void Comms_QML_Proxy::disconnect()
{
    if (getConnectionStatus() != ConnectionStatus::DISCONNECTED)
    {
        m_comms->disconnect();
        emit connectionStatusChanged(ConnectionStatus::DISCONNECTED);
    }
}


