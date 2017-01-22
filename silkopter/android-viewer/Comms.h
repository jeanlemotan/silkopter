#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <string>

#include "common/stream/IMultirotor_State.h"
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

    bool init(std::string const& address, uint16_t port);

    std::pair<void const*, size_t> getVideoData() const;

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

    void process();

signals:
    void connectionStatusChanged(ConnectionStatus);

private:
    void reset();
    void stateChanged(QTcpSocket::SocketState socketState);

    std::string m_address;
    uint16_t m_port = 0;

    std::vector<uint8_t> m_videoData;

    QTcpSocketAdapter m_socketAdapter;
    util::comms::Channel<silk::viewer::Packet_Type, QTcpSocketAdapter> m_channel;

    mutable std::mutex m_samplesMutex;
};
