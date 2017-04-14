#include "Gimbal_Control_ADS1115.h"

namespace silk
{

///////////////////////////////////////////////////////////////////////////////////////////////////


Gimbal_Control_ADS1115::Gimbal_Control_ADS1115(util::hw::ADS1115& ads1115, boost::optional<size_t> pitch_adc, boost::optional<size_t> roll_adc)
    : m_ads1115(ads1115)
    , m_pitch_adc(pitch_adc)
    , m_roll_adc(roll_adc)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Gimbal_Control_ADS1115::~Gimbal_Control_ADS1115()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Gimbal_Control_ADS1115::init()
{

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

float Gimbal_Control_ADS1115::get_stick_value(ADC const& adc)
{
    float value = scale_value_center(adc.value, adc.min, adc.center, adc.center, adc.max);
    return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Gimbal_Control_ADS1115::get_raw_pitch() const
{
    ADC const& adc = m_adcs[0];
    return math::clamp(adc.value, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Gimbal_Control_ADS1115::get_pitch() const
{
    return get_stick_value(m_adcs[0]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Gimbal_Control_ADS1115::set_pitch_calibration(float min, float center, float max)
{
    ADC& adc = m_adcs[0];
    adc.min = min;
    adc.center = center;
    adc.max = max;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Gimbal_Control_ADS1115::get_raw_roll() const
{
    ADC const& adc = m_adcs[1];
    return math::clamp(adc.value, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float Gimbal_Control_ADS1115::get_roll() const
{
    return get_stick_value(m_adcs[1]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Gimbal_Control_ADS1115::set_roll_calibration(float min, float center, float max)
{
    ADC& adc = m_adcs[1];
    adc.min = min;
    adc.center = center;
    adc.max = max;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Gimbal_Control_ADS1115::process()
{
    if (m_pitch_adc != boost::none)
    {
        util::hw::ADS1115::Reading reading = m_ads1115.get_reading(*m_pitch_adc);
        m_adcs[0].value = reading.value;
    }
    if (m_roll_adc != boost::none)
    {
        util::hw::ADS1115::Reading reading = m_ads1115.get_reading(*m_roll_adc);
        m_adcs[1].value = reading.value;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
