#pragma once

#include <atomic>
#include "IButton.h"

namespace silk
{

class Button_PIGPIO : public IButton
{
public:
    Button_PIGPIO();
    ~Button_PIGPIO();

    ts::Result<void> init(uint8_t gpio);

    bool is_pressed() const override;
    bool was_pressed() const override;
    bool was_released() const override;

    uint32_t get_press_count() const override;
    uint32_t get_release_count() const override;
    void process() override;

private:
    static void switch_pulse(int gpio, int level, uint32_t tick, void* userdata);

    uint8_t m_gpio = 0;
    std::atomic_bool m_is_pressed = { false };
    std::atomic_uint m_press_count = { 0 };
    std::atomic_uint m_release_count = { 0 };

    bool m_was_pressed = false;
    bool m_was_released = false;

    uint32_t m_last_press_count = 0;
    uint32_t m_last_release_count = 0;
};

}
