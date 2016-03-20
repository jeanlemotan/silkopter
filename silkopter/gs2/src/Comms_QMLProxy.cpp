#include "Comms_QMLProxy.h"
#include "utils/RCP_RFMON_Socket.h"

Comms_QMLProxy::Comms_QMLProxy(QObject *parent)
    : QObject(parent)
{

}

void Comms_QMLProxy::init(silk::Comms& comms)
{
    m_comms = &comms;
}

Comms_QMLProxy::ConnectionStatus Comms_QMLProxy::getConnectionStatus() const
{
    return m_comms->is_connected() ? ConnectionStatus::CONNECTED : ConnectionStatus::DISCONNECTED;
}

Comms_QMLProxy::ConnectionType Comms_QMLProxy::getConnectionType() const
{
    return m_connectionType;
}
void Comms_QMLProxy::setConnectionType(ConnectionType type)
{
    if (m_connectionType != type)
    {
        disconnect();
        m_connectionType = type;
        emit connectionTypeChanged(m_connectionType);
    }
}

void Comms_QMLProxy::connect()
{
    disconnect();

    if (m_connectionType == ConnectionType::UDP)
    {
        m_comms->start_udp(boost::asio::ip::address::from_string("127.0.0.1"), 8001, 8000);
    }
    else if (m_connectionType == ConnectionType::RFMON)
    {
        m_comms->start_rfmon("wlan1", 5);
    }

    emit connectionStatusChanged(getConnectionStatus());
}
void Comms_QMLProxy::disconnect()
{
    if (getConnectionStatus() != ConnectionStatus::DISCONNECTED)
    {
        m_comms->disconnect();
        emit connectionStatusChanged(ConnectionStatus::DISCONNECTED);
    }
}


