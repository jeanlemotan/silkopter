#include "BrainStdAfx.h"
#include "SRF02.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

namespace silk
{
namespace node
{
namespace source
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


SRF02::SRF02(HAL& hal)
    : m_hal(hal)
{

}

auto SRF02::init(Init_Params const& params) -> bool
{
    QLOG_TOPIC("srf02::init");

    m_params = params;

    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(params.bus);
    if (!init(params) ||
        !m_hal.get_sources().add<ISonar>(params.name, *this) ||
        !m_hal.get_streams().add<stream::IDistance>(q::util::format2<q::String>("{}/stream", params.name), m_stream))
    {
        return false;
    }
    return true;
}

auto SRF02::init() -> bool
{
    QLOG_TOPIC("srf02::init");
    if (!m_i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    uint8_t rev = 0;
    auto ret = m_i2c->read_register_u8(ADDR, SW_REV_CMD, rev);
    if (!ret || rev == 255)
    {
        QLOGE("Failed to initialize SRF02");
        return false;
    }

    QLOGI("SRF02 Revision: {}", rev);

    m_stream.dt = math::clamp(std::chrono::milliseconds(1000 / m_params.rate),
                       std::chrono::milliseconds(100), std::chrono::milliseconds(1000));
    m_stream.last_time_point = q::Clock::now();
    m_state = 0;

    return true;
}

void SRF02::process()
{
    QLOG_TOPIC("srf02::process");
    auto now = q::Clock::now();

    //begin?
    if (m_state == 0)
    {
        if (now - m_stream.last_time_point < m_stream.dt)
        {
            return;
        }

        m_state = 1;
        m_stream.last_time_point = now;
        m_i2c->write_register_u8(ADDR, SW_REV_CMD, REAL_RAGING_MODE_CM);
        return; //we have to wait first
    }

    //wait for echo
    if (now - m_stream.last_time_point < m_stream.dt)
    {
        return;
    }

    m_state = 0;

    std::array<uint8_t, 4> buf;
    m_i2c->read_register(ADDR, RANGE_H, buf.data(), buf.size());

    int d = (unsigned int)(buf[0] << 8) | buf[1];
    int min_d = (unsigned int)(buf[2] << 8) | buf[3];

    float distance = static_cast<float>(d) / 100.f; //meters
    float min_distance = static_cast<float>(min_d) / 100.f; //meters

    m_stream.samples.clear();

    if (distance >= m_params.min_distance && distance <= m_params.max_distance)
    {
        Stream::Sample sample;
        sample.value = distance;
        sample.sample_idx = ++m_stream.sample_idx;
        sample.dt = m_stream.dt; //TODO - calculate the dt since the last sample time_point, not since the trigger time
        m_stream.samples.push_back(sample);
    }
}

auto SRF02::get_stream() -> stream::IDistance&
{
    return m_stream;
}


}
}
}

