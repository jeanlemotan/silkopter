#include "Fly_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "IButton.h"
#include "IStick_Actuators.h"
#include "ISticks.h"

#include "Comms.h"

namespace silk
{

static float dBm_to_strength(int8_t dBm)
{
    //return math::pow(10.f, static_cast<float>(dBm) / 10.f);
    const int max_dBm = -50;
    const int min_dBm = -100;

    float factor = 1.f - static_cast<float>(max_dBm - dBm) / static_cast<float>(max_dBm - min_dBm);
    return math::clamp(factor, 0.f, 1.f);
}



Fly_Menu_Page::Fly_Menu_Page(Comms& comms)
    : m_comms(comms)
{
}

bool Fly_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    q::Clock::time_point now = q::Clock::now();
    q::Clock::duration dt = now - m_last_tp;
    m_last_tp = now;

    if (now - m_last_blink_tp >= std::chrono::milliseconds(100))
    {
        m_blink_color = !m_blink_color;
        m_last_blink_tp = now;
    }

    if (input.get_menu_switch().was_released())
    {
        return false;
    }

    m_multirotor_state = m_comms.get_multirotor_state();

    m_rx_strength = math::lerp(m_rx_strength, dBm_to_strength(m_comms.get_rx_dBm()), std::chrono::duration<float>(dt).count());
    m_slow_rx_strength = math::lerp(m_slow_rx_strength, m_rx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    m_tx_strength = math::lerp(m_tx_strength, dBm_to_strength(m_comms.get_tx_dBm()), std::chrono::duration<float>(dt).count());
    m_slow_tx_strength = math::lerp(m_slow_tx_strength, m_tx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    input.get_stick_actuators().set_target_throttle(input.get_sticks().get_yaw());

    return true;
}

void Fly_Menu_Page::render(Adafruit_GFX& display)
{
    q::Clock::time_point now = q::Clock::now();

    stream::IMultirotor_State::Value& state = m_multirotor_state;
//    sample.value.

    {
        math::vec3f euler;
        state.local_frame.get_as_euler_zxy(euler);

        math::trans2df rot;
        rot.set_rotation(math::anglef(euler.y));

        float length = 20.f;
        math::vec2f center(display.width() - length * 0.5f, 30);
        math::vec2f dir = math::rotate(rot, math::vec2f(length * 0.5f, 0));
        math::vec2f p0 = center - dir;
        math::vec2f p1 = center + dir;
        display.drawLine(p0.x, p0.y, p1.x, p1.y, 1);
    }

    int16_t y = 0;

    //mode
    {
        const char* mode_str = "N/A";
        switch (state.mode)
        {
        case stream::IMultirotor_State::Mode::IDLE: mode_str = "IDLE"; break;
        case stream::IMultirotor_State::Mode::ARMED: mode_str = "ARMED"; break;
        case stream::IMultirotor_State::Mode::TAKE_OFF: mode_str = "TAKE OFF"; break;
        case stream::IMultirotor_State::Mode::FLY: mode_str = "FLY"; break;
        case stream::IMultirotor_State::Mode::RETURN_HOME: mode_str = "RTH"; break;
        case stream::IMultirotor_State::Mode::LAND: mode_str = "LAND"; break;
        }

        display.setCursor(0, y);
        display.print(mode_str);
    }

    //signal strength
    {
        const q::Clock::duration k_timeout_duration = std::chrono::seconds(2);
        const float k_min_signal_strength = 0.15f;

        int16_t w = 60;
        int16_t h = 3;
        int16_t line_y = 0;
        int16_t x = display.width() - w;
        int16_t arrow_x = x - 5;
        {
            display.drawFastHLine(arrow_x - 2, line_y,     5, 1);
            display.drawFastHLine(arrow_x - 1, line_y + 1, 3, 1);
            display.drawFastHLine(arrow_x,     line_y + 2, 1, 1);

            bool error = m_rx_strength < k_min_signal_strength ||
                    (now - m_comms.get_last_rx_tp()) >= k_timeout_duration;
            uint16_t color = error ? m_blink_color : 1;

            display.fillRect(x, line_y, static_cast<int16_t>(m_rx_strength * w), h, color);
            int16_t slow_x = x + static_cast<int16_t>(m_slow_rx_strength * w);
            display.drawLine(slow_x,     line_y, slow_x,     line_y + h - 1, 1);
            display.drawLine(slow_x + 1, line_y, slow_x + 1, line_y + h - 1, 0);
        }

        line_y += h + 2;
        {
            display.drawFastHLine(arrow_x - 2, line_y + 2, 5, 1);
            display.drawFastHLine(arrow_x - 1, line_y + 1, 3, 1);
            display.drawFastHLine(arrow_x,     line_y,     1, 1);

            bool error = m_tx_strength < k_min_signal_strength ||
                    (now - m_comms.get_last_tx_tp()) >= k_timeout_duration;
            uint16_t color = error ? m_blink_color : 1;

            display.fillRect(x, line_y, static_cast<int16_t>(m_tx_strength * w), h, color);
            int16_t slow_x = x + static_cast<int16_t>(m_slow_tx_strength * w);
            display.drawLine(slow_x,     line_y, slow_x,     line_y + h - 1, 1);
            display.drawLine(slow_x + 1, line_y, slow_x + 1, line_y + h - 1, 0);
        }
    }

//    display.setCursor(display.width() - 72, 0);
//    display.printf("%d / %d", static_cast<int>(m_rx_strength * 100.f), static_cast<int>(m_tx_strength * 100.f));

    y += 12;
    //battery
    {
        {
            const float k_min_voltage = 9.f;
            uint16_t color = state.battery_state.average_voltage < k_min_voltage ? m_blink_color : 1;

            display.setCursor(0, y);
            display.setTextColor(color);
            display.printf("%2.1fV", state.battery_state.average_voltage);
            display.setTextColor(1);
        }

        {
            display.setCursor(40, y);
            display.printf("%2.1fA", state.battery_state.average_current);
        }

        {
            int16_t x = display.width() - 50;

            display.setCursor(x, y);
            int percentage = std::min(99, static_cast<int>(state.battery_state.capacity_left * 100.f));
            display.printf("%d%%", percentage);

            x += 20;
            int16_t w = display.width() - x;
            int16_t h = 6;

            const float k_min_capacity = 0.15f;
            uint16_t color = state.battery_state.capacity_left < k_min_capacity ? m_blink_color : 1;

            display.fillRect(x, y, static_cast<int>(state.battery_state.capacity_left * w), h, color);
            display.drawRect(x, y, w, h, color);
        }
    }

    y += 11;
    //display.drawFastHLine(0, y, display.width(), 1);

    y += 6;
    //vertical mode
    {
        const char* mode_str = "N/A";
        switch (m_commands.vertical_mode)
        {
        case silk::stream::IMultirotor_Commands::Vertical_Mode::ALTITUDE: mode_str = "ALT"; break;
        case silk::stream::IMultirotor_Commands::Vertical_Mode::THRUST: mode_str = "THR"; break;
        }

        display.setCursor(0, y);
        display.printf("V: %s", mode_str);
    }

    y += 10;
    //horizontal mode
    {
        const char* mode_str = "N/A";
        switch (m_commands.horizontal_mode)
        {
        case silk::stream::IMultirotor_Commands::Horizontal_Mode::ANGLE_RATE: mode_str = "RATE"; break;
        case silk::stream::IMultirotor_Commands::Horizontal_Mode::ANGLE: mode_str = "ANG"; break;
        case silk::stream::IMultirotor_Commands::Horizontal_Mode::VELOCITY: mode_str = "VEL"; break;
        }

        display.setCursor(0, y);
        display.printf("H: %s", mode_str);
    }

    y += 10;
    //yaw mode
    {
        const char* mode_str = "N/A";
        switch (m_commands.yaw_mode)
        {
        case silk::stream::IMultirotor_Commands::Yaw_Mode::ANGLE_RATE: mode_str = "RATE"; break;
        case silk::stream::IMultirotor_Commands::Yaw_Mode::ANGLE: mode_str = "ANG"; break;
        }

        display.setCursor(0, y);
        display.printf("Y: %s", mode_str);
    }
}

}
