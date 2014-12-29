#include "BrainStdAfx.h"
#include "MS5611.h"

#ifdef RASPBERRY_PI

using namespace silk;

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

auto MS5611::init(const std::string& device) -> bool
{
    SILK_INFO("initializing device: {}", device);

    if (!m_i2c.open(device))
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(120));

    m_i2c.write(ADDR_MS5611, CMD_MS5611_RESET);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // We read the factory calibration
    // The on-chip CRC is not used
    uint16_t C1, C2, C3, C4, C5, C6;
    auto res = m_i2c.read_u16(ADDR_MS5611, CMD_MS5611_PROM_C1, C1);
    res &= m_i2c.read_u16(ADDR_MS5611, CMD_MS5611_PROM_C2, C2);
    res &= m_i2c.read_u16(ADDR_MS5611, CMD_MS5611_PROM_C3, C3);
    res &= m_i2c.read_u16(ADDR_MS5611, CMD_MS5611_PROM_C4, C4);
    res &= m_i2c.read_u16(ADDR_MS5611, CMD_MS5611_PROM_C5, C5);
    res &= m_i2c.read_u16(ADDR_MS5611, CMD_MS5611_PROM_C6, C6);
    if (!res)
    {
        SILK_ERR("MS5611 not found!");
        return false;
    }
    SILK_INFO("PROM: {} {} {} {} {} {}", C1, C2, C3, C4, C5, C6);

    m_c1 = C1;
    m_c2 = C2;
    m_c3 = C3;
    m_c4 = C4;
    m_c5 = C5;
    m_c6 = C6;
    if (m_c1 == 0 || m_c2 == 0 || m_c3 == 0 || m_c4 == 0 || m_c5 == 0 || m_c6 == 0)
    {
        SILK_ERR("MS5611 seems broken!");
        return false;
    }

    m_i2c.write(ADDR_MS5611, CMD_CONVERT_D2_OSR256);

//    while(true)
//    {
//        process();
//    }

//    exit(1);

    return true;
}

void MS5611::process()
{
    auto now = q::Clock::now();
    if (now - m_last_timestamp < std::chrono::milliseconds(10))
    {
        m_sample_time = std::chrono::seconds(0);
        return;
    }

    m_sample_time = now - m_last_timestamp;
    m_last_timestamp = now;

    constexpr size_t PRESSURE_TO_TEMPERATURE_RATIO = 10;

    std::array<uint8_t, 3> buf;

    if (m_stage == 0)
    {
        //read temperature

        if (m_i2c.read(ADDR_MS5611, 0x00, buf.data(), buf.size()))
        {
            double val = (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];
            m_temperature_reading = val;

        }

        //next
        if (m_i2c.write(ADDR_MS5611, CMD_CONVERT_D1_OSR256)) //read pressure next
        {
            m_stage++;
        }
    }
    else
    {
        //read pressure

        if (m_i2c.read(ADDR_MS5611, 0x00, buf.data(), buf.size()))
        {
            double val = (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];
            m_pressure_reading = val;
        }

        //next
        if (m_stage >= PRESSURE_TO_TEMPERATURE_RATIO)
        {
            if (m_i2c.write(ADDR_MS5611, CMD_CONVERT_D2_OSR256)) //read temp next
            {
                m_stage = 0;
            }
        }
        else if (m_i2c.write(ADDR_MS5611, CMD_CONVERT_D1_OSR256)) //read pressure next
        {
            m_stage++;
        }
    }

    calculate();
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

void MS5611::calculate()
{
    // Formulas from manufacturer datasheet
    // sub -20c temperature compensation is not included

    double dT = m_temperature_reading - m_c5 * 256.0;
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

    m_temperature = static_cast<float>(TEMP) * 0.01;
    //m_pressure = (m_pressure_data*SENS/2097152.f - OFF)/32768.f;
    m_pressure = static_cast<float>((m_pressure_reading*SENS*0.000000476837158203125 - OFF)*0.000030517578125 * 0.01);

//    static Butterworth xxx;
//    m_pressure = xxx.process(m_pressure.get());
//    double alt = (1.0 - math::pow(m_pressure.get() / 1013.25, 0.190284)) * 4430769.396f;
//    SILK_INFO("{} / {}, cm: {}", m_temperature, m_pressure, alt);

    //SILK_INFO("pressure: {}, temp: {}", m_pressure, m_temperature);
}

boost::optional<float> MS5611::read_barometer()
{
    auto r = m_pressure;
    m_pressure.reset();
    return r;
}

boost::optional<float> MS5611::read_thermometer()
{
    auto r = m_temperature;
    m_temperature.reset();
    return r;
}

auto MS5611::get_barometer_sample_time() const -> q::Clock::duration
{
    return m_sample_time;
}
auto MS5611::get_thermometer_sample_time() const -> q::Clock::duration
{
    return m_sample_time;
}


#endif
