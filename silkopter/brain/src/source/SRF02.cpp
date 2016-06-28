#include "BrainStdAfx.h"
#include "SRF02.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "hal.def.h"

namespace silk
{
namespace node
{


constexpr uint8_t ADDR = 0x70;

//Registers
constexpr uint8_t SW_REV_CMD        = 0x0;
constexpr uint8_t UNUSED            = 0x1;
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


constexpr std::chrono::milliseconds MAX_MEASUREMENT_DURATION(100);


SRF02::SRF02(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::SRF02_Descriptor())
    , m_config(new hal::SRF02_Config())
{
    m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down

    m_output_stream = std::make_shared<Output_Stream>();
}

auto SRF02::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Distance";
    outputs[0].stream = m_output_stream;
    return outputs;
}
ts::Result<void> SRF02::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("srf02::init");

    auto specialized = dynamic_cast<hal::SRF02_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> SRF02::init()
{
    m_bus = m_hal.get_bus_registry().find_by_name<bus::II2C>(m_descriptor->get_bus());
    auto bus = m_bus.lock();
    if (!bus)
    {
        return make_error("No bus configured");
    }

    bus->lock();
    At_Exit at_exit([this, &bus]()
    {
        bus->unlock();
    });

    m_descriptor->set_rate(math::clamp<size_t>(m_descriptor->get_rate(), 1, 10));

    QLOGI("Probing SRF02 on {}...", m_descriptor->get_bus());

    uint32_t tries = 0;
    constexpr uint32_t max_tries = 10;
    while (++tries <= max_tries)
    {
        uint8_t rev = 0, test = 0;
        auto ret = bus->read_register_u8(ADDR, SW_REV_CMD, rev);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ret &= bus->read_register_u8(ADDR, UNUSED, test);
        if (ret && rev != 0 && rev != 255 && test != 0 && test != 255)
        {
            QLOGI("Found SRF02 rev {} after {} tries", rev, tries);//rev is 6 so far
            break;
        }
        QLOGW("\tFailed {} try to initialize SRF02: bus {}, rev {}, test {}", tries, ret, rev, test);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (tries > max_tries)
    {
        return make_error("Failed to initialize SRF02");
    }

    trigger(*bus);

    m_output_stream->set_rate(m_descriptor->get_rate());

    return ts::success;
}

void SRF02::trigger(bus::II2C& bus)
{
    m_last_trigger_tp = q::Clock::now();
    bus.write_register_u8(ADDR, SW_REV_CMD, REAL_RAGING_MODE_CM);
}

ts::Result<void> SRF02::start(q::Clock::time_point tp)
{
    m_last_trigger_tp = tp;
    m_output_stream->set_tp(tp);
    return ts::success;
}

void SRF02::process()
{
    QLOG_TOPIC("srf02::process");

    m_output_stream->clear();

    //wait for echo
    auto now = q::Clock::now();
    if (now - m_last_trigger_tp < MAX_MEASUREMENT_DURATION ||
        now - m_last_trigger_tp < m_output_stream->get_dt())
    {
        return;
    }

    auto bus = m_bus.lock();
    if (!bus)
    {
        return;
    }

    bus->lock();
    At_Exit at_exit([this, &bus]()
    {
        bus->unlock();
    });

    std::array<uint8_t, 4> buf;
    bool res = bus->read_register(ADDR, RANGE_H, buf.data(), buf.size());

    //trigger immediately
    trigger(*bus);

    //TODO - add health indication


    if (res)
    {
        int d = (unsigned int)(buf[0] << 8) | buf[1];
        int min_d = (unsigned int)(buf[2] << 8) | buf[3];

        //QLOGI("d = {}, min_d = {}", d, min_d);

        float distance = static_cast<float>(d) / 100.f; //meters

        float min_distance = math::max(m_config->get_min_distance(), static_cast<float>(min_d) / 100.f); //meters
        float max_distance = m_config->get_max_distance();
        math::vec3f value = m_config->get_direction() * math::clamp(distance, min_distance, max_distance);
        bool is_healthy = distance >= min_distance && distance <= max_distance;

        m_output_stream->clear();
        auto samples_needed = m_output_stream->compute_samples_needed();
        while (samples_needed > 0)
        {
            m_output_stream->push_sample(value, is_healthy);
            samples_needed--;
        }
    }
}

ts::Result<void> SRF02::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("srf02::set_config");

    auto specialized = dynamic_cast<hal::SRF02_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_config = *specialized;

    //    m_config->set_min_distance(math::max(m_config->get_min_distance(), 0.1f));
//    m_config->set_max_distance(math::min(m_config->get_max_distance(), 12.f));
    if (math::is_zero(math::length(m_config->get_direction()), math::epsilon<float>()))
    {
        m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down
    }
    m_config->set_direction(math::normalized(m_config->get_direction()));

    return ts::success;
}
auto SRF02::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto SRF02::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto SRF02::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
