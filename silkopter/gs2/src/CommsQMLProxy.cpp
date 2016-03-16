#include "CommsQMLProxy.h"

CommsQMLProxy::CommsQMLProxy(silk::Comms& comms, QObject *parent)
    : QObject(parent)
    , m_comms(comms)
{

}

bool CommsQMLProxy::isConnected() const
{
    return m_comms.is_connected();
}

CommsQMLProxy::ConnectionType CommsQMLProxy::getConnectionType() const
{
    return isConnected() ? ConnectionType::RF_MON : ConnectionType::NONE;
}

QString CommsQMLProxy::getInterface() const
{
    return "wlan0";
}

uint8_t CommsQMLProxy::getInterfaceId() const
{
    return 3;
}
