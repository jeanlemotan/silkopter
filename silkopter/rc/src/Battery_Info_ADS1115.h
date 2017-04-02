#pragma once

#include "IBattery_Info.h"
#include "utils/hw/ADS1115.h"

namespace silk
{

class HAL;

class Battery_Info_ADS1115 : public IBattery_Info
{
public:
    Battery_Info_ADS1115(HAL& hal, util::hw::ADS1115& adc, uint8_t channel_idx);
    ~Battery_Info_ADS1115();

    void init();

    float get_voltage() const override;

    float get_low_voltage() const override;
    void set_low_voltage(float voltage) override;

    float get_critical_voltage() const override;
    void set_critical_voltage(float voltage) override;

    Calibration_Data get_calibration_data() const override;
    void set_calibration_data(Calibration_Data const& data) override;

    void process() override;

private:
    HAL& m_hal;
    util::hw::ADS1115& m_adc;
    uint8_t m_channel_idx = 0;

    float m_voltage = 0.f;
};

}
