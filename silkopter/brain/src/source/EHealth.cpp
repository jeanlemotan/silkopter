#include "BrainStdAfx.h"
#include "EHealth.h"

/*
//#include "sz_math.hpp"

namespace silk
{
namespace node
{

EHealth::EHealth(UAV& uav)
    : m_uav(uav)
{
    m_stream = std::make_shared<Stream>();
}

EHealth::~EHealth()
{
}

auto EHealth::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "EKG";
    outputs[0].stream = m_stream;
    return outputs;
}

auto EHealth::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("EHealth::init");

    return init();
}
auto EHealth::init() -> bool
{
    m_uart = m_uav.get_buses().find_by_name<bus::IUART>("uart0");

    Buses buses = { m_uart.lock() };
    if (!buses.uart)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_stream->rate = 100;

    return true;
}

auto EHealth::start(q::Clock::time_point tp) -> bool
{
    m_last_tp = tp;
    return true;
}

void EHealth::process()
{
    QLOG_TOPIC("EHealth::process");

    m_stream->samples.clear();

    Buses buses = { m_uart.lock() };
    if (!buses.uart)
    {
        return;
    }

    do
    {
        auto res = buses.uart->read(reinterpret_cast<uint8_t*>(m_temp_buffer.data()), m_temp_buffer.size());
        if (res > 0)
        {
            std::copy(m_temp_buffer.begin(), m_temp_buffer.begin() + res, std::back_inserter(m_buffer));
        }
        else
        {
            break;
        }
    } while (1);

    const auto m_dt = std::chrono::milliseconds(10);

    auto now = q::Clock::now();
    auto tp = m_last_tp + m_dt;
    m_last_tp = now;

    while (!m_buffer.empty())
    {
        auto start = m_buffer.find_first_not_of("\n\r ");
        if (start == std::string::npos)
        {
            break;
        }
        auto end = m_buffer.find_first_of("\n\r ", start);
        if (end == std::string::npos)
        {
            break;
        }

        Stream::Sample s;
        auto str = m_buffer.substr(start, end - start);
        if (str.size() != 3)
        {
            int a = 0;
        }
        s.value.x = atof(str.c_str());
        if (s.value.x > 10)
        {
            int a = 0;
        }

        m_stream->samples.push_back(s);

        tp += m_dt;
        m_buffer.erase(m_buffer.begin(), m_buffer.begin() + end);
    }
}

auto EHealth::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("EHealth::set_config");

    return true;
}
auto EHealth::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto EHealth::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_desc;
}
auto EHealth::send_message(rapidjson::Value const& json) -> rapidjson::Document
{
    return rapidjson::Document();
}



}
}
*/
