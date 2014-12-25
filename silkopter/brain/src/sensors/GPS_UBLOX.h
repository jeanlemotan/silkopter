#pragma once

#include "GPS_Interface.h"


namespace silk
{

class GPS_UBLOX : public  GPS_Interface
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


//    enum ubs_protocol_bytes
//    {
//        PREAMBLE1 = 0xb5,
//        PREAMBLE2 = 0x62,
//        CLASS_NAV = 0x01,
//        CLASS_ACK = 0x05,
//        CLASS_CFG = 0x06,
//        MSG_ACK_NACK = 0x00,
//        MSG_ACK_ACK = 0x01,
//        MSG_POSLLH = 0x2,
//        MSG_STATUS = 0x3,
//        MSG_SOL = 0x6,
//        MSG_VELNED = 0x12,
//        MSG_CFG_PRT = 0x00,
//        MSG_CFG_RATE = 0x08,
//        MSG_CFG_SET_RATE = 0x01,
//        MSG_CFG_NAV_SETTINGS = 0x24
//    };
//    enum ubs_nav_fix_type
//    {
//        FIX_NONE = 0,
//        FIX_DEAD_RECKONING = 1,
//        FIX_2D = 2,
//        FIX_3D = 3,
//        FIX_GPS_DEAD_RECKONING = 4,
//        FIX_TIME = 5
//    };
//    enum ubx_nav_status_bits
//    {
//        NAV_STATUS_FIX_VALID = 1
//    };

//    // Packet checksum accumulators
//    uint8_t         _ck_a;
//    uint8_t         _ck_b;

//    // State machine state
//    uint8_t         _step;
//    uint8_t         _msg_id;
//    uint16_t        _payload_length;
//    uint16_t        _payload_counter;

//	// 8 bit count of fix messages processed, used for periodic
//	// processing
//    uint8_t			_fix_count;

//    uint8_t         _class;

//    // do we have new position information?
//    bool            _new_position;

//    // do we have new speed information?
//    bool            _new_speed;

//    uint8_t         _disable_counter;

//    // Buffer parse & GPS state update
//    bool        _parse_gps();

//    // used to update fix between status and position packets
//    Fix_Status  next_fix;

//    bool need_rate_update;
//    uint8_t rate_update_step;
//    uint32_t _last_fix_time;

    struct Detection
    {
        uint8_t payload_length = 0;
        uint8_t payload_counter = 0;
        uint8_t step = 0;
        uint8_t ck_a = 0;
        uint8_t ck_b = 0;
    } m_detection;

    bool m_is_initialized = false;

    std::vector<uint8_t> m_data;

    std::string m_device;
    int m_fd = 0;
    GPS_Sample m_sample;
};





}
