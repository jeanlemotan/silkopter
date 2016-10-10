#pragma once

#include <atomic>
#include "IRotary_Encoder.h"

namespace silk
{

class Rotary_Encoder_PIGPIO : public IRotary_Encoder
{
public:
    Rotary_Encoder_PIGPIO();
    ~Rotary_Encoder_PIGPIO();

    ts::Result<void> init(uint8_t gpio1, uint8_t gpio2);

    int32_t get_clicks() const override;
    void process() override;

private:
    static void encoder_pulse(int gpio, int level, uint32_t tick, void* userdata);
    int m_lev_a = 0;
    int m_lev_b = 0;
    int m_last_gpio = -1;

    uint8_t m_gpio1 = 0;
    uint8_t m_gpio2 = 0;
    std::atomic_int m_clicks = { 0 };
};

}
