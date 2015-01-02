#pragma once

#include "GPS_Protocol.h"


namespace silk
{

class GPS_UBLOX : public GPS_Protocol
{
public:
    ~GPS_UBLOX();

    auto detect(uint8_t const* data, size_t size) -> bool;

    auto init(int fd) -> bool;
    void process();
    auto get_sample() const -> GPS_Sample const&;

private:
    typedef uint8_t U1;
    typedef int8_t I1;
    typedef uint8_t X1;

    typedef uint16_t U2;
    typedef int16_t I2;
    typedef uint16_t X2;

    typedef uint32_t U4;
    typedef int32_t I4;
    typedef uint32_t X4;

    typedef float R4;
    typedef double R8;

    enum class Class : U1
    {
        NAV = 0x01,
        RXM = 0x02,
        INF = 0x04,
        ACK = 0x05,
        CFG = 0x06,
        MON = 0x0A,
        AID = 0x0B,
        TIM = 0x0D,
        ESF = 0x10
    };

    enum class Message : U1
    {
        ACK_ACK     = 0x01,
        ACK_NACK    = 0x00,

        CFG_PRT     = 0x00,
        CFG_RATE    = 0x08,

        NAV_POLLH   = 0x02,
        NAV_STATUS  = 0x03,
        NAV_SOL     = 0x06,
        NAV_VELNED  = 0x12,
    };


#pragma pack(push, 1)
    struct Header
    {
        U1 preamble1;
        U1 preamble2;
        Class ubx_class;
        Message ubx_message;
        U2 length;
    };
    struct NAV_POLLH
    {
        U4 iTOW; //ms
        I4 lon; //deg
        I4 lat; //deg
        I4 height; //mm
        I4 hMSL; //mm
        U4 hAcc; //mm
        U4 vAcc; //mm
    };
    struct NAV_STATUS
    {
        U4 iTOW; //ms
        U1 gpsFix;
        X1 flags;
        X1 fixStat;
        X1 flags2;
        U4 ttff;
        U4 msss;
    };

    struct NAV_SOL
    {
        U4 iTOW; //ms
        I4 fTOW; //ns
        I2 week;
        U1 gpsFix;
        X1 flags;
        I4 ecefX;//cm
        I4 ecefY;//cm
        I4 ecefZ;//cm
        U4 pAcc;//cm
        I4 ecefVX;//cm/s
        I4 ecefVY;//cm/s
        I4 ecefVZ;//cm/s
        U4 sAcc;//cm/s
        U2 pDOP;
        U1 reserved1;
        U1 numSV;
        U4 reserved2;
    };

#pragma pack(pop)

    struct Packet
    {
        uint8_t step = 0;

        Class cls;
        Message message;
        uint16_t payload_size = 0;
        uint8_t ck_a = 0;
        uint8_t ck_b = 0;
        std::vector<uint8_t> payload;
    } m_packet;


    auto decode_packet(Packet& packet, uint8_t const* data, uint8_t const* end) -> uint8_t const*;
    void process_packet(Packet& packet);
    void process_nav_sol_packet(Packet& packet);
    void process_nav_pollh_packet(Packet& packet);
    void process_nav_status_packet(Packet& packet);


    bool m_is_initialized = false;

    std::array<uint8_t, 1024> m_buffer;

    std::string m_device;
    int m_fd = -1;
    GPS_Sample m_sample;
};





}
