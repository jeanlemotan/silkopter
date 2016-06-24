#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IPosition.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IGPS_Info.h"
#include "common/bus/II2C.h"
#include "common/bus/ISPI.h"
#include "common/bus/IUART.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct UBLOX_Descriptor;
struct UBLOX_Config;
}
}


namespace silk
{
namespace node
{

class UBLOX : public ISource
{
public:

    UBLOX(HAL& hal);
    ~UBLOX();

    bool init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    bool set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::weak_ptr<bus::II2C> m_i2c;
    std::weak_ptr<bus::ISPI> m_spi;
    std::weak_ptr<bus::IUART> m_uart;

    struct Buses
    {
        std::shared_ptr<bus::II2C> i2c;
        std::shared_ptr<bus::ISPI> spi;
        std::shared_ptr<bus::IUART> uart;
    };
    //i use 2 vectors to avoid memsetting the tx_data to zero all the time
    std::vector<uint8_t> m_dummy_tx_data;
    std::vector<uint8_t> m_dummy_rx_data;

    auto lock(Buses& buses) -> bool;
    void unlock(Buses& buses);

    auto read(Buses& buses, uint8_t* data, size_t max_size) -> size_t;
    auto write(Buses& buses, uint8_t const* data, size_t size) -> bool;

    std::shared_ptr<hal::UBLOX_Descriptor> m_descriptor;
    std::shared_ptr<hal::UBLOX_Config> m_config;

    struct Packet
    {
        uint8_t cls;
        uint16_t message;
        std::vector<uint8_t> payload;
    } m_packet;

    auto setup() -> bool;
    void poll_for_data(Buses& buses);
    void reset(Buses& buses);

    void read_data(Buses& buses);

    struct Decoder_State
    {
        enum class Result
        {
            FOUND_PACKET,
            NEEDS_DATA,
            DONE
        };

        size_t data_idx = 0;
        size_t step = 0;
        size_t payload_size = 0;
        uint8_t ck_a = 0;
        uint8_t ck_b = 0;
    };
    Decoder_State m_state;

    auto decode_packet(Packet& packet, std::deque<uint8_t>& buffer) -> Decoder_State::Result;
    void process_packet(Buses& buses, Packet& packet);

    void process_nav_sol_packet(Buses& buses, Packet& packet);
    void process_nav_velecef_packet(Buses& buses, Packet& packet);
    void process_nav_posecef_packet(Buses& buses, Packet& packet);
    void process_nav_pollh_packet(Buses& buses, Packet& packet);
    void process_nav_status_packet(Buses& buses, Packet& packet);

    void process_cfg_prt_packet(Buses& buses, Packet& packet);
    void process_cfg_rate_packet(Buses& buses, Packet& packet);
    void process_cfg_sbas_packet(Buses& buses, Packet& packet);
    void process_cfg_ant_packet(Buses& buses, Packet& packet);
    void process_cfg_msg_packet(Buses& buses, Packet& packet);

    void process_inf_notice_packet(Buses& buses, Packet& packet);

    void process_mon_hw_packet(Buses& buses, Packet& packet);
    void process_mon_ver_packet(Buses& buses, Packet& packet);

    template<class T> auto send_packet(Buses& buses, uint16_t mgs, T const& payload) -> bool;
    template<class T> auto send_packet_with_retry(Buses& buses, uint16_t msg, T const& data, q::Clock::duration timeout, size_t retries) -> bool;
    auto send_packet(Buses& buses, uint16_t mgs, uint8_t const* payload, size_t payload_size) -> bool;

    auto wait_for_ack(Buses& buses, q::Clock::duration d) -> bool;
    boost::optional<bool> m_ack;

    enum class Setup_State
    {
        UNKNOWN,
        RUNNING,
        FAILED,
        DONE
    };

    std::atomic<Setup_State> m_setup_state = {Setup_State::UNKNOWN};
    //boost::unique_future<void> m_setup_future;
    std::future<void> m_setup_future;


    std::array<uint8_t, 1024> m_temp_buffer;
    std::deque<uint8_t> m_buffer;

    typedef Basic_Output_Stream<stream::IECEF_Position> Position_Stream;
    mutable std::shared_ptr<Position_Stream> m_position_stream;

    typedef Basic_Output_Stream<stream::IECEF_Velocity> Velocity_Stream;
    mutable std::shared_ptr<Velocity_Stream> m_velocity_stream;

    typedef Basic_Output_Stream<stream::IGPS_Info> GPS_Info_Stream;
    mutable std::shared_ptr<GPS_Info_Stream> m_gps_info_stream;

    Position_Stream::Value m_last_position_value;
    q::Clock::time_point m_last_position_tp = q::Clock::time_point(q::Clock::duration::zero());

    Velocity_Stream::Value m_last_velocity_value;
    q::Clock::time_point m_last_velocity_tp = q::Clock::time_point(q::Clock::duration::zero());

    GPS_Info_Stream::Value m_last_gps_info_value;
    q::Clock::time_point m_last_gps_info_tp = q::Clock::time_point(q::Clock::duration::zero());

    q::Clock::time_point m_last_process_tp = q::Clock::time_point(q::Clock::duration::zero());
    q::Clock::time_point m_last_reset_tp = q::Clock::now() - std::chrono::seconds(1000);
    q::Clock::time_point m_last_poll_tp = q::Clock::now() - std::chrono::seconds(1000);

    struct Stats
    {
        q::Clock::time_point last_report_tp = q::Clock::now();
        struct Position
        {
            bool operator==(Position const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Position const& o) const { return !operator==(o); }
            size_t added = 0;
        } pos;
        struct Velocity
        {
            bool operator==(Velocity const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Velocity const& o) const { return !operator==(o); }
            size_t added = 0;
        } vel;
        struct Info
        {
            bool operator==(Info const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Info const& o) const { return !operator==(o); }
            size_t added = 0;
        } info;

        bool operator==(Stats const& o) const { return pos == o.pos && vel == o.vel && info == o.info; }
        bool operator!=(Stats const& o) const { return !operator==(o); }
    } m_stats;
};


}
}
