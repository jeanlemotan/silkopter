#include "BrainStdAfx.h"
#include "SRF02.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "pigpiod_if.h"
}


namespace silk
{

constexpr uint8_t ADDR = 0x70;

//Registers
constexpr uint8_t SW_REV_CMD        = 0x0;
constexpr uint8_t RANGE_H           = 0x2;
constexpr uint8_t RANGE_L           = 0x3;
constexpr uint8_t AUTOTUNE_MIN_H	= 0x4;
constexpr uint8_t AUTOTUNE_MIN_L	= 0x5;

//Commands
constexpr uint8_t REAL_RAGING_MODE_IN       = 0x50;
constexpr uint8_t REAL_RAGING_MODE_CM       = 0x51;
constexpr uint8_t REAL_RAGING_MODE_US       = 0x52;
constexpr uint8_t FAKE_RAGING_MODE_IN       = 0x56;
constexpr uint8_t FAKE_RAGING_MODE_CM       = 0x57;
constexpr uint8_t FAKE_RAGING_MODE_US       = 0x58;
constexpr uint8_t BURST                     = 0x5C;
constexpr uint8_t FORCE_AUTOTUNE_RESTART    = 0x60;

constexpr std::chrono::milliseconds MEASUREMENT_DURATION(80);
constexpr float MAX_VALID_DISTANCE = 5.f;


auto SRF02::init(q::Clock::duration sample_time) -> bool
{
    std::string device("/dev/i2c-0");
    SILK_INFO("initializing device: {}", device);

    if (!m_i2c.open(device))
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }

    uint8_t rev = 0;
    auto ret = m_i2c.read_u8(ADDR, SW_REV_CMD, rev);
    if (!ret || rev == 255)
    {
        SILK_ERR("Failed to initialize SRF02");
        return false;
    }

    SILK_INFO("SRF02 Revision: {}", rev);

    m_sample_time = math::max(sample_time, q::Clock::duration(MEASUREMENT_DURATION));
    m_last_time_point = q::Clock::now();
    m_state = 0;

    return true;
}

void SRF02::process()
{

}

auto SRF02::read_distance() -> boost::optional<float>
{
    auto now = q::Clock::now();

    //begin?
    if (m_state == 0)
    {
        if (now - m_last_time_point < m_sample_time)
        {
            return boost::none;
        }

        m_state = 1;
        m_last_time_point = now;
        m_i2c.write_u8(ADDR, SW_REV_CMD, REAL_RAGING_MODE_CM);
        return boost::none; //we have to wait first
    }

    //wait for echo
    if (now - m_last_time_point < MEASUREMENT_DURATION)
    {
        return boost::none;
    }

    m_state = 0;

    std::array<uint8_t, 4> buf;
    m_i2c.read(ADDR, RANGE_H, buf.data(), buf.size());

    int d = (unsigned int)(buf[0] << 8) | buf[1];
    int min_d = (unsigned int)(buf[2] << 8) | buf[3];

    float distance = static_cast<float>(d) / 100.f; //meters
    float min_distance = static_cast<float>(min_d) / 100.f; //meters

    return (distance > MAX_VALID_DISTANCE) ? boost::none : boost::optional<float>(distance);
}

auto SRF02::get_sample_time() const -> q::Clock::duration
{
    return m_sample_time;
}




}


#endif
