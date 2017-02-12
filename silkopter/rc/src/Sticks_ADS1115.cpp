#include "Sticks_ADS1115.h"

namespace silk
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



constexpr std::chrono::milliseconds MIN_CONVERSION_DURATION(2);

///////////////////////////////////////////////////////////////////////////////////////////////////


Sticks_ADS1115::Sticks_ADS1115()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Sticks_ADS1115::~Sticks_ADS1115()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> Sticks_ADS1115::init()
{
    auto result = m_dev.init("/dev/i2c-1");
    if (result != ts::success)
    {
        return result;
    }

    m_address = ADS1115_DEFAULT_ADDRESS;

    m_config_register.gain = ADS1115_PGA_4096;
    m_config_register.mux = ADS1115_MUX_P0_NG;
    m_config_register.mode = ADS1115_MODE_SINGLESHOT;//ADS1115_MODE_CONTINUOUS;
    m_config_register.queue = ADS1115_COMP_QUE_DISABLE;
    m_config_register.rate = ADS1115_RATE_860;

    bool res = set_config_register();

    uint8_t data;
    res &= m_dev.read_register_u8(m_address, ADS1115_RA_CONFIG, data);

    //start first measurement
    m_crt_adc = 0;
    m_config_register.status = ADS1115_OS_ACTIVE;
    res &= set_config_register();
    if (!res)
    {
        return ts::Error("Cannot find ADS1115");
    }

    m_last_tp = Clock::now();

    return ts::success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

auto Sticks_ADS1115::set_config_register() -> bool
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
    return m_dev.write_register_u16(m_address, ADS1115_RA_CONFIG, config);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static float scale_value_center(float value, float min, float min_center, float max_center, float max)
{
    if (value <= min_center)
    {
        float range = min_center - min;
        value = value - min; //now the value goes betwee 0 and (center - min)
        value = value / range;//now the value is 0 - 1
        value *= 0.5f; //rescale back to 0 - 0.5
    }
    else if (value < max_center) //min_center < value < max_center
    {
        value = 0.5f;
    }
    else //value >= max_center
    {
        float range = max - max_center;
        value = value - max_center; //value is > 0
        value = value / range; //value is > 0, with 0 being center value and 1 being max value
        value = value * 0.5f + 0.5f; //rescale back to 0.5 - 1
    }

    return math::clamp(value, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_stick_value(ADC const& adc, bool apply_deadband, bool center_deadband)
{
    float value = scale_value_center(adc.value, adc.min, adc.center, adc.center, adc.max);

    if (apply_deadband)
    {
        float deadband = adc.deadband * 0.5f;
        if (center_deadband)
        {
            value = scale_value_center(value, 0.f, 0.5f - deadband, 0.5f + deadband, 1.f);
        }
        else
        {
            if (value < deadband)
            {
                value = 0.f;
            }
            else
            {
                value = (value - deadband) / (1.f - deadband); //0 - 1 range
                value = math::clamp(value, 0.f, 1.f); //just to make sure
            }
        }
    }

    return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_raw_yaw(Raw_Type raw_type) const
{
    ADC const& adc = m_adcs[0];
    if (raw_type == Raw_Type::WITHOUT_REMAPPING)
    {
        return math::clamp(adc.value, 0.f, 1.f);
    }
    else
    {
        return get_stick_value(adc, false, false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_yaw() const
{
    return get_stick_value(m_adcs[0], true, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Sticks_ADS1115::set_yaw_calibration(float min, float center, float max, float deadband)
{
    ADC& adc = m_adcs[0];
    adc.min = min;
    adc.center = center;
    adc.max = max;
    adc.deadband = deadband;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_raw_pitch(Raw_Type raw_type) const
{
    ADC const& adc = m_adcs[1];
    if (raw_type == Raw_Type::WITHOUT_REMAPPING)
    {
        return math::clamp(adc.value, 0.f, 1.f);
    }
    else
    {
        return get_stick_value(adc, false, false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_pitch() const
{
    return get_stick_value(m_adcs[1], true, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Sticks_ADS1115::set_pitch_calibration(float min, float center, float max, float deadband)
{
    ADC& adc = m_adcs[1];
    adc.min = min;
    adc.center = center;
    adc.max = max;
    adc.deadband = deadband;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_raw_roll(Raw_Type raw_type) const
{
    ADC const& adc = m_adcs[2];
    if (raw_type == Raw_Type::WITHOUT_REMAPPING)
    {
        return math::clamp(adc.value, 0.f, 1.f);
    }
    else
    {
        return get_stick_value(adc, false, false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_roll() const
{
    return get_stick_value(m_adcs[2], true, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Sticks_ADS1115::set_roll_calibration(float min, float center, float max, float deadband)
{
    ADC& adc = m_adcs[2];
    adc.min = min;
    adc.center = center;
    adc.max = max;
    adc.deadband = deadband;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Sticks_ADS1115::set_throttle_deadband_position(Deadband_Position position)
{
    m_throttle_deadband_position = position;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Sticks_ADS1115::Deadband_Position Sticks_ADS1115::get_throttle_deadband_position() const
{
    return m_throttle_deadband_position;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_raw_throttle(Raw_Type raw_type) const
{
    ADC const& adc = m_adcs[3];
    if (raw_type == Raw_Type::WITHOUT_REMAPPING)
    {
        return math::clamp(adc.value, 0.f, 1.f);
    }
    else
    {
        return get_stick_value(adc, false, false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Sticks_ADS1115::get_throttle() const
{
    return get_stick_value(m_adcs[3], true, m_throttle_deadband_position == Deadband_Position::MIDDLE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Sticks_ADS1115::set_throttle_calibration(float min, float center, float max, float deadband)
{
    ADC& adc = m_adcs[3];
    adc.min = min;
    adc.center = center;
    adc.max = max;
    adc.deadband = deadband;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Sticks_ADS1115::process()
{
    auto now = Clock::now();
    if (now - m_last_tp < MIN_CONVERSION_DURATION)
    {
        return;
    }

    if (m_config_register.mode == ADS1115_MODE_SINGLESHOT)
    {
        uint16_t cr = 0;
        if (!m_dev.read_register_u16(m_address, ADS1115_RA_CONFIG, cr))
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
    if (!m_dev.read_register_u16(m_address, ADS1115_RA_CONVERSION, ufvalue))
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
    //value = math::clamp(value / 3.3f, 0.f, 1.f);
    value = math::clamp(fvalue / 32768.f, 0.f, 1.f);

    m_adcs[m_crt_adc].value = value;

//    QLOGI("ADC: {} : {} : {} : {}", m_adcs[0].value, m_adcs[1].value, m_adcs[2].value, m_adcs[3].value);

    ///////////////////////////////////////////////////////
    //advance to next ADC
    uint8_t next_adc = (m_crt_adc + 1) % m_adcs.size();

    //start measurement again
    uint16_t muxes[] = { ADS1115_MUX_P0_NG, ADS1115_MUX_P1_NG, ADS1115_MUX_P2_NG, ADS1115_MUX_P3_NG };
    m_config_register.mux = muxes[next_adc];
    if (set_config_register())
    {
        m_crt_adc = next_adc;
    }

    m_last_tp = now;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
