#include "BrainStdAfx.h"
#include "ADS1115.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "hal.def.h"
//#include "sz_ADS1115.hpp"

namespace silk
{
namespace node
{


constexpr uint8_t ADS1115_ADDRESS_ADDR_GND    = 0x48; // address pin low (GND)
constexpr uint8_t ADS1115_ADDRESS_ADDR_VDD    = 0x49; // address pin high (VCC)
constexpr uint8_t ADS1115_ADDRESS_ADDR_SDA    = 0x4A; // address pin tied to SDA pin
constexpr uint8_t ADS1115_ADDRESS_ADDR_SCL    = 0x4B; // address pin tied to SCL pin
constexpr uint8_t ADS1115_DEFAULT_ADDRESS     = ADS1115_ADDRESS_ADDR_GND;

constexpr uint8_t ADS1115_RA_CONVERSION       = 0x00;
constexpr uint8_t ADS1115_RA_CONFIG           = 0x01;
constexpr uint8_t ADS1115_RA_LO_THRESH        = 0x02;
constexpr uint8_t ADS1115_RA_HI_THRESH        = 0x03;

constexpr uint16_t ADS1115_OS_SHIFT            = 15;
constexpr uint16_t ADS1115_OS_INACTIVE         = 0x00 << ADS1115_OS_SHIFT;
constexpr uint16_t ADS1115_OS_ACTIVE           = 0x01 << ADS1115_OS_SHIFT;

constexpr uint16_t ADS1115_MUX_SHIFT           = 12;
constexpr uint16_t ADS1115_MUX_P0_N1           = 0x00 << ADS1115_MUX_SHIFT; /* default */
constexpr uint16_t ADS1115_MUX_P0_N3           = 0x01 << ADS1115_MUX_SHIFT;
constexpr uint16_t ADS1115_MUX_P1_N3           = 0x02 << ADS1115_MUX_SHIFT;
constexpr uint16_t ADS1115_MUX_P2_N3           = 0x03 << ADS1115_MUX_SHIFT;
constexpr uint16_t ADS1115_MUX_P0_NG           = 0x04 << ADS1115_MUX_SHIFT;
constexpr uint16_t ADS1115_MUX_P1_NG           = 0x05 << ADS1115_MUX_SHIFT;
constexpr uint16_t ADS1115_MUX_P2_NG           = 0x06 << ADS1115_MUX_SHIFT;
constexpr uint16_t ADS1115_MUX_P3_NG           = 0x07 << ADS1115_MUX_SHIFT;

constexpr uint16_t ADS1115_PGA_SHIFT           = 9;
constexpr uint16_t ADS1115_PGA_6144            = 0x00 << ADS1115_PGA_SHIFT;
constexpr uint16_t ADS1115_PGA_4096            = 0x01 << ADS1115_PGA_SHIFT;
constexpr uint16_t ADS1115_PGA_2048            = 0x02 << ADS1115_PGA_SHIFT; // default
constexpr uint16_t ADS1115_PGA_1024            = 0x03 << ADS1115_PGA_SHIFT;
constexpr uint16_t ADS1115_PGA_512             = 0x04 << ADS1115_PGA_SHIFT;
constexpr uint16_t ADS1115_PGA_256             = 0x05 << ADS1115_PGA_SHIFT;
constexpr uint16_t ADS1115_PGA_256B            = 0x06 << ADS1115_PGA_SHIFT;
constexpr uint16_t ADS1115_PGA_256C            = 0x07 << ADS1115_PGA_SHIFT;

constexpr float ADS1115_MV_6144                = 0.000187500f;
constexpr float ADS1115_MV_4096                = 0.000125000f;
constexpr float ADS1115_MV_2048                = 0.000062500f; // default
constexpr float ADS1115_MV_1024                = 0.000031250f;
constexpr float ADS1115_MV_512                 = 0.000015625f;
constexpr float ADS1115_MV_256                 = 0.000007813f;
constexpr float ADS1115_MV_256B                = 0.000007813f;
constexpr float ADS1115_MV_256C                = 0.000007813f;

constexpr uint16_t ADS1115_MODE_SHIFT          = 8;
constexpr uint16_t ADS1115_MODE_CONTINUOUS     = 0x00 << ADS1115_MODE_SHIFT;
constexpr uint16_t ADS1115_MODE_SINGLESHOT     = 0x01 << ADS1115_MODE_SHIFT; // default

constexpr uint16_t ADS1115_RATE_SHIFT          = 5;
constexpr uint16_t ADS1115_RATE_8              = 0x00 << ADS1115_RATE_SHIFT;
constexpr uint16_t ADS1115_RATE_16             = 0x01 << ADS1115_RATE_SHIFT;
constexpr uint16_t ADS1115_RATE_32             = 0x02 << ADS1115_RATE_SHIFT;
constexpr uint16_t ADS1115_RATE_64             = 0x03 << ADS1115_RATE_SHIFT;
constexpr uint16_t ADS1115_RATE_128            = 0x04 << ADS1115_RATE_SHIFT; // default
constexpr uint16_t ADS1115_RATE_250            = 0x05 << ADS1115_RATE_SHIFT;
constexpr uint16_t ADS1115_RATE_475            = 0x06 << ADS1115_RATE_SHIFT;
constexpr uint16_t ADS1115_RATE_860            = 0x07 << ADS1115_RATE_SHIFT;

constexpr uint16_t ADS1115_COMP_MODE_SHIFT        = 4;
constexpr uint16_t ADS1115_COMP_MODE_HYSTERESIS   = 0x00 << ADS1115_COMP_MODE_SHIFT;        // default
constexpr uint16_t ADS1115_COMP_MODE_WINDOW       = 0x01 << ADS1115_COMP_MODE_SHIFT;

constexpr uint16_t ADS1115_COMP_POL_SHIFT         = 3;
constexpr uint16_t ADS1115_COMP_POL_ACTIVE_LOW    = 0x00 << ADS1115_COMP_POL_SHIFT;     // default
constexpr uint16_t ADS1115_COMP_POL_ACTIVE_HIGH   = 0x01 << ADS1115_COMP_POL_SHIFT;

constexpr uint16_t ADS1115_COMP_LAT_SHIFT         = 2;
constexpr uint16_t ADS1115_COMP_LAT_NON_LATCHING  = 0x00 << ADS1115_COMP_LAT_SHIFT;    // default
constexpr uint16_t ADS1115_COMP_LAT_LATCHING      = 0x01 << ADS1115_COMP_LAT_SHIFT;

constexpr uint16_t ADS1115_COMP_QUE_SHIFT      = 0;
constexpr uint16_t ADS1115_COMP_QUE_ASSERT1    = 0x00 << ADS1115_COMP_QUE_SHIFT;
constexpr uint16_t ADS1115_COMP_QUE_ASSERT2    = 0x01 << ADS1115_COMP_QUE_SHIFT;
constexpr uint16_t ADS1115_COMP_QUE_ASSERT4    = 0x02 << ADS1115_COMP_QUE_SHIFT;
constexpr uint16_t ADS1115_COMP_QUE_DISABLE    = 0x03 << ADS1115_COMP_QUE_SHIFT; // default








constexpr std::chrono::microseconds MIN_CONVERSION_DURATION(1200);


ADS1115::ADS1115(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::ADS1115_Descriptor())
    , m_config(new hal::ADS1115_Config())
{
}

auto ADS1115::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_adcs.size());
    for (size_t i = 0; i < m_adcs.size(); i++)
    {
        outputs[i].name = q::util::format<std::string>("adc {}", m_adcs[i].pin_index);
        outputs[i].stream = m_adcs[i].stream;
    }
    return outputs;
}
ts::Result<void> ADS1115::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("ADS1115::init");

    auto specialized = dynamic_cast<hal::ADS1115_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> ADS1115::init()
{
    m_i2c = m_hal.get_bus_registry().find_by_name<bus::II2C>(m_descriptor->get_bus());

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return make_error("No bus configured");
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });


    if (m_descriptor->get_adc0().get_is_enabled())
    {
        ADC adc;
        adc.stream = std::make_shared<Output_Stream>();
        adc.stream->set_rate(m_descriptor->get_adc0().get_rate());
        adc.pin_index = 0;
        adc.last_tp = Clock::now();
        m_adcs.push_back(adc);
    }
    if (m_descriptor->get_adc1().get_is_enabled())
    {
        ADC adc;
        adc.stream = std::make_shared<Output_Stream>();
        adc.stream->set_rate(m_descriptor->get_adc1().get_rate());
        adc.pin_index = 1;
        adc.last_tp = Clock::now();
        m_adcs.push_back(adc);
    }
    if (m_descriptor->get_adc2().get_is_enabled())
    {
        ADC adc;
        adc.stream = std::make_shared<Output_Stream>();
        adc.stream->set_rate(m_descriptor->get_adc2().get_rate());
        adc.pin_index = 2;
        adc.last_tp = Clock::now();
        m_adcs.push_back(adc);
    }
    if (m_descriptor->get_adc3().get_is_enabled())
    {
        ADC adc;
        adc.stream = std::make_shared<Output_Stream>();
        adc.stream->set_rate(m_descriptor->get_adc3().get_rate());
        adc.pin_index = 3;
        adc.last_tp = Clock::now();
        m_adcs.push_back(adc);
    }

    m_config_register.gain = ADS1115_PGA_4096;
    m_config_register.mux = ADS1115_MUX_P0_NG;
    m_config_register.mode = ADS1115_MODE_SINGLESHOT;//ADS1115_MODE_CONTINUOUS;
    m_config_register.queue = ADS1115_COMP_QUE_DISABLE;
    m_config_register.rate = ADS1115_RATE_860;

    bool res = set_config_register(*i2c);

    uint8_t data;
    res &= i2c->read_register_u8(m_descriptor->get_i2c_address(), ADS1115_RA_CONFIG, data);

    //start first measurement
    m_crt_adc_idx = 0;
    m_config_register.status = ADS1115_OS_ACTIVE;
    res &= set_config_register(*i2c);
    if (!res)
    {
        return make_error("Cannot find ADS1115");
    }
    return ts::success;
}

auto ADS1115::set_config_register(bus::II2C& i2c) -> bool
{
    uint16_t config =   m_config_register.gain |
                        m_config_register.mux |
                        m_config_register.status |
                        m_config_register.mode |
                        m_config_register.rate |
                        m_config_register.comparator |
                        m_config_register.polarity |
                        m_config_register.latch |
                        m_config_register.queue;
    return i2c.write_register_u16(m_descriptor->get_i2c_address(), ADS1115_RA_CONFIG, config);
}

ts::Result<void> ADS1115::start(Clock::time_point tp)
{
    m_schedule_tp = tp;

    for (ADC& adc: m_adcs)
    {
        adc.stream->set_tp(tp);
    }
    return ts::success;
}


void ADS1115::process()
{
    QLOG_TOPIC("ADS1115::process");

    for (ADC& adc: m_adcs)
    {
        adc.stream->clear();
    }

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return;
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });

    uint8_t next_adc_idx = m_crt_adc_idx;
    bool schedule_reading = false;

    const size_t MAX_SKIPPED_SAMPLES = 5;

    for (ADC& adc: m_adcs)
    {
        size_t samples_needed = adc.stream->compute_samples_needed();
        auto now = Clock::now();
        if (samples_needed == 0)
        {
            continue;
        }

        float value = adc.stream->get_last_sample().value;

        //refresh the current adc pin
        if (&adc == &m_adcs[m_crt_adc_idx] && now - m_schedule_tp >= MIN_CONVERSION_DURATION)
        {
            bool good = read_sensor(*i2c, value);
            if (good)
            {
                adc.last_tp = now;

                //move to the next one
                next_adc_idx = (m_crt_adc_idx + 1) % m_adcs.size();
                schedule_reading = true;
            }
        }

        //add samples up to the sample rate
        while (samples_needed > 0)
        {
            bool is_healthy = now - adc.last_tp < adc.stream->get_dt() * MAX_SKIPPED_SAMPLES;
            adc.stream->push_sample(value, is_healthy);
            samples_needed--;
        }
    }

    //start the next reading
    if (schedule_reading)
    {
        //start measurement again
        uint16_t muxes[] = { ADS1115_MUX_P0_NG, ADS1115_MUX_P1_NG, ADS1115_MUX_P2_NG, ADS1115_MUX_P3_NG };
        m_config_register.mux = muxes[m_adcs[next_adc_idx].pin_index];
        if (set_config_register(*i2c))
        {
            m_crt_adc_idx = next_adc_idx;
            m_schedule_tp = Clock::now();
        }
    }
}

bool ADS1115::read_sensor(bus::II2C& i2c, float& o_value)
{
    if (m_config_register.mode == ADS1115_MODE_SINGLESHOT)
    {
        uint16_t cr = 0;
        if (!i2c.read_register_u16(m_descriptor->get_i2c_address(), ADS1115_RA_CONFIG, cr))
        {
            return false;
        }
        //not ready
        if ((cr & ADS1115_OS_ACTIVE) == 0)
        {
            return false;
        }
    }

    uint16_t ufvalue = 0;
    if (!i2c.read_register_u16(m_descriptor->get_i2c_address(), ADS1115_RA_CONVERSION, ufvalue))
    {
        return false;
    }
    int16_t fvalue = reinterpret_cast<int16_t&>(ufvalue);

    switch (m_config_register.gain)
    {
    case ADS1115_PGA_6144:
        o_value = fvalue * ADS1115_MV_6144;
        break;
    case ADS1115_PGA_4096:
        o_value = fvalue * ADS1115_MV_4096;
        break;
    case ADS1115_PGA_2048:
        o_value = fvalue * ADS1115_MV_2048;
        break;
    case ADS1115_PGA_1024:
        o_value = fvalue * ADS1115_MV_1024;
        break;
    case ADS1115_PGA_512:
        o_value = fvalue * ADS1115_MV_512;
        break;
    case ADS1115_PGA_256:
    case ADS1115_PGA_256B:
    case ADS1115_PGA_256C:
        o_value = fvalue * ADS1115_MV_256;
        break;
    default:
        return false;
    }

    //scale 0..3.3v to 0..1
    o_value = math::clamp(o_value / 3.3f, 0.f, 1.f);

    return true;
}

ts::Result<void> ADS1115::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("ADS1115::set_config");

    auto specialized = dynamic_cast<hal::ADS1115_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto ADS1115::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto ADS1115::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> ADS1115::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
