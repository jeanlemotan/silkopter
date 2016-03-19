#include "CommsQMLProxy.h"
#include "utils/RCP_RFMON_Socket.h"

CommsQMLProxy::CommsQMLProxy(QObject *parent)
    : QObject(parent)
{

}

void CommsQMLProxy::init(silk::Comms& comms)
{
    m_comms = &comms;
}

CommsQMLProxy::ConnectionStatus CommsQMLProxy::getConnectionStatus() const
{
    return m_comms->is_connected() ? ConnectionStatus::CONNECTED : ConnectionStatus::DISCONNECTED;
}

CommsQMLProxy::ConnectionType CommsQMLProxy::getConnectionType() const
{
    return m_connectionType;
}
void CommsQMLProxy::setConnectionType(ConnectionType type)
{
    if (m_connectionType != type)
    {
        disconnect();
        m_connectionType = type;
        emit connectionTypeChanged(m_connectionType);
    }
}

void CommsQMLProxy::connect()
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
void CommsQMLProxy::disconnect()
{
    if (getConnectionStatus() != ConnectionStatus::DISCONNECTED)
    {
        m_comms->disconnect();
        emit connectionStatusChanged(ConnectionStatus::DISCONNECTED);
    }
}


