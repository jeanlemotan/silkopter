#include "ADS1115.h"
#include "def_lang/Result.h"

namespace util
{
namespace hw
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


ADS1115::ADS1115()
{
}

ts::Result<void> ADS1115::init(II2C& i2c, Descriptor const& descriptor)
{
    QLOG_TOPIC("ADS1115::init");

    m_descriptor = descriptor;
    return init(i2c);
}

ts::Result<void> ADS1115::init(II2C& i2c)
{
    size_t i = 0;
    for (Descriptor::ADC const& dadc: m_descriptor.adcs)
    {
        ADC adc;
        adc.descriptor = dadc;
        adc.last_tp = Clock::now();
        m_adcs.push_back(adc);
    }

    m_config_register.gain = ADS1115_PGA_4096;
    m_config_register.mux = ADS1115_MUX_P0_NG;
    m_config_register.mode = ADS1115_MODE_SINGLESHOT;//ADS1115_MODE_CONTINUOUS;
    m_config_register.queue = ADS1115_COMP_QUE_DISABLE;
    m_config_register.rate = ADS1115_RATE_860;

    bool res = set_config_register(i2c);

    uint8_t data;
    res &= i2c.read_register_u8(m_descriptor.i2c_address, ADS1115_RA_CONFIG, data);

    //start first measurement
    m_crt_adc_idx = 0;
    m_config_register.status = ADS1115_OS_ACTIVE;
    res &= set_config_register(i2c);
    if (!res)
    {
        return ts::Error("Cannot find ADS1115");
    }
    return ts::success;
}

bool ADS1115::set_config_register(II2C& i2c)
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
    return i2c.write_register_u16(m_descriptor.i2c_address, ADS1115_RA_CONFIG, config);
}

ADS1115::Reading ADS1115::get_reading(uint32_t adc_idx) const
{
    if (adc_idx >= m_adcs.size())
    {
        return Reading();
    }

    ADC const& adc = m_adcs[adc_idx];

    Reading reading;
    reading.is_enabled = adc.descriptor.is_enabled;
    reading.last_tp = adc.last_tp;
    reading.value = adc.value;
    return reading;
}

void ADS1115::process(II2C& i2c)
{
    uint8_t next_adc_idx = m_crt_adc_idx;
    bool schedule_reading = false;

    auto now = Clock::now();

    //refresh the current adc pin
    if (now - m_schedule_tp >= MIN_CONVERSION_DURATION)
    {
        ADC& adc = m_adcs[m_crt_adc_idx];
        float value = adc.value;
        bool good = read_sensor(i2c, value);
        if (good)
        {
            adc.value = value;
            adc.last_tp = now;

            //move to the next one
            next_adc_idx = (m_crt_adc_idx + 1) % m_adcs.size();
            schedule_reading = true;
        }
    }

    //start the next reading
    if (schedule_reading)
    {
        //start measurement again
        uint16_t muxes[] = { ADS1115_MUX_P0_NG, ADS1115_MUX_P1_NG, ADS1115_MUX_P2_NG, ADS1115_MUX_P3_NG };
        m_config_register.mux = muxes[next_adc_idx];
        if (set_config_register(i2c))
        {
            m_crt_adc_idx = next_adc_idx;
            m_schedule_tp = Clock::now();
        }
    }
}

bool ADS1115::read_sensor(II2C& i2c, float& o_value)
{
    if (m_config_register.mode == ADS1115_MODE_SINGLESHOT)
    {
        uint16_t cr = 0;
        if (!i2c.read_register_u16(m_descriptor.i2c_address, ADS1115_RA_CONFIG, cr))
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
    if (!i2c.read_register_u16(m_descriptor.i2c_address, ADS1115_RA_CONVERSION, ufvalue))
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

}
}
