#include "BrainStdAfx.h"
#include "SRF02.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

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

auto SRF02::init(rapidjson::Value const& json) -> bool
{
    sz::SRF02 sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SRF02 data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.bus = m_hal.get_buses().find_by_name<bus::II2C>(sz.bus);
    params.rate = sz.rate;
    params.direction = sz.direction;
    params.min_distance = sz.min_distance;
    params.max_distance = sz.max_distance;
    return init(params);
}

auto SRF02::init(Init_Params const& params) -> bool
{
    QLOG_TOPIC("srf02::init");

    m_params = params;

    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_params.name);
        if (!m_hal.get_sources().add(*this) ||
            !m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }

    return true;
}

auto SRF02::init() -> bool
{
    if (!m_params.bus)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_params.rate = math::clamp<size_t>(m_params.rate, 1, 12);
    m_stream.rate = m_params.rate;

    uint8_t rev = 0;
    auto ret = m_params.bus->read_register_u8(ADDR, SW_REV_CMD, rev);
    if (!ret || rev == 255)
    {
        QLOGE("Failed to initialize SRF02");
        return false;
    }

    QLOGI("SRF02 Revision: {}", rev);

    m_stream.dt = std::chrono::milliseconds(1000 / m_params.rate);
    m_stream.last_time_point = q::Clock::now();
    m_state = 0;

    return true;
}

auto SRF02::get_name() const -> std::string const&
{
    return m_params.name;
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
        m_params.bus->write_register_u8(ADDR, SW_REV_CMD, REAL_RAGING_MODE_CM);
        return; //we have to wait first
    }

    //wait for echo
    if (now - m_stream.last_time_point < m_stream.dt)
    {
        return;
    }

    m_state = 0;

    std::array<uint8_t, 4> buf;
    m_params.bus->read_register(ADDR, RANGE_H, buf.data(), buf.size());

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

