#include "Button_Matrix_PIGPIO.h"
#include "IButton.h"

#include "utils/hw/pigpio.h"

namespace silk
{

struct Button_Matrix_PIGPIO::Matrix_Button : public IButton
{
    uint32_t m_press_count = 0;
    uint32_t m_last_press_count = 0;

    uint32_t m_release_count = 0;
    uint32_t m_last_release_count = 0;

    size_t m_pressed_cycle_count = 0;
    static constexpr size_t PRESSED_CYCLE_COUNT = 10;

    bool m_is_pressed = false;
    bool m_was_pressed = false;
    bool m_was_released = false;

    bool is_pressed() const override { return m_is_pressed; }
    bool was_pressed() const override { return m_was_pressed; }
    bool was_released() const override { return m_was_released; }
    uint32_t get_press_count() const override { return m_press_count; }
    uint32_t get_release_count() const override { return m_release_count; }

    void process() {}
};

///////////////////////////////////////////////////////////////////////////

Button_Matrix_PIGPIO::Button_Matrix_PIGPIO()
{

}
Button_Matrix_PIGPIO::~Button_Matrix_PIGPIO()
{

}

ts::Result<void> Button_Matrix_PIGPIO::init(std::vector<uint8_t> const& row_gpios, std::vector<uint8_t> const& column_gpios)
{
    if (row_gpios.empty() || column_gpios.empty())
    {
        return ts::Error("Invalid rows/columns");
    }

    for (size_t i = 0; i < row_gpios.size(); i++)
    {
        uint8_t row_gpio = row_gpios[i];
        if (gpioSetMode(row_gpio, PI_INPUT) != 0)
        {
            return ts::Error("Cannot set as input GPIO " + std::to_string(row_gpio));
        }
        gpioSetPullUpDown(row_gpio, PI_PUD_DOWN);
    }

    for (size_t i = 0; i < column_gpios.size(); i++)
    {
        uint8_t column_gpio = column_gpios[i];
        if (gpioSetMode(column_gpio, PI_OUTPUT) != 0)
        {
            return ts::Error("Cannot set as output GPIO " + std::to_string(column_gpio));
        }
        gpioWrite(column_gpio, 0);
        gpioSetMode(column_gpio, PI_INPUT);
    }

    m_row_gpios = row_gpios;
    m_column_gpios = column_gpios;

    m_buttons.resize(m_row_gpios.size() * m_column_gpios.size());
    for (size_t c = 0; c < m_column_gpios.size(); c++)
    {
        for (size_t r = 0; r < m_row_gpios.size(); r++)
        {
            m_buttons[get_button_index(r, c)].reset(new Matrix_Button());
        }
    }

    return ts::success;
}

size_t Button_Matrix_PIGPIO::get_row_count() const
{
    return m_row_gpios.size();
}

size_t Button_Matrix_PIGPIO::get_column_count() const
{
    return m_column_gpios.size();
}

IButton& Button_Matrix_PIGPIO::get_button(size_t row, size_t column) const
{
    return *m_buttons[get_button_index(row, column)];
}

size_t Button_Matrix_PIGPIO::get_button_index(size_t row, size_t column) const
{
    return row + column * get_row_count();
}

static void microsleep(uint32_t us)
{
    auto start = q::Clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(q::Clock::now() - start).count() < us);
}

void Button_Matrix_PIGPIO::process()
{
    //first gpio
    uint8_t column_gpio = m_column_gpios[0];
    gpioSetMode(column_gpio, PI_OUTPUT);
    gpioWrite(column_gpio, 1);

    //read
    for (size_t c = 0; c < m_column_gpios.size(); c++)
    {
        //to allow the pin to ramp up
        microsleep(5);

        for (size_t r = 0; r < m_row_gpios.size(); r++)
        {
            uint8_t row_gpio = m_row_gpios[r];
            int level = gpioRead(row_gpio);

            //compute the number of cycles the button has been on
            Matrix_Button& button = *m_buttons[get_button_index(r, c)];
            if (level)
            {
                if (button.m_pressed_cycle_count < Matrix_Button::PRESSED_CYCLE_COUNT)
                {
                    button.m_pressed_cycle_count++;
                }
            }
            else
            {
                if (button.m_pressed_cycle_count > 0)
                {
                    button.m_pressed_cycle_count--;
                }
            }

            //compute the pressed/released status using a hysteresis
            if (button.m_pressed_cycle_count >= Matrix_Button::PRESSED_CYCLE_COUNT)
            {
                if (!button.m_is_pressed)
                {
                    QLOGI("Pressed {}/{}", r, c);
                    button.m_press_count++;
                }
                button.m_is_pressed = true;
            }
            else if (button.m_pressed_cycle_count == 0)
            {
                if (button.m_is_pressed)
                {
                    QLOGI("Released {}/{}", r, c);
                    button.m_release_count++;
                }
                button.m_is_pressed = false;
            }
        }

        gpioWrite(column_gpio, 0);
        gpioSetMode(column_gpio, PI_INPUT);

        //set the next pin to output
        if (c + 1 < m_column_gpios.size())
        {
            column_gpio = m_column_gpios[c + 1];
            gpioSetMode(column_gpio, PI_OUTPUT);

            gpioWrite(column_gpio, 1);
        }
    }

    for (std::unique_ptr<Matrix_Button>& button: m_buttons)
    {
        button->m_was_pressed = button->get_press_count() > button->m_last_press_count;
        button->m_last_press_count = button->get_press_count();

        button->m_was_released = button->get_release_count() > button->m_last_release_count;
        button->m_last_release_count = button->get_release_count();
    }
}

}
