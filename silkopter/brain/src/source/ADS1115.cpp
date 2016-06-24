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








constexpr std::chrono::milliseconds MIN_CONVERSION_DURATION(5);



ADS1115::ADS1115(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::ADS1115_Descriptor())
    , m_config(new hal::ADS1115_Config())
{
    for (auto& adc: m_adcs)
    {
        adc = std::make_shared<Output_Stream>();
    }
}

auto ADS1115::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "ADC0", m_adcs[0] },
        { "ADC1", m_adcs[1] },
        { "ADC2", m_adcs[2] },
        { "ADC3", m_adcs[3] }
     }};
    return outputs;
}
auto ADS1115::init(hal::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("ADS1115::init");

    auto specialized = dynamic_cast<hal::ADS1115_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}

auto ADS1115::init() -> bool
{
    m_i2c = m_hal.get_bus_registry().find_by_name<bus::II2C>(m_descriptor->get_bus());

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });


//    m_descriptor->adc0_rate = math::clamp<size_t>(m_descriptor->adc0_rate, 1, 500);
//    m_descriptor->adc1_rate = math::clamp<size_t>(m_descriptor->adc1_rate, 1, 500);
//    m_descriptor->adc2_rate = math::clamp<size_t>(m_descriptor->adc2_rate, 1, 500);
//    m_descriptor->adc3_rate = math::clamp<size_t>(m_descriptor->adc3_rate, 1, 500);


    m_adcs[0]->set_rate(m_descriptor->get_adc0_rate());
    m_adcs[1]->set_rate(m_descriptor->get_adc1_rate());
    m_adcs[2]->set_rate(m_descriptor->get_adc2_rate());
    m_adcs[3]->set_rate(m_descriptor->get_adc3_rate());

    m_config_register.gain = ADS1115_PGA_4096;
    m_config_register.mux = ADS1115_MUX_P0_NG;
    m_config_register.mode = ADS1115_MODE_SINGLESHOT;//ADS1115_MODE_CONTINUOUS;
    m_config_register.queue = ADS1115_COMP_QUE_DISABLE;
    m_config_register.rate = ADS1115_RATE_860;

    bool res = set_config_register(*i2c);

    uint8_t data;
    res &= i2c->read_register_u8(ADS1115_DEFAULT_ADDRESS, ADS1115_RA_CONFIG, data);

    //start first measurement
    m_crt_adc = 0;
    m_config_register.status = ADS1115_OS_ACTIVE;
    res &= set_config_register(*i2c);

    return res;
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
    return i2c.write_register_u16(ADS1115_DEFAULT_ADDRESS, ADS1115_RA_CONFIG, config);
}

auto ADS1115::start(q::Clock::time_point tp) -> bool
{
    m_last_tp = tp;
    for (auto& adc: m_adcs)
    {
        adc->set_tp(tp);
    }
    return true;
}


void ADS1115::process()
{
    QLOG_TOPIC("ADS1115::process");

    for (auto& adc: m_adcs)
    {
        adc->clear();
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

    auto& adc = *m_adcs[m_crt_adc];
    auto samples_needed = adc.compute_samples_needed();
    auto now = q::Clock::now();
    if (samples_needed == 0 || now - m_last_tp < MIN_CONVERSION_DURATION)
    {
        return;
    }

    //TODO - add healthy indication

    if (m_config_register.mode == ADS1115_MODE_SINGLESHOT)
    {
        uint16_t cr = 0;
        if (!i2c->read_register_u16(ADS1115_DEFAULT_ADDRESS, ADS1115_RA_CONFIG, cr))
        {
            return;
        }
        //not ready
        if ((cr & ADS1115_OS_ACTIVE) == 0)
        {
            return;
        }
    }

    uint16_t ufvalue = 0;
    if (!i2c->read_register_u16(ADS1115_DEFAULT_ADDRESS, ADS1115_RA_CONVERSION, ufvalue))
    {
        return;
    }
    int16_t fvalue = reinterpret_cast<int16_t&>(ufvalue);

    float value = 0;
    switch (m_config_register.gain)
    {
    case ADS1115_PGA_6144:
        value = fvalue * ADS1115_MV_6144;
        break;
    case ADS1115_PGA_4096:
        value = fvalue * ADS1115_MV_4096;
        break;
    case ADS1115_PGA_2048:
        value = fvalue * ADS1115_MV_2048;
        break;
    case ADS1115_PGA_1024:
        value = fvalue * ADS1115_MV_1024;
        break;
    case ADS1115_PGA_512:
        value = fvalue * ADS1115_MV_512;
        break;
    case ADS1115_PGA_256:
    case ADS1115_PGA_256B:
    case ADS1115_PGA_256C:
        value = fvalue * ADS1115_MV_256;
        break;
    default:
        return;
    }

    //scale 0..3.3v to 0..1
    value = math::clamp(value / 3.3f, 0.f, 1.f);

    //add samples up to the sample rate
    while (samples_needed > 0)
    {
        adc.push_sample(value, true);
        samples_needed--;
    }


    ///////////////////////////////////////////////////////
    //advance to next ADC
    uint8_t next_adc = (m_crt_adc + 1) % m_adcs.size();

    //start measurement again
    uint16_t muxes[] = { ADS1115_MUX_P0_NG, ADS1115_MUX_P1_NG, ADS1115_MUX_P2_NG, ADS1115_MUX_P3_NG };
    m_config_register.mux = muxes[next_adc];
    if (set_config_register(*i2c))
    {
        m_crt_adc = next_adc;
    }

    m_last_tp = now;
}

auto ADS1115::set_config(hal::INode_Config const& config) -> bool
{
    QLOG_TOPIC("ADS1115::set_config");

    auto specialized = dynamic_cast<hal::ADS1115_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
}
auto ADS1115::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto ADS1115::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto ADS1115::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
