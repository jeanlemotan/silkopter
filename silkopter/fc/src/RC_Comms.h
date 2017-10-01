#pragma once

#include "common/Comm_Data.h"
#include "HAL.h"

#include "common/Manual_Clock.h"
#include "common/node/ISource.h"
#include "common/stream/ICamera_Commands.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IVideo.h"
#include "utils/comms/esp8266/Phy.h"
#include "utils/comms/esp8266/Fec_Encoder.h"
#include "utils/comms/esp8266/Pool.h"
#include "utils/comms/esp8266/Queue.h"
#include "utils/Clock.h"
#include "utils/hw/ISPI.h"

namespace util
{
namespace comms
{
class RCP;
class ISocket;
}
}

namespace silk
{

class RC_Comms : q::util::Noncopyable
{
public:
    RC_Comms(HAL& hal);
    ~RC_Comms();

    bool start();

    bool is_connected() const;

    void process();

    Phy const& get_phy() const;
    Phy& get_phy();

    boost::optional<stream::IMultirotor_Commands::Value> const& get_multirotor_commands() const;
    boost::optional<stream::ICamera_Commands::Value> const& get_camera_commands() const;

    void set_multirotor_state(stream::IMultirotor_State::Value const& value);
    void add_video_data(stream::IVideo::Value const& value);

    struct Impl; //this needs to be public...
private:
    void process_rx_packet(rc_comms::Packet_Type packet_type, std::vector<uint8_t> const& data, size_t offset);
    void process_received_data(std::vector<uint8_t> const& data);

    bool create_fec_encoder_tx(Fec_Encoder::TX_Descriptor const& descriptor);

    HAL& m_hal;
    Clock::time_point m_uav_sent_tp = Clock::now();

    boost::optional<stream::ICamera_Commands::Value> m_camera_commands;
    boost::optional<stream::IMultirotor_Commands::Value> m_multirotor_commands;

    mutable std::mutex m_new_camera_commands_mutex;
    boost::optional<stream::ICamera_Commands::Value> m_new_camera_commands;

    mutable std::mutex m_new_multirotor_commands_mutex;
    boost::optional<stream::IMultirotor_Commands::Value> m_new_multirotor_commands;

    mutable std::mutex m_multirotor_state_mutex;
    stream::IMultirotor_State::Value m_multirotor_state;

    std::atomic_int m_phy_rate = { -1 };
    std::atomic_int m_new_phy_rate = { -1 };

    mutable std::mutex m_video_header_mutex;
    rc_comms::Video_Header m_video_header;

    struct Phy_Data
    {
        Phy_Data()
            : tx_queue(32)
            , rx_queue(32)
        {}

        typedef std::vector<uint8_t> Packet;
        typedef Pool<Packet>::Ptr Packet_ptr;
        Pool<Packet> packet_pool;
        Queue<Packet_ptr> tx_queue;
        Queue<Packet_ptr> rx_queue;
        bool thread_exit = false;
        std::thread thread;
    } m_phy_data;

    Phy m_phy;
    std::unique_ptr<Fec_Encoder> m_fec_encoder_tx;
    Clock::time_point m_last_phy_received_tp = Clock::now();

    Clock::time_point m_last_multirotor_state_sent_tp = Clock::now();

    bool m_is_connected = false;

    void phy_thread_proc();
};


}
