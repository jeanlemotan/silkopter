#pragma once

#include "def_lang/Result.h"
#include "IGimbal_Control.h"
#include "utils/hw/I2C_Dev.h"
#include "utils/hw/ADS1115.h"
#include "utils/Clock.h"
#include <boost/optional.hpp>

namespace silk
{

class Gimbal_Control_ADS1115 : public IGimbal_Control
{
public:
    Gimbal_Control_ADS1115(util::hw::ADS1115& ads1115, boost::optional<size_t> pitch_adc, boost::optional<size_t> roll_adc);
    ~Gimbal_Control_ADS1115();

    void init();

    float get_raw_pitch() const override;
    float get_pitch() const override;
    void set_pitch_calibration(float min, float center, float max) override;

    float get_raw_roll() const override;
    float get_roll() const override;
    void set_roll_calibration(float min, float center, float max) override;

    void process() override;

private:
    util::hw::ADS1115& m_ads1115;
    boost::optional<size_t> m_pitch_adc;
    boost::optional<size_t> m_roll_adc;

    struct ADC
    {
        float value = 0.f;
        float min = 0.f;
        float center = 0.5f;
        float max = 1.f;
    };

    std::array<ADC, 2> m_adcs;

    static float get_stick_value(ADC const& adc);
};

}
