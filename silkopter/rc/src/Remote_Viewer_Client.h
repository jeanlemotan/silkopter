#pragma once

#include <memory>
#include <asio.hpp>
#include <thread>
#include "common/Comm_Data.h"
#include "common/stream/IMultirotor_State.h"
#include "utils/comms/Channel.h"
#include "utils/comms/ASIO_Socket_Adapter.h"


namespace silk
{

class Remote_Viewer_Client
{
public:
    Remote_Viewer_Client();
    ~Remote_Viewer_Client();

    void send_video_data(void const* video_data, size_t video_data_size, math::vec2u16 const& resolution);
    void send_telemetry(stream::IMultirotor_Commands::Value const& multirotor_commands, stream::IMultirotor_State::Value const& multirotor_state);

    bool is_connected() const;

    void process();

private:
    void start_connect();

    asio::io_service m_io_service;
    std::unique_ptr<asio::io_service::work> m_io_service_work;
    std::unique_ptr<asio::ip::tcp::socket> m_socket;
    std::thread m_io_service_thread;

    mutable std::recursive_mutex m_socket_mutex;
    typedef util::comms::ASIO_Socket_Adapter<asio::ip::tcp::socket> Socket_Adapter;
    Socket_Adapter m_socket_adapter;
    util::comms::Channel<viewer::Packet_Type, Socket_Adapter> m_channel;

    Clock::time_point m_last_connection_attempt_tp = Clock::now();
    std::atomic_bool m_is_connected = { false };


    std::vector<uint8_t> m_serialization_buffer;
};

}
