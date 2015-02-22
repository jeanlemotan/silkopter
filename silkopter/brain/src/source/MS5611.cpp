#include "BrainStdAfx.h"
#include "MS5611.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace source
{


//registers
constexpr uint8_t CMD_MS5611_RESET = 0x1E;
constexpr uint8_t CMD_MS5611_PROM_Setup = 0xA0;
constexpr uint8_t CMD_MS5611_PROM_C1 = 0xA2;
constexpr uint8_t CMD_MS5611_PROM_C2 = 0xA4;
constexpr uint8_t CMD_MS5611_PROM_C3 = 0xA6;
constexpr uint8_t CMD_MS5611_PROM_C4 = 0xA8;
constexpr uint8_t CMD_MS5611_PROM_C5 = 0xAA;
constexpr uint8_t CMD_MS5611_PROM_C6 = 0xAC;
constexpr uint8_t CMD_MS5611_PROM_CRC = 0xAE;

constexpr uint8_t CMD_CONVERT_D1_OSR256 = 0x40;
constexpr uint8_t CMD_CONVERT_D1_OSR512 = 0x42;
constexpr uint8_t CMD_CONVERT_D1_OSR1024 = 0x44;
constexpr uint8_t CMD_CONVERT_D1_OSR2048 = 0x46;
constexpr uint8_t CMD_CONVERT_D1_OSR4096 = 0x48;

constexpr uint8_t CMD_CONVERT_D2_OSR256 = 0x50;
constexpr uint8_t CMD_CONVERT_D2_OSR512 = 0x52;
constexpr uint8_t CMD_CONVERT_D2_OSR1024 = 0x54;
constexpr uint8_t CMD_CONVERT_D2_OSR2048 = 0x56;
constexpr uint8_t CMD_CONVERT_D2_OSR4096 = 0x58;

constexpr uint8_t ADDR_MS5611 = 0x77;

MS5611::MS5611(HAL& hal)
    : m_hal(hal)
{
}

void MS5611::lock()
{
    if (m_i2c)
    {
        m_i2c->lock();
    }
    else
    {
        m_spi->lock();
    }
}
void MS5611::unlock()
{
    if (m_i2c)
    {
        m_i2c->unlock();
    }
    else
    {
        m_spi->unlock();
    }
}

auto MS5611::bus_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool
{
    return m_i2c ? m_i2c->read_register(ADDR_MS5611, reg, data, size) : m_spi->read_register(reg, data, size);
}
auto MS5611::bus_read_u8(uint8_t reg, uint8_t& dst) -> bool
{
    return m_i2c ? m_i2c->read_register_u8(ADDR_MS5611, reg, dst) : m_spi->read_register_u8(reg, dst);
}
auto MS5611::bus_read_u16(uint8_t reg, uint16_t& dst) -> bool
{
    return m_i2c ? m_i2c->read_register_u16(ADDR_MS5611, reg, dst) : m_spi->read_register_u16(reg, dst);
}
auto MS5611::bus_write(uint8_t reg, uint8_t const* data, uint32_t size) -> bool
{
    return m_i2c ? m_i2c->write_register(ADDR_MS5611, reg, data, size) : m_spi->write_register(reg, data, size);
}
auto MS5611::bus_write_u8(uint8_t reg, uint8_t const& t) -> bool
{
    return m_i2c ? m_i2c->write_register_u8(ADDR_MS5611, reg, t) : m_spi->write_register_u8(reg, t);
}
auto MS5611::bus_write_u16(uint8_t reg, uint16_t const& t) -> bool
{
    return m_i2c ? m_i2c->write_register_u16(ADDR_MS5611, reg, t) : m_spi->write_register_u16(reg, t);
}

auto MS5611::get_name() const -> std::string const&
{
    return m_params.name;
}
auto MS5611::get_output_stream_count() const -> size_t
{
    return 2;
}
auto MS5611::get_output_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_output_stream_count());
    std::array<stream::IStream*, 2> streams =
    {{
        &m_pressure, &m_temperature
    }};
    QASSERT(streams.size() == get_output_stream_count());
    return *streams[idx];
}

auto MS5611::init(rapidjson::Value const& json) -> bool
{
    sz::MS5611 sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize MS5611 data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.bus = m_hal.get_buses().find_by_name<bus::IBus>(sz.bus);
    params.rate = sz.rate;
    params.temperature_rate_ratio = sz.temperature_rate_ratio;
    return init(params);
}
auto MS5611::init(Init_Params const& params) -> bool
{
    QLOG_TOPIC("ms5611::init");

    m_params = params;

    m_i2c = dynamic_cast<bus::II2C*>(params.bus);
    m_spi = dynamic_cast<bus::ISPI*>(params.bus);
    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        m_pressure.name = q::util::format2<std::string>("{}-pressure", params.name);
        m_temperature.name = q::util::format2<std::string>("{}-temperature", params.name);

        if (!m_hal.get_sources().add(*this) ||
            !m_hal.get_streams().add(m_pressure) ||
            !m_hal.get_streams().add(m_temperature))
        {
            return false;
        }
    }

    return true;
}
auto MS5611::init() -> bool
{
    if (!m_i2c && !m_spi)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_params.rate = math::clamp<size_t>(m_params.rate, 10, 100);
    m_params.temperature_rate_ratio = math::clamp<size_t>(m_params.temperature_rate_ratio, 1, 10);
    m_pressure.rate = m_params.rate;
    m_temperature.rate = m_params.rate / m_params.temperature_rate_ratio;

    m_dt = std::chrono::milliseconds(1000 / m_params.rate);

    QLOGI("Probing MS5611 on {}", m_params.bus->get_name());

    boost::this_thread::sleep_for(boost::chrono::milliseconds(120));

    bus_write(CMD_MS5611_RESET, nullptr, 0);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // We read the factory calibration
    // The on-chip CRC is not used
    uint16_t C1, C2, C3, C4, C5, C6;
    auto res = bus_read_u16(CMD_MS5611_PROM_C1, C1);
    res &= bus_read_u16(CMD_MS5611_PROM_C2, C2);
    res &= bus_read_u16(CMD_MS5611_PROM_C3, C3);
    res &= bus_read_u16(CMD_MS5611_PROM_C4, C4);
    res &= bus_read_u16(CMD_MS5611_PROM_C5, C5);
    res &= bus_read_u16(CMD_MS5611_PROM_C6, C6);
    if (!res)
    {
        QLOGE("MS5611 not found!");
        return false;
    }
    QLOGI("PROM: {} {} {} {} {} {}", C1, C2, C3, C4, C5, C6);

    m_c1 = C1;
    m_c2 = C2;
    m_c3 = C3;
    m_c4 = C4;
    m_c5 = C5;
    m_c6 = C6;
    if (m_c1 == 0 || m_c2 == 0 || m_c3 == 0 || m_c4 == 0 || m_c5 == 0 || m_c6 == 0)
    {
        QLOGE("MS5611 seems broken!");
        return false;
    }

    bus_write(CMD_CONVERT_D2_OSR256, nullptr, 0);

    return true;
}

void MS5611::process()
{
    m_pressure.samples.clear();
    m_temperature.samples.clear();

    QLOG_TOPIC("ms5611::process");
    auto now = q::Clock::now();
    if (now - m_last_timestamp < m_dt)
    {
        return;
    }

    auto dt = now - m_last_timestamp;
    m_last_timestamp = now;

    std::array<uint8_t, 3> buf;
    if (m_stage == 0)
    {
        //read temperature
        if (bus_read(0x00, buf.data(), buf.size()))
        {
            double val = (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];
            m_temperature.reading = val;

        }

        //next
        if (bus_write(CMD_CONVERT_D1_OSR256, nullptr, 0)) //read pressure next
        {
            m_stage++;
        }
    }
    else
    {
        //read pressure
        if (bus_read(0x00, buf.data(), buf.size()))
        {
            double val = (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];
            m_pressure.reading = val;
        }

        //next
        if (m_stage >= m_params.temperature_rate_ratio)
        {
            if (bus_write(CMD_CONVERT_D2_OSR256, nullptr, 0)) //read temp next
            {
                m_stage = 0;
            }
        }
        else if (bus_write(CMD_CONVERT_D1_OSR256, nullptr, 0)) //read pressure next
        {
            m_stage++;
        }
    }

    calculate(dt);
}

class Butterworth //10hz
{
public:
    static constexpr size_t NZEROS  = 2;
    static constexpr size_t NPOLES  = 2;
    static constexpr float GAIN    = 1.058546241e+03;
    float xv[NZEROS+1], yv[NPOLES+1];
    float process(float t)
    {
        xv[0] = xv[1]; xv[1] = xv[2];
        xv[2] = t / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2];
        yv[2] =   (xv[0] + xv[2]) + 2 * xv[1]
                   + ( -0.9149758348f * yv[0]) + (  1.9111970674f * yv[1]);
        return yv[2];
    }
};

void MS5611::calculate(q::Clock::duration dt)
{
    // Formulas from manufacturer datasheet
    // sub -20c temperature compensation is not included

    double dT = m_temperature.reading - m_c5 * 256.0;
    double TEMP = 2000.0 + (dT * m_c6)*0.00000011920928955078125;
    double OFF = m_c2 * 65536.0 + (m_c4 * dT) * 0.0078125;
    double SENS = m_c1 * 32768.0 + (m_c3 * dT) * 0.00390625;

    if (TEMP < 2000.0)
    {
        // second order temperature compensation when under 20 degrees C
        double T2 = (dT*dT) / 0x80000000;
        double Aux = TEMP*TEMP;
        double OFF2 = 2.5*Aux;
        double SENS2 = 1.25*Aux;
        TEMP = TEMP - T2;
        OFF = OFF - OFF2;
        SENS = SENS - SENS2;
    }

    auto t = static_cast<float>(TEMP) * 0.01;
    auto p = static_cast<float>((m_pressure.reading*SENS*0.000000476837158203125 - OFF)*0.000030517578125 * 0.01);

    Temperature::Sample& ts = m_temperature.last_sample;
    ts.value = t;
    ts.sample_idx++;
    ts.dt = dt;
    m_temperature.samples.push_back(ts);
    //m_pressure = (m_pressure_data*SENS/2097152.f - OFF)/32768.f;

    Pressure::Sample& ps = m_pressure.last_sample;
    ps.value = p;
    ps.sample_idx++;
    ps.dt = dt;
    m_pressure.samples.push_back(ps);

//    static Butterworth xxx;
//    m_pressure = xxx.process(m_pressure.get());
//    double alt = (1.0 - math::pow(m_pressure.get() / 1013.25, 0.190284)) * 4430769.396f;
//    LOG_INFO("{} / {}, cm: {}", m_temperature, m_pressure, alt);

    //LOG_INFO("pressure: {}, temp: {}", m_pressure, m_temperature);
}

auto MS5611::set_config(rapidjson::Value const& json) -> bool
{
    return false;
}
auto MS5611::get_config() -> boost::optional<rapidjson::Value const&>
{
    return boost::none;
}

}
}
}

