#include "BrainStdAfx.h"
#include "SRF01.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "uav.def.h"

namespace silk
{
namespace node
{


constexpr uint8_t ADDR = 0x01;

constexpr uint8_t SW_REV                = 0x5D;
constexpr uint8_t REAL_RANGING_CM_TX    = 0x54;
constexpr uint8_t SET_ADVANCED_MODE     = 0x62;
constexpr uint8_t CLEAR_ADVANCED_MODE   = 0x63;



constexpr std::chrono::milliseconds MAX_MEASUREMENT_DURATION(75);


SRF01::SRF01(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::SRF01_Descriptor())
    , m_config(new uav::SRF01_Config())
{
    m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down

    m_output_stream = std::make_shared<Output_Stream>();
}

auto SRF01::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Distance";
    outputs[0].stream = m_output_stream;
    return outputs;
}
auto SRF01::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("SRF01::init");

    auto specialized = std::dynamic_pointer_cast<uav::SRF01_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

auto SRF01::init() -> bool
{
    m_bus = m_uav.get_buses().find_by_name<bus::IUART>(m_descriptor->get_bus());
    auto bus = m_bus.lock();
    if (!bus)
    {
        QLOGE("No bus configured");
        return false;
    }

    bus->lock();
    At_Exit at_exit([this, &bus]()
    {
        bus->unlock();
    });

    QLOGI("Probing SRF01 on {}...", m_descriptor->get_bus());

    uint32_t tries = 0;
    constexpr uint32_t max_tries = 10;
    while (++tries <= max_tries)
    {
        bool ok = send_command(*bus, SW_REV);

        uint8_t rev = 0;
        ok &= read_response_u8(*bus, SW_REV, rev);
        if (ok && rev != 0 && rev != 255)
        {
            QLOGI("Found SRF01 rev {} after {} tries", rev, tries);
            break;
        }
        QLOGW("\tFailed {} try to initialize SRF01: bus {}, rev {}", tries, ok, rev);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (tries > max_tries)
    {
        QLOGE("Failed to initialize SRF01");
        return false;
    }

    //clear advanced mode
    //send_command(*bus, CLEAR_ADVANCED_MODE);
    send_command(*bus, SET_ADVANCED_MODE);

    trigger(*bus);

    m_output_stream->set_rate(m_descriptor->get_rate());

    return true;
}

auto SRF01::start(q::Clock::time_point tp) -> bool
{
    m_last_trigger_tp = tp;
    m_output_stream->set_tp(tp);
    return true;
}

auto SRF01::send_command(bus::IUART& bus, uint8_t command) -> bool
{
    std::array<uint8_t, 2> buf;
    buf[0] = ADDR;											//SRF01 address
    buf[1] = command;

    bus.send_break();
    return bus.write(buf.data(), buf.size());
}

auto SRF01::read_response(bus::IUART& bus, uint8_t sent_command, uint8_t* response, size_t size) -> bool
{
    QASSERT(response && size > 0);
    std::array<uint8_t, 32> buf;
    size_t count = bus.read(buf.data(), buf.size());
    if (count > 0)
    {
        std::copy(buf.begin(), buf.begin() + count, std::back_inserter(m_read_data));

        //consume the echo of what we sent
        while (m_read_data.size() > 2 && (m_read_data[0] != ADDR || m_read_data[1] != sent_command))
        {
            m_read_data.pop_front();
        }

        if (m_read_data.size() >= 2 + size && m_read_data[0] == ADDR && m_read_data[1] == sent_command)
        {
            std::copy(m_read_data.begin() + 2, m_read_data.begin() + 2 + size, response);
            m_read_data.clear();
            return true;
        }
    }

    return false;
}
auto SRF01::read_response_u8(bus::IUART& bus, uint8_t sent_command, uint8_t& response) -> bool
{
    return read_response(bus, sent_command, &response, 1);
}
auto SRF01::read_response_u16(bus::IUART& bus, uint8_t sent_command, uint16_t& response) -> bool
{
    uint8_t res[2];
    if (read_response(bus, sent_command, res, 2))
    {
        response = (uint16_t)(res[0] << 8) | res[1];
        return true;
    }
    return false;
}


void SRF01::trigger(bus::IUART& bus)
{
    m_last_trigger_tp = q::Clock::now();
    send_command(bus, REAL_RANGING_CM_TX);
}

void SRF01::process()
{
    QLOG_TOPIC("SRF01::process");

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

    //TODO - add health indication


    //the trigger data acts as a header. Following this header there is the actual measurement
    uint16_t d = 0;
    if (read_response_u16(*bus, REAL_RANGING_CM_TX, d))
    {
        QLOGI("d = {}", d);

        float distance = static_cast<float>(d) / 100.f; //meters

        float min_distance = m_config->get_min_distance();//math::max(m_config->min_distance, static_cast<float>(min_d) / 100.f); //meters
        float max_distance = m_config->get_max_distance();
        auto value = m_config->get_direction() * math::clamp(distance, min_distance, max_distance);
        auto is_healthy = distance >= min_distance && distance <= max_distance;

        m_output_stream->clear();
        auto samples_needed = m_output_stream->compute_samples_needed();
        while (samples_needed > 0)
        {
            m_output_stream->push_sample(value, is_healthy);
            samples_needed--;
        }

        trigger(*bus);
    }
    else
    {
        if (now - m_last_trigger_tp >= std::chrono::milliseconds(500))
        {
            QLOGI("Timeout. triggering again");
            trigger(*bus);
        }
    }
}

auto SRF01::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("SRF01::set_config");

    auto specialized = std::dynamic_pointer_cast<uav::SRF01_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

//    m_config->min_distance = math::max(m_config->min_distance, 0.1f);
//    m_config->max_distance = math::min(m_config->max_distance, 12.f);
    if (math::is_zero(math::length(m_config->get_direction()), math::epsilon<float>()))
    {
        m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down
    }
    m_config->set_direction(math::normalized(m_config->get_direction()));

    return true;
}
auto SRF01::get_config() const -> std::shared_ptr<uav::INode_Config>
{
    return m_config;
}

auto SRF01::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto SRF01::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
