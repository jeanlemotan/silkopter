#pragma once

#include "IGPS.h"
#include "buses/II2C.h"
#include "buses/ISPI.h"
#include "buses/IUART.h"

namespace silk
{
namespace sensors
{

class UBLOX : public IGPS, q::util::Noncopyable
{
public:

    UBLOX(q::String const& name);
    ~UBLOX();

    struct Params
    {
        size_t rate = 5;
    };

    auto init(buses::II2C* bus, Params const& params) -> bool;
    auto init(buses::ISPI* bus, Params const& params) -> bool;
    auto init(buses::IUART* bus, Params const& params) -> bool;

    void process();

    auto get_gps_name() const -> q::String const&;

    auto get_gps_samples() const -> std::vector<GPS_Sample> const&;

private:
    auto init(Params const& params) -> bool;

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    enum class Message : uint16_t
    {
        ACK_ACK     = (0x01 << 8) | 0x05,
        ACK_NACK    = (0x00 << 8) | 0x05,

        CFG_PRT     = (0x00 << 8) | 0x06,
        CFG_MSG     = (0x01 << 8) | 0x06,
        CFG_RATE    = (0x08 << 8) | 0x06,
        CFG_ANT     = (0x13 << 8) | 0x06,
        CFG_SBAS    = (0x16 << 8) | 0x06,

        NAV_POLLH   = (0x02 << 8) | 0x01,
        NAV_STATUS  = (0x03 << 8) | 0x01,
        NAV_SOL     = (0x06 << 8) | 0x01,
        NAV_VELNED  = (0x12 << 8) | 0x01,

        INF_NOTICE  = (0x02 << 8) | 0x04,

        MON_HW      = (0x09 << 8) | 0x0A,
        MON_VER     = (0x04 << 8) | 0x0A,
    };

    buses::II2C* m_i2c = nullptr;
    buses::ISPI* m_spi = nullptr;
    buses::IUART* m_uart = nullptr;

    Params m_params;
    q::String m_name;

    struct Packet
    {
        uint8_t cls;
        Message message;
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

    template<class T>
    auto send_packet(Message mgs, T const& payload) -> bool;

    template<class T>
    auto send_packet_with_retry(Message msg, T const& data, q::Clock::duration timeout, size_t retries) -> bool;

    auto send_packet(Message mgs, uint8_t const* payload, size_t payload_size) -> bool;

    auto wait_for_ack(q::Clock::duration d) -> bool;
    boost::optional<bool> m_ack;


    std::atomic_bool m_is_setup = {false};
    boost::unique_future<void> m_setup_future;


    std::array<uint8_t, 1024> m_temp_buffer;
    std::deque<uint8_t> m_buffer;

    struct Sample
    {
        bool has_nav_status = false;
        bool has_pollh = false;
        bool has_sol = false;
        Location data;
        std::vector<GPS_Sample> samples;
        uint32_t sample_idx = 0;

        q::Clock::time_point last_complete_time_point;
    } m_sample;
};


DECLARE_CLASS_PTR(UBLOX);

}
}
