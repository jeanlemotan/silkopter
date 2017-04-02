#include "Sticks_ADS1115.h"

namespace silk
{

///////////////////////////////////////////////////////////////////////////////////////////////////


Sticks_ADS1115::Sticks_ADS1115(util::hw::II2C& i2c)
    : m_i2c(i2c)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Sticks_ADS1115::~Sticks_ADS1115()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> Sticks_ADS1115::init()
{
    util::hw::ADS1115::Descriptor descriptor;
    descriptor.i2c_address = 0x48;
    descriptor.adcs[0].is_enabled = true;
    descriptor.adcs[0].rate = 200;
    descriptor.adcs[1].is_enabled = true;
    descriptor.adcs[1].rate = 200;
    descriptor.adcs[2].is_enabled = true;
    descriptor.adcs[2].rate = 200;
    descriptor.adcs[3].is_enabled = true;
    descriptor.adcs[3].rate = 200;

    ts::Result<void> result = m_ads1115.init(m_i2c, descriptor);
    if (result != ts::success)
    {
        return result.error();
    }

    return ts::success;
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
    m_ads1115.process(m_i2c);

    for (size_t i = 0; i < 4; i++)
    {
        util::hw::ADS1115::Reading reading = m_ads1115.get_reading(i);
        m_adcs[i].value = reading.value;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
