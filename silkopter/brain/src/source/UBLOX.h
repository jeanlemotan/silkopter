#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/node/stream/IECEF.h"
#include "common/node/stream/IWGS84.h"
#include "common/node/bus/II2C.h"
#include "common/node/bus/ISPI.h"
#include "common/node/bus/IUART.h"


namespace sz
{
namespace UBLOX
{
struct Init_Params;
struct Config;
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

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    bus::II2C_wptr m_i2c;
    bus::ISPI_wptr m_spi;
    bus::IUART_wptr m_uart;

    struct Buses
    {
        bus::II2C_ptr i2c;
        bus::ISPI_ptr spi;
        bus::IUART_ptr uart;
    };

    auto lock(Buses& buses) -> bool;
    void unlock(Buses& buses);

    auto read(Buses& buses, uint8_t* data, size_t max_size) -> size_t;
    auto write(Buses& buses, uint8_t const* data, size_t size) -> bool;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::UBLOX::Init_Params> m_init_params;
    std::shared_ptr<sz::UBLOX::Config> m_config;

    struct Packet
    {
        uint8_t cls;
        uint16_t message;
        std::vector<uint8_t> payload;
    } m_packet;

    auto setup() -> bool;
    void read_data(Buses& buses);

    auto decode_packet(Packet& packet, std::deque<uint8_t>& buffer) -> bool;
    void process_packet(Buses& buses, Packet& packet);

    void process_nav_sol_packet(Buses& buses, Packet& packet);
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


    std::atomic_bool m_is_setup = {false};
    boost::unique_future<void> m_setup_future;


    std::array<uint8_t, 1024> m_temp_buffer;
    std::deque<uint8_t> m_buffer;

    bool m_has_nav_status = false;
    bool m_has_pollh = false;
    bool m_has_sol = false;
    q::Clock::time_point m_last_complete_tp;

    struct ECEF_Stream : public stream::IECEF
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
    };
    mutable std::shared_ptr<ECEF_Stream> m_ecef_stream;

    struct WGS84_Stream : public stream::IWGS84
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
    };
    mutable std::shared_ptr<WGS84_Stream> m_wgs84_stream;
};


}
}
