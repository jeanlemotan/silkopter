#pragma once

#include "II2C.h"
#include "utils/Clock.h"

namespace util
{
namespace hw
{

class ADS1115
{
public:
    ADS1115();

    struct Descriptor
    {
        struct ADC
        {
            bool is_enabled = true;
            uint32_t rate = 100;
        };

        std::array<ADC, 4> adcs;
        uint8_t i2c_address = 0;
    };

    ts::Result<void> init(II2C& i2c, Descriptor const& descriptor);

    void process(II2C& i2c);

    struct Reading
    {
        bool is_enabled = false;
        float value = 0; //0 - 1
        Clock::time_point last_tp = Clock::time_point(Clock::duration::zero());
    };

    //adc_idx is 0 - 3
    Reading get_reading(uint32_t adc_idx) const;

private:
    ts::Result<void> init(II2C& i2c);

    bool read_sensor(II2C& i2c, float& o_value);

    Descriptor m_descriptor;

    Clock::time_point m_schedule_tp = Clock::now();

    struct ADC
    {
        Descriptor::ADC descriptor;
        float value = 0;
        Clock::time_point last_tp = Clock::now();
    };

    mutable std::vector<ADC> m_adcs;

    bool set_config_register(II2C& i2c);

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

    uint8_t m_crt_adc_idx = 0;
};


}
}
