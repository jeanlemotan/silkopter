#include "Battery_Info_ADS1115.h"

#include "HAL.h"
#include "settings.def.h"

namespace silk
{

Battery_Info_ADS1115::Battery_Info_ADS1115(HAL& hal, util::hw::ADS1115& adc, uint8_t channel_idx)
    : m_hal(hal)
    , m_adc(adc)
    , m_channel_idx(channel_idx)
{
}

void Battery_Info_ADS1115::init()
{
}

Battery_Info_ADS1115::~Battery_Info_ADS1115()
{
}

float Battery_Info_ADS1115::get_voltage() const
{
    return m_voltage;
}

float Battery_Info_ADS1115::get_low_voltage() const
{
    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    return bi.get_low_voltage();
}
void Battery_Info_ADS1115::set_low_voltage(float voltage)
{
    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    bi.set_low_voltage(voltage);
}

float Battery_Info_ADS1115::get_critical_voltage() const
{
    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    return bi.get_critical_voltage();
}
void Battery_Info_ADS1115::set_critical_voltage(float voltage)
{
    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    bi.set_critical_voltage(voltage);
}

Battery_Info_ADS1115::Calibration_Data Battery_Info_ADS1115::get_calibration_data() const
{
    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    Calibration_Data data;
    data.scale = bi.get_calibration_scale();
    data.bias = bi.get_calibration_bias();
    return data;
}
void Battery_Info_ADS1115::set_calibration_data(Calibration_Data const& data)
{
    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    bi.set_calibration_scale(data.scale);
    bi.set_calibration_bias(data.bias);
}

void Battery_Info_ADS1115::process()
{
    util::hw::ADS1115::Reading reading = m_adc.get_reading(m_channel_idx);
    QASSERT(reading.is_enabled);

    silk::settings::Settings::Battery_Info& bi = m_hal.get_settings().get_battery_info();
    m_voltage = reading.value * bi.get_calibration_scale() + bi.get_calibration_bias();
}


}
