#include "BrainStdAfx.h"
#include "MaxSonar.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "uav.def.h"
//#include "sz_MaxSonar.hpp"

namespace silk
{
namespace node
{


MaxSonar::MaxSonar(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::MaxSonar_Descriptor())
    , m_config(new uav::MaxSonar_Config())
{
    m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down

    m_output_stream = std::make_shared<Output_Stream>();
}

auto MaxSonar::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Distance";
    outputs[0].stream = m_output_stream;
    return outputs;
}
auto MaxSonar::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("MaxSonar::init");

    auto specialized = std::dynamic_pointer_cast<uav::MaxSonar_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

auto MaxSonar::init() -> bool
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

    m_output_stream->set_rate(m_descriptor->get_rate());

    return true;
}

auto MaxSonar::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);

    return true;
}

void MaxSonar::process()
{
    QLOG_TOPIC("MaxSonar::process");

    m_output_stream->clear();

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

    //accumulate data from the serial port
    std::array<uint8_t, 32> buf;
    size_t count = bus->read(buf.data(), buf.size());
    if (count > 0)
    {
        std::copy(buf.begin(), buf.begin() + count, std::back_inserter(m_read_data));
    }

    //ignore bad data
    while (!m_read_data.empty() && m_read_data.front() != 'R')
    {
        m_read_data.pop_front();
    }

    auto now = q::Clock::now();

    //parse the entire buffer to get the latest data
    while (m_read_data.size() >= 5) //R + XXX + New Line
    {
        char const* str = reinterpret_cast<char const*>(&m_read_data[1]);
        m_read_data[4] = 0; //replace newline with zero so we can convert this to a string

        m_last_distance = static_cast<float>(atoi(str)) / 100.f;

        m_last_reading_tp = now;
        m_read_data.erase(m_read_data.begin(), m_read_data.begin() + 5);
    }

    //use the latest data only
    {
        constexpr size_t k_max_sample_difference = 5;

        auto samples_needed = m_output_stream->compute_samples_needed();

        float min_distance = m_config->get_min_distance();
        float max_distance = m_config->get_max_distance();

        bool is_healthy = m_last_distance >= min_distance && m_last_distance <= max_distance &&
                        q::Clock::now() - m_last_reading_tp <= m_output_stream->get_dt() * k_max_sample_difference;
        if (!is_healthy)
        {
            m_stats.added += samples_needed;
        }

        math::vec3f value = m_config->get_direction() * math::clamp(m_last_distance, min_distance, max_distance);

        while (samples_needed > 0)
        {
            m_output_stream->push_sample(value, is_healthy);
            samples_needed--;
        }
    }

    if (m_stats.last_report_tp + std::chrono::seconds(1) < now)
    {
        if (m_stats != Stats())
        {
            QLOGW("Stats: a{}", m_stats.added);
        }
        m_stats = Stats();
        m_stats.last_report_tp = now;
    }
}

auto MaxSonar::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("MaxSonar::set_config");

    auto specialized = std::dynamic_pointer_cast<uav::MaxSonar_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    if (math::is_zero(math::length(m_config->get_direction()), math::epsilon<float>()))
    {
        m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down
    }
    m_config->set_direction(math::normalized(m_config->get_direction()));

    return true;
}
auto MaxSonar::get_config() const -> std::shared_ptr<uav::INode_Config>
{
    return m_config;
}

auto MaxSonar::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto MaxSonar::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
