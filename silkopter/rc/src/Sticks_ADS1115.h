#pragma once

#include "def_lang/Result.h"
#include "ISticks.h"
#include "utils/hw/I2C_Dev.h"
#include "utils/Clock.h"

namespace silk
{

class Sticks_ADS1115 : public ISticks
{
public:
    Sticks_ADS1115();
    ~Sticks_ADS1115();

    ts::Result<void> init();
    float get_raw_yaw() const override;
    float get_yaw() const override;
    void set_yaw_calibration(float center, float min, float max) override;

    float get_raw_pitch() const override;
    float get_pitch() const override;
    void set_pitch_calibration(float center, float min, float max) override;

    float get_raw_roll() const override;
    float get_roll() const override;
    void set_roll_calibration(float center, float min, float max) override;

    float get_raw_throttle() const override;
    float get_throttle() const override;
    void set_throttle_calibration(float center, float min, float max) override;

    void process() override;

private:
    util::hw::I2C_Dev m_dev;
    uint8_t m_address = 0;

    Clock::time_point m_last_tp = Clock::now();

    bool set_config_register();

    struct Config_Register
    {
        uint16_t gain = 0;
        uint16_t mux = 0;
        uint16_t status = 0;
        uint16_t mode = 0;
        uint16_t rate = 0;
        uint16_t comparator = 0;
        uint16_t polarity = 0;
        uint16_t latch = 0;
        uint16_t queue = 0;
    } m_config_register;

    struct ADC
    {
        float value = 0.f;
        float center = 0.5f;
        float min = 0.f;
        float max = 1.f;
    };

    std::array<ADC, 4> m_adcs;

    uint8_t m_crt_adc = 0;
};

}
