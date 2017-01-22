#include "Comms.h"

#include <QObject>

/////////////////////////////////////////////////////////////////////////////////////////////

Comms::Comms()
    : m_socketAdapter()
    , m_channel(m_socketAdapter)
{
}

Comms::~Comms()
{
}

bool Comms::init(std::string const& address, uint16_t port)
{
    m_address = address;
    m_port = port;

    m_socketAdapter.start();

    return true;
}

void Comms::connect()
{
    disconnect();
    m_socketAdapter.getSocket().connectToHost(m_address.c_str(), m_port, QTcpSocket::ReadWrite);

    QObject::connect(&m_socketAdapter.getSocket(), &QTcpSocket::stateChanged, this, &Comms::stateChanged);
}

void Comms::disconnect()
{
    reset();
}

void Comms::stateChanged(QTcpSocket::SocketState socketState)
{
    Q_EMIT connectionStatusChanged(getConnectionStatus());
}

Comms::ConnectionStatus Comms::getConnectionStatus() const
{
    switch (m_socketAdapter.getSocket().state())
    {
    case QTcpSocket::HostLookupState:
    case QTcpSocket::ConnectingState:
    case QTcpSocket::ListeningState: return ConnectionStatus::CONNECTING;
    case QTcpSocket::ConnectedState: return ConnectionStatus::CONNECTED;
    default: return ConnectionStatus::DISCONNECTED;
    }
}

void Comms::reset()
{
    m_socketAdapter.getSocket().disconnectFromHost();
}

std::pair<void const*, size_t> Comms::getVideoData() const
{
    if (m_videoData.empty())
    {
        return std::pair<void const*, size_t>(nullptr, 0);
    }
    else
    {
        return std::pair<void const*, size_t>(m_videoData.data(), m_videoData.size());
    }
}

void Comms::process()
{
    m_videoData.clear();
    m_socketAdapter.read(m_videoData);
}

