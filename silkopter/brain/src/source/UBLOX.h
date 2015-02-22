#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/stream/ILocation.h"
#include "common/node/bus/II2C.h"
#include "common/node/bus/ISPI.h"
#include "common/node/bus/IUART.h"

namespace silk
{
namespace node
{
namespace source
{

class UBLOX : public ISource
{
public:

    UBLOX(HAL& hal);
    ~UBLOX();

    struct Init_Params
    {
        std::string name;
        bus::IBus* bus = nullptr;
        uint32_t rate = 5;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_name() const -> std::string const&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;

    void process();

private:
    auto init() -> bool;

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    HAL& m_hal;
    bus::II2C* m_i2c = nullptr;
    bus::ISPI* m_spi = nullptr;
    bus::IUART* m_uart = nullptr;

    Init_Params m_params;

    struct Packet
    {
        uint8_t cls;
        uint16_t message;
        std::vector<uint8_t> payload;
    } m_packet;

    auto setup() -> bool;
    void read_data();

    auto decode_packet(Packet& packet, std::deque<uint8_t>& buffer) -> bool;
    void process_packet(Packet& packet);

    void process_nav_sol_packet(Packet& packet);
    void process_nav_pollh_packet(Packet& packet);
    void process_nav_status_packet(Packet& packet);

    void process_cfg_prt_packet(Packet& packet);
    void process_cfg_rate_packet(Packet& packet);
    void process_cfg_sbas_packet(Packet& packet);
    void process_cfg_ant_packet(Packet& packet);
    void process_cfg_msg_packet(Packet& packet);

    void process_inf_notice_packet(Packet& packet);

    void process_mon_hw_packet(Packet& packet);
    void process_mon_ver_packet(Packet& packet);

    template<class T> auto send_packet(uint16_t mgs, T const& payload) -> bool;
    template<class T> auto send_packet_with_retry(uint16_t msg, T const& data, q::Clock::duration timeout, size_t retries) -> bool;
    auto send_packet(uint16_t mgs, uint8_t const* payload, size_t payload_size) -> bool;

    auto wait_for_ack(q::Clock::duration d) -> bool;
    boost::optional<bool> m_ack;


    std::atomic_bool m_is_setup = {false};
    boost::unique_future<void> m_setup_future;


    std::array<uint8_t, 1024> m_temp_buffer;
    std::deque<uint8_t> m_buffer;

    struct Stream : public stream::ILocation
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        std::string name;

        bool has_nav_status = false;
        bool has_pollh = false;
        bool has_sol = false;
        q::Clock::time_point last_complete_time_point;
    } m_stream;
};


}
}
}
