#include "BrainStdAfx.h"
#include "MS5611.h"

#include "hal.def.h"
//#include "sz_MS5611.hpp"

namespace silk
{
namespace node
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

constexpr Clock::duration MIN_CONVERSION_TIME = std::chrono::milliseconds(10);

constexpr uint8_t ADDR_MS5611 = 0x77;


MS5611::MS5611(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::MS5611_Descriptor())
    , m_config(new hal::MS5611_Config())
{
    m_pressure = std::make_shared<Pressure_Stream>();
    m_temperature = std::make_shared<Temperature_Stream>();
}

auto MS5611::lock(Buses& buses) -> bool
{
    if (buses.i2c)
    {
        buses.i2c->lock(); //lock the bus
        return true;
    }
    if (buses.spi)
    {
        buses.spi->lock(); //lock the bus
        return true;
    }
    return false;
}
void MS5611::unlock(Buses& buses)
{
    if (buses.i2c)
    {
        buses.i2c->unlock(); //unlock the bus
        return;
    }
    if (buses.spi)
    {
        buses.spi->unlock(); //unlock the bus
        return;
    }
}

auto MS5611::bus_read_u24(Buses& buses, uint8_t reg, uint32_t& dst) -> bool
{
    uint8_t tx_data[3] = {0};
    uint8_t rx_data[3];
    bool res = buses.i2c ? buses.i2c->read_register(m_descriptor->get_i2c_address(), reg, rx_data, 3)
                : buses.spi ? buses.spi->transfer_register(reg, tx_data, rx_data, 3)
                : false;
    if (res)
    {
        dst = (((uint32_t)rx_data[0]) << 16) | (((uint32_t)rx_data[1]) << 8) | rx_data[2];
    }
    return res;
}
auto MS5611::bus_read_u8(Buses& buses, uint8_t reg, uint8_t& rx_data) -> bool
{
    uint8_t dummy_data = 0;
    return buses.i2c ? buses.i2c->read_register_u8(m_descriptor->get_i2c_address(), reg, rx_data)
         : buses.spi ? buses.spi->transfer_register_u8(reg, dummy_data, rx_data)
         : false;
}
auto MS5611::bus_read_u16(Buses& buses, uint8_t reg, uint16_t& rx_data) -> bool
{
    uint16_t dummy_data = 0;
    return buses.i2c ? buses.i2c->read_register_u16(m_descriptor->get_i2c_address(), reg, rx_data)
         : buses.spi ? buses.spi->transfer_register_u16(reg, dummy_data, rx_data)
         : false;
}
auto MS5611::bus_write(Buses& buses, uint8_t data) -> bool
{
    uint8_t dummy_data;
    return buses.i2c ? buses.i2c->write(m_descriptor->get_i2c_address(), &data, 1)
         : buses.spi ? buses.spi->transfer(&data, &dummy_data, 1)
         : false;
}

auto MS5611::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(2);
    outputs[0].name = "pressure";
    outputs[0].stream = m_pressure;
    outputs[1].name = "temperature";
    outputs[1].stream = m_temperature;
    return outputs;
}
ts::Result<void> MS5611::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("ms5611::init");

    auto specialized = dynamic_cast<hal::MS5611_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> MS5611::init()
{
    m_i2c = m_hal.get_bus_registry().find_by_name<bus::II2C>(m_descriptor->get_bus());
    m_spi = m_hal.get_bus_registry().find_by_name<bus::ISPI>(m_descriptor->get_bus());

    Buses buses = { m_i2c.lock(), m_spi.lock() };
    if (!buses.i2c && !buses.spi)
    {
        return make_error("No bus configured");
    }

    lock(buses);
    At_Exit at_exit([this, &buses]()
    {
        unlock(buses);
    });

//    m_descriptor->pressure_rate = math::clamp<size_t>(m_descriptor->pressure_rate, 10, 100);
//    m_descriptor->temperature_rate_ratio = math::clamp<size_t>(m_descriptor->temperature_rate_ratio, 1, 10);

    QLOGI("Probing MS5611 on {}", m_descriptor->get_bus());

    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    bool res = bus_write(buses, CMD_MS5611_RESET);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // We read the factory calibration
    // The on-chip CRC is not used
    uint16_t C1, C2, C3, C4, C5, C6;
    res &= bus_read_u16(buses, CMD_MS5611_PROM_C1, C1);
    res &= bus_read_u16(buses, CMD_MS5611_PROM_C2, C2);
    res &= bus_read_u16(buses, CMD_MS5611_PROM_C3, C3);
    res &= bus_read_u16(buses, CMD_MS5611_PROM_C4, C4);
    res &= bus_read_u16(buses, CMD_MS5611_PROM_C5, C5);
    res &= bus_read_u16(buses, CMD_MS5611_PROM_C6, C6);
    if (!res)
    {
        return make_error("MS5611 not found!");
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
        return make_error("MS5611 seems broken!");
    }

    m_stage = Stage::PRESSURE;
    res = bus_write(buses, CMD_CONVERT_D1_OSR256);
    if (!res)
    {
        return make_error("cannot start conversion");
    }

    m_pressure->set_rate(m_descriptor->get_pressure_rate());
    m_temperature->set_rate(m_descriptor->get_temperature_rate());

    return ts::success;
}

ts::Result<void> MS5611::start(Clock::time_point tp)
{
    m_last_temperature_reading_tp = tp;
    m_last_pressure_reading_tp = tp;
    m_pressure->set_tp(tp);
    m_temperature->set_tp(tp);
    return ts::success;
}

void MS5611::process()
{
    m_pressure->clear();
    m_temperature->clear();

    Buses buses = { m_i2c.lock(), m_spi.lock() };
    if (!buses.i2c && !buses.spi)
    {
        return;
    }

    lock(buses);
    At_Exit at_exit([this, &buses]()
    {
        unlock(buses);
    });

    QLOG_TOPIC("ms5611::process");
    auto now = Clock::now();
    if (now - m_last_process_tp < MIN_CONVERSION_TIME)
    {
        return;
    }
    m_last_process_tp = now;

    if (m_stage == Stage::PRESSURE)
    {
        uint32_t data = 0;
        //read pressure
        if (bus_read_u24(buses, 0x00, data))
        {
            m_last_pressure_reading_tp = now;
            m_pressure->reading = static_cast<double>(data);
        }
        else
        {
            m_stats.bus_failures++;
        }
    }
    else if (m_stage == Stage::TEMPERATURE)
    {
        uint32_t data = 0;
        //read temperature
        if (bus_read_u24(buses, 0x00, data))
        {
            m_last_temperature_reading_tp = now;
            m_temperature->reading = static_cast<double>(data);
        }
        else
        {
            m_stats.bus_failures++;
        }
    }

    //figure out what to read next
    auto future_tp_p = m_last_pressure_reading_tp + m_pressure->get_dt();
    auto future_tp_t = m_last_temperature_reading_tp + m_temperature->get_dt();
    if (future_tp_p < future_tp_t)
    {
        if (bus_write(buses, CMD_CONVERT_D1_OSR256)) //read pressure next
        {
            m_stage = Stage::PRESSURE;
        }
        else
        {
            m_stage = Stage::UNKNOWN;
            m_stats.bus_failures++;
        }
    }
    else
    {
        if (bus_write(buses, CMD_CONVERT_D2_OSR256)) //read temp next
        {
            m_stage = Stage::TEMPERATURE;
        }
        else
        {
            m_stage = Stage::UNKNOWN;
            m_stats.bus_failures++;
        }
    }

    {
        // Formulas from manufacturer datasheet
        // sub -20c temperature compensation is not included

        double dT = m_temperature->reading - m_c5 * 256.0;
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

        float t = static_cast<float>(TEMP) * 0.01f;
        double p = (m_pressure->reading*SENS*0.000000476837158203125 - OFF)*0.000030517578125;
        p = p * 0.001; //pascals to kilopascals

        constexpr size_t MAX_LATE_SAMPLES = 5;

        {
            size_t samples_needed = m_pressure->compute_samples_needed();
            bool is_healthy = Clock::now() - m_last_pressure_reading_tp <= m_pressure->get_dt() * MAX_LATE_SAMPLES;
            if (!is_healthy)
            {
                m_stats.pres.added += samples_needed;
            }
            while (samples_needed > 0)
            {
                m_pressure->push_sample(p, is_healthy);
                samples_needed--;
            }
        }
        {
            size_t samples_needed = m_temperature->compute_samples_needed();
            bool is_healthy = Clock::now() - m_last_temperature_reading_tp <= m_temperature->get_dt() * MAX_LATE_SAMPLES;
            if (!is_healthy)
            {
                m_stats.temp.added += samples_needed;
            }
            while (samples_needed > 0)
            {
                m_temperature->push_sample(t, is_healthy);
                samples_needed--;
            }
        }
    }

    if (m_stats.last_report_tp + std::chrono::seconds(1) < now)
    {
        if (m_stats != Stats())
        {
            QLOGW("Stats: P:a{}, T:a{}, bus{}",
                        m_stats.pres.added,
                        m_stats.temp.added,
                        m_stats.bus_failures);
        }
        m_stats = Stats();
        m_stats.last_report_tp = now;
    }
}

ts::Result<void> MS5611::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("ms5611::set_config");

    auto specialized = dynamic_cast<hal::MS5611_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto MS5611::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto MS5611::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> MS5611::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
