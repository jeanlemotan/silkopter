#include "Comms.h"
#include "QBase.h"

#include <QObject>

const uint16_t k_port = 3333;


/////////////////////////////////////////////////////////////////////////////////////////////

Comms::Comms()
    : m_socketAdapter()
    , m_channel(m_socketAdapter)
{
    QObject::connect(&m_socketAdapter.getSocket(), &QTcpSocket::stateChanged, this, &Comms::onSocketStateChanged);
    QObject::connect(&m_socketAdapter.getSocket(), SIGNAL(error), this, SLOT(onSocketError()));
}

Comms::~Comms()
{
}

bool Comms::init()
{
    m_socketAdapter.start();

    connect("192.168.42.1", k_port);

    return true;
}

void Comms::connect(std::string const& address, uint16_t port)
{
    QLOGI("Trying to connect to {}:{}", address, port);

    disconnect();
    //m_socketAdapter.getSocket().bind(QHostAddress("192.168.42.2"), k_port);
    m_socketAdapter.getSocket().connectToHost(address.c_str(), port, QTcpSocket::ReadWrite);

    m_lastConnectAttemptTP = Clock::now();
}

void Comms::disconnect()
{
    m_socketAdapter.getSocket().disconnectFromHost();
}

void Comms::onSocketError(QAbstractSocket::SocketError error)
{
    QLOGI("Connection error: {}", error);
}

void Comms::onSocketStateChanged(QTcpSocket::SocketState /*socketState*/)
{
    Q_EMIT connectionStatusChanged(getConnectionStatus());
    QLOGI("Connection status changed to {}", m_socketAdapter.getSocket().state());
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

Comms::VideoData const& Comms::getVideoData() const
{
    return m_videoData;
}

Telemetry& Comms::getTelemetry()
{
    return m_telemetry;
}

void Comms::processVideoData()
{
    m_channelData.clear();
    if (m_channel.unpack(m_channelData) != Channel::Unpack_Result::OK)
    {
        return;
    }

    math::vec2u16 resolution;
    size_t offset = 0;
    if (!util::serialization::deserialize(m_channelData, resolution, offset))
    {
        return;
    }
    uint8_t const* src_video_data_ptr = m_channelData.data() + offset;
    size_t src_video_data_size = m_channelData.size() - offset;

    m_videoData.resolution = resolution;
    offset = m_videoData.data.size();
    m_videoData.data.resize(offset + src_video_data_size);
    if (src_video_data_size > 0)
    {
        memcpy(m_videoData.data.data() + offset, src_video_data_ptr, src_video_data_size);
    }
//    __android_log_print(ANDROID_LOG_INFO, "Skptr", "Receiving: %d / %d", (int)m_channelData.size(), (int)(m_channelData.size() - offset));
}

void Comms::processTelemetry()
{
    m_channelData.clear();
    if (m_channel.unpack(m_channelData) != Channel::Unpack_Result::OK)
    {
        return;
    }

    size_t offset = 0;

    silk::stream::IMultirotor_State::Value multirotor_state;
    silk::stream::IMultirotor_Commands::Value multirotor_commands;

    if (!util::serialization::deserialize(m_channelData, multirotor_state, offset) ||
        !util::serialization::deserialize(m_channelData, multirotor_commands, offset))
    {
        return;
    }

    m_telemetry.setData(multirotor_state, multirotor_commands);

//    __android_log_print(ANDROID_LOG_INFO, "Skptr", "Commands: %.2f", m_multirotor_commands.sticks.throttle);
}

void Comms::process()
{
    if (getConnectionStatus() == ConnectionStatus::DISCONNECTED &&
        Clock::now() - m_lastConnectAttemptTP >= std::chrono::seconds(1))
    {
        connect("192.168.42.1", k_port);
    }

    m_videoData.data.clear();

    silk::viewer::Packet_Type message;
    while (m_channel.get_next_message(message))
    {
        switch (message)
        {
        case silk::viewer::Packet_Type::VIDEO_DATA:
            processVideoData();
            break;
        case silk::viewer::Packet_Type::TELEMETRY:
            processTelemetry();
            break;
        }
    }
}

