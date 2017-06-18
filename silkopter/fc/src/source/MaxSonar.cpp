#include "FCStdAfx.h"
#include "MaxSonar.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "hal.def.h"
//#include "sz_MaxSonar.hpp"

namespace silk
{
namespace node
{


MaxSonar::MaxSonar(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::MaxSonar_Descriptor())
    , m_config(new hal::MaxSonar_Config())
{
    m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down

    m_output_stream = std::make_shared<Output_Stream>();
}

auto MaxSonar::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "distance";
    outputs[0].stream = m_output_stream;
    return outputs;
}
ts::Result<void> MaxSonar::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("MaxSonar::init");

    auto specialized = dynamic_cast<hal::MaxSonar_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> MaxSonar::init()
{
    m_uart_bus = m_hal.get_bus_registry().find_by_name<bus::IUART_Bus>(m_descriptor->get_bus());
    if (!m_uart_bus.lock())
    {
        return make_error("No bus configured");
    }

    m_output_stream->set_rate(m_descriptor->get_rate());

    return ts::success;
}

ts::Result<void> MaxSonar::start(Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

void MaxSonar::process()
{
    QLOG_TOPIC("MaxSonar::process");

    m_output_stream->clear();

    auto uart_bus = m_uart_bus.lock();
    if (!uart_bus)
    {
        return;
    }

    util::hw::IUART& uart = uart_bus->get_uart();

    //accumulate data from the serial port
    std::array<uint8_t, 32> buf;
    size_t count = uart.read(buf.data(), buf.size());
    if (count > 0)
    {
        std::copy(buf.begin(), buf.begin() + count, std::back_inserter(m_read_data));
    }

    //ignore bad data
    while (!m_read_data.empty() && m_read_data.front() != 'R')
    {
        m_read_data.pop_front();
    }

    auto now = Clock::now();

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
        constexpr size_t MAX_SKIPPED_SAMPLES = 5;

        auto samples_needed = m_output_stream->compute_samples_needed();

        float min_distance = m_config->get_min_distance();
        float max_distance = m_config->get_max_distance();

        bool is_healthy = m_last_distance >= min_distance && m_last_distance <= max_distance &&
                        Clock::now() - m_last_reading_tp <= m_output_stream->get_dt() * MAX_SKIPPED_SAMPLES;
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

ts::Result<void> MaxSonar::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("MaxSonar::set_config");

    auto specialized = dynamic_cast<hal::MaxSonar_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    if (math::is_zero(math::length(m_config->get_direction()), math::epsilon<float>()))
    {
        m_config->set_direction(math::vec3f(0, 0, -1)); //pointing down
    }
    m_config->set_direction(math::normalized(m_config->get_direction()));

    return ts::success;
}
auto MaxSonar::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto MaxSonar::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> MaxSonar::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
