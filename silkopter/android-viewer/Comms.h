#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <string>

#include "Telemetry.h"
#include "common/Comm_Data.h"
#include "utils/comms/Channel.h"
#include "QTcpSocketAdapter.h"

class Comms : public QObject
{
    Q_OBJECT
public:
    Comms();
    ~Comms();

    Q_PROPERTY(ConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(Telemetry* telemetry READ getTelemetry)

    bool init(std::string const& address, uint16_t port);

    struct VideoData
    {
        std::vector<uint8_t> data;
        math::vec2u16 resolution;
    };

    VideoData const& getVideoData() const;

    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();

    enum class ConnectionStatus
    {
        DISCONNECTED = 0,
        CONNECTING,
        CONNECTED,
        FAILED
    };
    Q_ENUMS(ConnectionStatus);

    ConnectionStatus getConnectionStatus() const;
    Telemetry* getTelemetry();

    void process();

signals:
    void connectionStatusChanged(ConnectionStatus);

private:
    void reset();
    void stateChanged(QTcpSocket::SocketState socketState);

    void processVideoData();
    void processTelemetry();

    std::string m_address;
    uint16_t m_port = 0;

    std::vector<uint8_t> m_channelData;

    VideoData m_videoData;
    Telemetry m_telemetry;

    QTcpSocketAdapter m_socketAdapter;
    typedef util::comms::Channel<silk::viewer::Packet_Type, QTcpSocketAdapter> Channel;
    Channel m_channel;

    mutable std::mutex m_samplesMutex;
};
