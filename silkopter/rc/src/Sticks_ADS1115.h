#pragma once

#include "def_lang/Result.h"
#include "ISticks.h"
#include "utils/hw/I2C_Dev.h"
#include "utils/hw/ADS1115.h"
#include "utils/Clock.h"

namespace silk
{

class Sticks_ADS1115 : public ISticks
{
public:
    Sticks_ADS1115(util::hw::II2C& i2c);
    ~Sticks_ADS1115();

    ts::Result<void> init();
    float get_raw_yaw(Raw_Type raw_type) const override;
    float get_yaw() const override;
    void set_yaw_calibration(float min, float center, float max, float deadband, float curve) override;

    float get_raw_pitch(Raw_Type raw_type) const override;
    float get_pitch() const override;
    void set_pitch_calibration(float min, float center, float max, float deadband, float curve) override;

    float get_raw_roll(Raw_Type raw_type) const override;
    float get_roll() const override;
    void set_roll_calibration(float min, float center, float max, float deadband, float curve) override;

    float get_raw_throttle(Raw_Type raw_type) const override;
    float get_throttle() const override;
    void set_throttle_calibration(float min, float center, float max, float deadband, float curve) override;
    void set_throttle_deadband_position(Deadband_Position position) override;
    Deadband_Position get_throttle_deadband_position() const override;

    void process() override;

private:
    util::hw::II2C& m_i2c;
    util::hw::ADS1115 m_ads1115;

    struct ADC
    {
        float value = 0.f;
        float min = 0.f;
        float center = 0.5f;
        float max = 1.f;
        float deadband = 0.f;
        float curve = 1.f;
    };

    std::array<ADC, 4> m_adcs;

    Deadband_Position m_throttle_deadband_position = Deadband_Position::LOW;

    static float get_remapped_stick_value(ADC const& adc);
    static float get_stick_value(ADC const& adc, bool center_deadband);
};

}
