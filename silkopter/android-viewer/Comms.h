#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <string>

#include "Telemetry.h"
#include "common/Comm_Data.h"
#include "utils/comms/Channel.h"
#include "QTcpSocketAdapter.h"

#include <QUdpSocket>
#include <QTcpServer>

class Comms : public QObject
{
    Q_OBJECT
public:
    Comms();
    ~Comms();

    Q_PROPERTY(ConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)

    bool init();

    struct VideoData
    {
        std::vector<uint8_t> data;
        math::vec2u16 resolution;
    };

    VideoData const& getVideoData() const;

    enum class ConnectionStatus
    {
        DISCONNECTED = 0,
        CONNECTING,
        CONNECTED,
        FAILED
    };
    Q_ENUMS(ConnectionStatus);

    ConnectionStatus getConnectionStatus() const;
    Telemetry& getTelemetry();

    void process();

signals:
    void connectionStatusChanged(ConnectionStatus);

private slots:
    void onSocketError(QAbstractSocket::SocketError error);
    void onSocketStateChanged(QTcpSocket::SocketState socketState);
    void newConnection();

private:

    void processVideoData();
    void processTelemetry();

    std::vector<uint8_t> m_channelData;

    VideoData m_videoData;
    Telemetry m_telemetry;

    QTcpServer* m_tcpServer = nullptr;
    QTcpSocketAdapter m_socketAdapter;
    typedef util::comms::Channel<silk::viewer::Packet_Type, QTcpSocketAdapter> Channel;
    Channel m_channel;

    mutable std::mutex m_samplesMutex;
};
