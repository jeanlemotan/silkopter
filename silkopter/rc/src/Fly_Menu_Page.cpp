#include "Fly_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "IButton.h"

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

    if (input.get_menu_switch().was_released())
    {
        return false;
    }

//    m_commands.sticks.yaw = m_sticks->get_yaw();
//    m_commands.sticks.pitch = m_sticks->get_pitch();
//    m_commands.sticks.roll = m_sticks->get_roll();
//    m_commands.sticks.throttle = m_sticks->get_throttle();

//    m_comms->send_multirotor_commands_value(m_commands);

    m_multirotor_state = m_comms.get_multirotor_state();

    m_rx_strength = math::lerp(m_rx_strength, dBm_to_strength(m_comms.get_rx_dBm()), std::chrono::duration<float>(dt).count());
    m_slow_rx_strength = math::lerp(m_slow_rx_strength, m_rx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    m_tx_strength = math::lerp(m_tx_strength, dBm_to_strength(m_comms.get_tx_dBm()), std::chrono::duration<float>(dt).count());
    m_slow_tx_strength = math::lerp(m_slow_tx_strength, m_tx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    return true;
}

void Fly_Menu_Page::render(Adafruit_GFX& display)
{
    stream::IMultirotor_State::Value& state = m_multirotor_state;
//    sample.value.

    {
        math::vec3f euler;
        state.local_frame.get_as_euler_zxy(euler);

        math::trans2df rot;
        rot.set_rotation(math::anglef(euler.y));

        float length = 20.f;
        math::vec2f center(display.width() - length * 0.5f, 20);
        math::vec2f dir = math::rotate(rot, math::vec2f(length * 0.5f, 0));
        math::vec2f p0 = center - dir;
        math::vec2f p1 = center + dir;
        display.drawLine(p0.x, p0.y, p1.x, p1.y, 1);
    }

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

    display.setCursor(0, 0);
    display.print(mode_str);

    {
        int16_t w = 60;
        int16_t h = 3;
        int16_t y = 0;
        {
            display.fillRect(display.width() - w, y, static_cast<int16_t>(m_rx_strength * w), h, 1);
            int16_t slow_x = display.width() - w + static_cast<int16_t>(m_slow_rx_strength * w);
            display.drawLine(slow_x,     y, slow_x,     y + h - 1, 1);
            display.drawLine(slow_x + 1, y, slow_x + 1, y + h - 1, 0);
        }

        y += h + 2;
        {
            display.fillRect(display.width() - w, y, static_cast<int16_t>(m_tx_strength * w), h, 1);
            int16_t slow_x = display.width() - w + static_cast<int16_t>(m_slow_tx_strength * w);
            display.drawLine(slow_x,     y, slow_x,     y + h - 1, 1);
            display.drawLine(slow_x + 1, y, slow_x + 1, y + h - 1, 0);
        }
    }

//    display.setCursor(display.width() - 72, 0);
//    display.printf("%d / %d", static_cast<int>(m_rx_strength * 100.f), static_cast<int>(m_tx_strength * 100.f));

    display.setCursor(0, 12);
    display.printf("%.1fV %.1fA %d%%",
                   state.battery_state.average_voltage,
                   state.battery_state.average_current,
                   static_cast<int>(state.battery_state.capacity_left * 100.f));
}

}
