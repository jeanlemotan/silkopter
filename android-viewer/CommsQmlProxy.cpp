#include "CommsQmlProxy.h"

CommsQmlProxy::CommsQmlProxy(QObject *parent)
    : QObject(parent)
{
    m_connectionType = ConnectionType::RFMON;
}

void CommsQmlProxy::init(/*silk::Comms& comms*/)
{
//    m_comms = &comms;
}

CommsQmlProxy::ConnectionStatus CommsQmlProxy::getConnectionStatus() const
{
    return ConnectionStatus::DISCONNECTED;//m_comms->is_connected() ? ConnectionStatus::CONNECTED : ConnectionStatus::DISCONNECTED;
}

CommsQmlProxy::ConnectionType CommsQmlProxy::getConnectionType() const
{
    return m_connectionType;
}
void CommsQmlProxy::setConnectionType(ConnectionType type)
{
    if (m_connectionType != type)
    {
        disconnect();
        m_connectionType = type;
        emit connectionTypeChanged(m_connectionType);
    }
}

void CommsQmlProxy::connect()
{
    disconnect();

    if (m_connectionType == ConnectionType::UDP)
    {
        //m_comms->start_udp(boost::asio::ip::address::from_string("127.0.0.1"), 8001, 8000);
        //m_comms->start_udp(boost::asio::ip::address::from_string("192.168.1.39"), 8001, 8000);
    }
    else if (m_connectionType == ConnectionType::RFMON)
    {
        //m_comms->start("wlan1", 5);
    }

    emit connectionStatusChanged(getConnectionStatus());
}
void CommsQmlProxy::disconnect()
{
    if (getConnectionStatus() != ConnectionStatus::DISCONNECTED)
    {
        //m_comms->disconnect();
        emit connectionStatusChanged(ConnectionStatus::DISCONNECTED);
    }
}


