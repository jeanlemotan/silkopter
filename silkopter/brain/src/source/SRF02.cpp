#include "BrainStdAfx.h"
#include "SRF02.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_SRF02.hpp"

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
    , m_init_params(new sz::SRF02::Init_Params())
    , m_config(new sz::SRF02::Config())
{

}

auto SRF02::get_name() const -> std::string const&
{
    return m_init_params->name;
}
auto SRF02::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::IDistance>();
    outputs[0].stream = &m_stream;
    return outputs;
}
auto SRF02::init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    QLOG_TOPIC("srf02::init");

    sz::SRF02::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SRF02 data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init() && set_config(config);
}

auto SRF02::init() -> bool
{
    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(m_init_params->bus);

    if (!m_i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_init_params->rate = math::clamp<size_t>(m_init_params->rate, 1, 12);
    m_stream.rate = m_init_params->rate;

    QLOGI("Probing SRF02 on {}", m_init_params->bus);

    uint8_t rev = 0;
    auto ret = m_i2c->read_register_u8(ADDR, SW_REV_CMD, rev);
    if (!ret || rev == 255)
    {
        QLOGE("Failed to initialize SRF02");
        return false;
    }

    QLOGI("SRF02 Revision: {}", rev);

    m_stream.dt = std::chrono::milliseconds(1000 / m_init_params->rate);
    m_stream.last_time_point = q::Clock::now();
    m_state = 0;

    if (!m_init_params->name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}-distance", m_init_params->name);
        if (!m_hal.get_sources().add(*this) ||
            !m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }

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
    //int min_d = (unsigned int)(buf[2] << 8) | buf[3];

    float distance = static_cast<float>(d) / 100.f; //meters
    //float min_distance = static_cast<float>(min_d) / 100.f; //meters

    m_stream.samples.clear();

    if (distance >= m_config->min_distance && distance <= m_config->max_distance)
    {
        Stream::Sample& sample = m_stream.last_sample;
        sample.value = distance;
        sample.sample_idx++;
        sample.dt = m_stream.dt; //TODO - calculate the dt since the last sample time_point, not since the trigger time
        m_stream.samples.push_back(sample);
    }
}

auto SRF02::set_config(rapidjson::Value const& json) -> bool
{
    sz::SRF02::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SRF02 config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    m_config->min_distance = math::max(m_config->min_distance, 0.1f);
    m_config->max_distance = math::min(m_config->max_distance, 12.f);

    return true;
}
auto SRF02::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto SRF02::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
}

