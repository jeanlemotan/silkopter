#include "Fly_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "IButton.h"
#include "IStick_Actuators.h"
#include "ISticks.h"
#include "IGimbal_Control.h"
#include "IHaptic.h"

#include "HAL.h"

namespace silk
{


std::vector<IHaptic::Note> k_mode_change_haptic =
{{
    { 1000u, std::chrono::milliseconds(30) },
    { 0, std::chrono::milliseconds(30) },
}};

std::vector<IHaptic::Note> k_alert_haptic =
{{
    { 1000u, std::chrono::milliseconds(100) },
    { 0, std::chrono::milliseconds(30) },
    { 1000u, std::chrono::milliseconds(100) },
    { 0, std::chrono::milliseconds(30) },
    { 1000u, std::chrono::milliseconds(100) },
    { 0, std::chrono::milliseconds(30) }
}};

///////////////////////////////////////////////////////////////////////////////////////////////////

static float dBm_to_strength(int8_t dBm)
{
    //return math::pow(10.f, static_cast<float>(dBm) / 10.f);
    const int max_dBm = -70;
    const int min_dBm = -120;

    float factor = 1.f - static_cast<float>(max_dBm - dBm) / static_cast<float>(max_dBm - min_dBm);
    return math::clamp(factor, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Fly_Menu_Page::Fly_Menu_Page(HAL& hal)
    : m_hal(hal)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::init(Input& input)
{
    m_multirotor_state = m_hal.get_comms().get_multirotor_state();

    set_mode(input, m_multirotor_state.mode);

    //start up in a safe state if flying
    if (m_multirotor_state.mode == stream::IMultirotor_State::Mode::FLY)
    {
        set_vertical_mode(input, Vertical_Mode::ALTITUDE);
        set_horizontal_mode(input, Horizontal_Mode::POSITION);
        set_yaw_mode(input, Yaw_Mode::ANGLE);
    }

    m_is_initialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Fly_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    if (!m_is_initialized)
    {
        init(input);
    }

    Clock::time_point now = Clock::now();
    Clock::duration dt = now - m_last_tp;
    m_last_tp = now;

    if (now - m_last_blink_tp >= std::chrono::milliseconds(100))
    {
        m_blink_color = !m_blink_color;
        m_last_blink_tp = now;
    }
    if (now - m_last_fast_blink_tp >= std::chrono::milliseconds(50))
    {
        m_fast_blink_color = !m_fast_blink_color;
        m_last_fast_blink_tp = now;
    }

    if (input.get_menu_switch().was_released())
    {
        //leave the uav in a safe state
        if (m_multirotor_state.mode == stream::IMultirotor_State::Mode::FLY)
        {
            set_vertical_mode(input, Vertical_Mode::ALTITUDE);
            set_horizontal_mode(input, Horizontal_Mode::POSITION);
            set_yaw_mode(input, Yaw_Mode::ANGLE);
        }

        return false;
    }

    Comms& comms = m_hal.get_comms();

    m_multirotor_state = comms.get_multirotor_state();

    m_rx_strength = math::lerp<math::safe>(m_rx_strength, dBm_to_strength(comms.get_rx_dBm()), std::chrono::duration<float>(dt).count());
    m_slow_rx_strength = math::lerp<math::safe>(m_slow_rx_strength, m_rx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    m_tx_strength = math::lerp<math::safe>(m_tx_strength, dBm_to_strength(comms.get_tx_dBm()), std::chrono::duration<float>(dt).count());
    m_slow_tx_strength = math::lerp<math::safe>(m_slow_tx_strength, m_tx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    stream::IMultirotor_State::Value const& state = m_multirotor_state;
    if (state.mode != m_multirotor_commands.mode)
    {
        if (now - m_last_mode_change_tp >= std::chrono::seconds(2))
        {
            QLOGI("REVERTED to mode {}!!!", state.mode);
            set_mode(input, state.mode);
            input.get_haptic().vibrate(k_alert_haptic);
            return true;
        }
    }
    else
    {
        switch (m_multirotor_commands.mode)
        {
        case stream::IMultirotor_State::Mode::IDLE: process_mode_idle(input); break;
        case stream::IMultirotor_State::Mode::TAKE_OFF: process_mode_take_off(input); break;
        case stream::IMultirotor_State::Mode::FLY: process_mode_fly(input); break;
        case stream::IMultirotor_State::Mode::RETURN_HOME: process_mode_return_home(input); break;
        case stream::IMultirotor_State::Mode::LAND: process_mode_land(input); break;
        }
    }

    m_multirotor_commands.gimbal_pitch = m_hal.get_gimbal_control().get_pitch();

    process_camera_commands(input);

    comms.send_multirotor_commands_value(m_multirotor_commands);
    comms.send_camera_commands_value(m_camera_commands);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::process_camera_commands(Input& input)
{
    if (input.get_camera_quality_switch().was_released())
    {
        if (m_camera_commands.quality == silk::stream::ICamera_Commands::Quality::HIGH)
        {
            m_camera_commands.quality = silk::stream::ICamera_Commands::Quality::LOW;
        }
        else
        {
            m_camera_commands.quality = silk::stream::ICamera_Commands::Quality::HIGH;
        }
        QLOGI("Switching camera quality to {}", m_camera_commands.quality);
    }

    if (input.get_camera_recording_switch().was_released())
    {
        m_camera_commands.recording = !m_camera_commands.recording;
        QLOGI("Camera recording {}", m_camera_commands.recording ? "ON" : "OFF");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::set_mode(Input& input, stream::IMultirotor_Commands::Mode new_mode)
{
    stream::IMultirotor_Commands::Mode old_mode = m_multirotor_commands.mode;
    if (old_mode == new_mode && m_is_initialized)
    {
        return;
    }

    m_last_mode_change_tp = Clock::now();

    if (new_mode == stream::IMultirotor_State::Mode::IDLE)
    {
        input.get_haptic().vibrate(k_alert_haptic);
        m_idle_mode_data.is_pressed = false;
        set_vertical_mode(input, Vertical_Mode::THRUST);
        set_horizontal_mode(input, Horizontal_Mode::ANGLE);
    }

    if (old_mode == stream::IMultirotor_State::Mode::IDLE)
    {
        input.get_haptic().vibrate(k_alert_haptic);
    }
    else if (old_mode == stream::IMultirotor_State::Mode::RETURN_HOME)
    {
        //when leaving RTH, these are the best modes
        set_vertical_mode(input, Vertical_Mode::ALTITUDE);
        set_horizontal_mode(input, Horizontal_Mode::POSITION);
        //no need to change the yaw as it's user controllable in RTH
    }

//    if (new_mode == stream::IMultirotor_State::Mode::IDLE)
//    {
//        input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::LOW);
//        input.get_stick_actuators().set_target_throttle(0.f);
//        m_idle_mode_data.is_pressed = false;
//    }

    m_multirotor_commands.mode = new_mode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::set_vertical_mode(Input& input, Vertical_Mode new_mode)
{
    if (m_multirotor_commands.vertical_mode == new_mode && m_is_initialized)
    {
        return;
    }

    m_last_vertical_mode_change_tp = Clock::now();
    input.get_haptic().vibrate(k_mode_change_haptic);

    if (new_mode == Vertical_Mode::ALTITUDE)
    {
        input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::MIDDLE);
        m_multirotor_commands.sticks.throttle = 0.5f;
    }
    else
    {
        input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::LOW);
        m_multirotor_commands.sticks.throttle = m_multirotor_state.throttle;
    }
    input.get_stick_actuators().set_target_throttle(m_multirotor_commands.sticks.throttle);

    m_multirotor_commands.vertical_mode = new_mode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::set_horizontal_mode(Input& input, Horizontal_Mode new_mode)
{
    if (m_multirotor_commands.horizontal_mode == new_mode && m_is_initialized)
    {
        return;
    }

    m_last_horizontal_mode_change_tp = Clock::now();
    input.get_haptic().vibrate(k_mode_change_haptic);

    m_multirotor_commands.horizontal_mode = new_mode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::set_yaw_mode(Input& input, Yaw_Mode new_mode)
{
    if (m_multirotor_commands.yaw_mode == new_mode && m_is_initialized)
    {
        return;
    }

    m_last_yaw_mode_change_tp = Clock::now();
    input.get_haptic().vibrate(k_mode_change_haptic);

    m_multirotor_commands.yaw_mode = new_mode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::process_mode_idle(Input& input)
{
    Clock::time_point now = Clock::now();

    if (input.get_mode_switch().was_pressed())
    {
        m_idle_mode_data.is_pressed = true;
        m_idle_mode_data.pressed_tp = now;
    }

    if (m_idle_mode_data.is_pressed && input.get_mode_switch().was_released())
    {
        if (now - m_idle_mode_data.pressed_tp >= std::chrono::seconds(2))
        {
            QLOGI("ARMED!!!");
            set_mode(input, stream::IMultirotor_Commands::Mode::FLY);
            return;
        }
        m_idle_mode_data.is_pressed = false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::process_mode_fly(Input& input)
{
    stream::IMultirotor_State::Value const& state = m_multirotor_state;
    QASSERT(state.mode == stream::IMultirotor_State::Mode::FLY);

    ISticks const& sticks = input.get_sticks();
    if (input.get_mode_switch().is_pressed())
    {
        m_multirotor_commands.sticks.yaw = 0.5f;
        m_multirotor_commands.sticks.pitch = 0.5f;
        m_multirotor_commands.sticks.roll = 0.5f;
        m_multirotor_commands.sticks.throttle = sticks.get_throttle();
    }
    else
    {
        if (input.get_mode_switch().was_released())
        {
            if (sticks.get_roll() < 0.1f && sticks.get_pitch() < 0.1f)
            {
                QLOGI("DISARMED!!!");
                set_mode(input, stream::IMultirotor_Commands::Mode::IDLE);
                return;
            }

            if (sticks.get_roll() >= 0.4f && sticks.get_roll() <= 0.6f)
            {
                if (sticks.get_pitch() <= 0.1f)
                {
                    QLOGI("LANDING!!!");
                    set_mode(input, stream::IMultirotor_Commands::Mode::LAND);
                    return;
                }
                if (sticks.get_pitch() >= 0.9f)
                {
                    QLOGI("TAKE OFF!!!");
                    set_mode(input, stream::IMultirotor_Commands::Mode::TAKE_OFF);
                    return;
                }
            }
        }

        m_multirotor_commands.sticks.yaw = sticks.get_yaw();
        m_multirotor_commands.sticks.pitch = sticks.get_pitch();
        m_multirotor_commands.sticks.roll = sticks.get_roll();

        //only apply the throttle some time after the mode chang, to let the actuator settle
        if (Clock::now() - m_last_vertical_mode_change_tp > std::chrono::milliseconds(200))
        {
            if (m_multirotor_commands.vertical_mode == Vertical_Mode::THRUST)
            {
                input.get_stick_actuators().set_target_throttle(boost::none);
            }

            m_multirotor_commands.sticks.throttle = sticks.get_throttle();
        }
    }


    if (input.get_horizontal_mode_switch_up().was_released())
    {
        if (m_multirotor_commands.horizontal_mode < Horizontal_Mode::POSITION)
        {
            set_horizontal_mode(input, static_cast<Horizontal_Mode>(static_cast<int>(m_multirotor_commands.horizontal_mode) + 1));
        }
    }

    if (input.get_horizontal_mode_switch_down().was_released())
    {
        if (m_multirotor_commands.horizontal_mode > Horizontal_Mode::ANGLE_RATE)
        {
            set_horizontal_mode(input, static_cast<Horizontal_Mode>(static_cast<int>(m_multirotor_commands.horizontal_mode) - 1));
        }
    }

    if (input.get_vertical_mode_switch().was_released())
    {
        set_vertical_mode(input, m_multirotor_commands.vertical_mode == Vertical_Mode::THRUST ? Vertical_Mode::ALTITUDE : Vertical_Mode::THRUST);
    }

    if (input.get_yaw_mode_switch().was_released())
    {
        set_yaw_mode(input, m_multirotor_commands.yaw_mode == Yaw_Mode::ANGLE_RATE ? Yaw_Mode::ANGLE : Yaw_Mode::ANGLE_RATE);
    }

    if (input.get_return_home_switch().was_released())
    {
        QLOGI("RTH!!!");
        set_mode(input, stream::IMultirotor_Commands::Mode::RETURN_HOME);
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::process_mode_take_off(Input& input)
{
    stream::IMultirotor_State::Value const& state = m_multirotor_state;
    QASSERT(state.mode == stream::IMultirotor_State::Mode::TAKE_OFF);

    input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::LOW);
    input.get_stick_actuators().set_target_throttle(state.throttle);

    ISticks const& sticks = input.get_sticks();
    if (input.get_mode_switch().was_released())
    {
        QLOGI("CANCELLED TAKE OFF!!!");
        set_mode(input, stream::IMultirotor_Commands::Mode::FLY);
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::process_mode_return_home(Input& input)
{
    if (input.get_return_home_switch().was_released())
    {
        QLOGI("CANCELLED RTH!!!");
        set_mode(input, stream::IMultirotor_Commands::Mode::FLY);
        return;
    }

    ISticks const& sticks = input.get_sticks();
    m_multirotor_commands.sticks.yaw = sticks.get_yaw();

    if (input.get_yaw_mode_switch().was_released())
    {
        set_yaw_mode(input, m_multirotor_commands.yaw_mode == Yaw_Mode::ANGLE_RATE ? Yaw_Mode::ANGLE : Yaw_Mode::ANGLE_RATE);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::process_mode_land(Input& input)
{
    stream::IMultirotor_State::Value const& state = m_multirotor_state;
    QASSERT(state.mode == stream::IMultirotor_State::Mode::LAND);

    input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::LOW);
    input.get_stick_actuators().set_target_throttle(state.throttle);

    ISticks const& sticks = input.get_sticks();
    if (input.get_mode_switch().was_released())
    {
        QLOGI("CANCELLED LANDING!!!");
        set_mode(input, stream::IMultirotor_Commands::Mode::FLY);
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Fly_Menu_Page::render(Adafruit_GFX& display)
{
    Clock::time_point now = Clock::now();

    const Clock::duration k_mode_change_blink_duration = std::chrono::milliseconds(300);

    Comms& comms = m_hal.get_comms();

    stream::IMultirotor_State::Value const& state = m_multirotor_state;

    util::coordinates::LLA home_lla;
    math::trans3dd home_ecef_to_enu_transform;
    if (state.home_ecef_position)
    {
        home_lla = util::coordinates::ecef_to_lla(*state.home_ecef_position);
        home_ecef_to_enu_transform = util::coordinates::ecef_to_enu_transform(home_lla);
    }
    math::vec3f enu_position = math::vec3f(math::transform(home_ecef_to_enu_transform, state.ecef_position));

//    {
//        math::vec3f euler;
//        state.local_frame.get_as_euler_zxy(euler);

//        math::trans2df rot;
//        rot.set_rotation(math::anglef(euler.y));

//        float length = 20.f;
//        math::vec2f center(display.width() - length * 0.5f, 30);
//        math::vec2f dir = math::rotate(rot, math::vec2f(length * 0.5f, 0));
//        math::vec2f p0 = center - dir;
//        math::vec2f p1 = center + dir;
//        display.drawLine(p0.x, p0.y, p1.x, p1.y, 1);
//    }

    int16_t y = 0;

    //mode
    {
        bool inverted = false;
        const char* mode_str = "N/A";
        switch (state.mode)
        {
        case stream::IMultirotor_State::Mode::IDLE: mode_str = "IDLE"; inverted = state.home_ecef_position != boost::none; break;
        case stream::IMultirotor_State::Mode::TAKE_OFF: mode_str = "TAKE OFF"; break;
        case stream::IMultirotor_State::Mode::FLY: mode_str = "FLY"; break;
        case stream::IMultirotor_State::Mode::RETURN_HOME: mode_str = "RTH"; break;
        case stream::IMultirotor_State::Mode::LAND: mode_str = "LAND"; break;
        }

        bool blink = (now - m_last_mode_change_tp) < k_mode_change_blink_duration ||
                state.mode != m_multirotor_commands.mode;
        uint16_t color = inverted ? 0 : 1;
        uint16_t bg_color = !color;

        if (!blink || m_fast_blink_color == 0)
        {
            display.setTextColor(color, bg_color);
            display.setCursor(0, y);
            display.print(mode_str);
            display.setTextColor(1 ,0);
        }
    }

    //signal strength
    {
        const Clock::duration k_timeout_duration = std::chrono::seconds(2);
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
                    (now - comms.get_last_rx_tp()) >= k_timeout_duration;
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

            bool error = m_tx_strength < k_min_signal_strength;
            uint16_t color = error ? m_blink_color : 1;

            display.fillRect(x, line_y, static_cast<int16_t>(m_tx_strength * w), h, color);
            int16_t slow_x = x + static_cast<int16_t>(m_slow_tx_strength * w);
            display.drawLine(slow_x,     line_y, slow_x,     line_y + h - 1, 1);
            display.drawLine(slow_x + 1, line_y, slow_x + 1, line_y + h - 1, 0);
        }
    }

//    display.setCursor(display.width() - 72, 0);
//    display.printf("%d / %d", static_cast<int>(m_rx_strength * 100.f), static_cast<int>(m_tx_strength * 100.f));

    y += 11;
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

    y += 10;
    //display.drawFastHLine(0, y, display.width(), 1);

    y += 5;
    //vertical mode
    {
        const char* mode_str = "---";
        if (m_multirotor_state.mode == silk::stream::IMultirotor_State::Mode::FLY)
        {
            switch (m_multirotor_commands.vertical_mode)
            {
            case Vertical_Mode::ALTITUDE: mode_str = "ALT"; break;
            case Vertical_Mode::THRUST: mode_str = "THR"; break;
            }
        }

        bool blink = (now - m_last_vertical_mode_change_tp) < k_mode_change_blink_duration;
        uint16_t color = blink ? m_fast_blink_color : 1;

        display.setTextColor(color);
        display.setCursor(0, y);
        display.printf("V:%s", mode_str);
        display.setTextColor(1);
    }
    if (state.mode != silk::stream::IMultirotor_Commands::Mode::IDLE) //altitude
    {
        float altitude = enu_position.z;
        display.setCursor(display.width() - 80, y);
        display.printf("ALT:%.1fm", altitude);
    }
    else
    {
        display.setCursor(display.width() - 80, y);
        if (state.home_ecef_position.is_initialized() || m_blink_color == 0)
        {
            display.printf("ALT:%.1fm", home_lla.altitude);
        }
    }

    y += 9;
    //horizontal mode
    {
        const char* mode_str = "---";
        if (m_multirotor_state.mode == silk::stream::IMultirotor_State::Mode::FLY)
        {
            switch (m_multirotor_commands.horizontal_mode)
            {
            case Horizontal_Mode::ANGLE_RATE: mode_str = "RATE"; break;
            case Horizontal_Mode::ANGLE: mode_str = "ANG"; break;
            case Horizontal_Mode::POSITION: mode_str = "POS"; break;
            }
        }

        bool blink = (now - m_last_horizontal_mode_change_tp) < k_mode_change_blink_duration;
        uint16_t color = blink ? m_fast_blink_color : 1;

        display.setTextColor(color);
        display.setCursor(0, y);
        display.printf("H:%s", mode_str);
        display.setTextColor(1);
    }
    if (state.mode != silk::stream::IMultirotor_Commands::Mode::IDLE) //v speed
    {
        float speed = state.enu_velocity.z;
        display.setCursor(display.width() - 80, y);
        display.printf("VSP:%.1fm/s", speed);
    }
    else
    {
        display.setCursor(display.width() - 80, y);
        if (state.home_ecef_position.is_initialized() || m_blink_color == 0)
        {
            display.printf("LAT:%.5f", math::degrees(home_lla.latitude));
        }
    }

    y += 9;
    //yaw mode
    {
        const char* mode_str = "---";
        if (m_multirotor_state.mode == silk::stream::IMultirotor_State::Mode::FLY ||
            m_multirotor_state.mode == silk::stream::IMultirotor_State::Mode::RETURN_HOME)
        {
            switch (m_multirotor_commands.yaw_mode)
            {
            case Yaw_Mode::ANGLE_RATE: mode_str = "RATE"; break;
            case Yaw_Mode::ANGLE: mode_str = "ANG"; break;
            }
        }

        bool blink = (now - m_last_yaw_mode_change_tp) < k_mode_change_blink_duration;
        uint16_t color = blink ? m_fast_blink_color : 1;

        display.setTextColor(color);
        display.setCursor(0, y);
        display.printf("Y:%s", mode_str);
        display.setTextColor(1);
    }
    if (state.mode != silk::stream::IMultirotor_Commands::Mode::IDLE) //h speed
    {
        float speed = math::length(math::vec2f(state.enu_velocity));
        display.setCursor(display.width() - 80, y);
        display.printf("HSP:%.1fm/s", speed);
    }
    else
    {
        display.setCursor(display.width() - 80, y);
        if (state.home_ecef_position.is_initialized() || m_blink_color == 0)
        {
            display.printf("LON:%.5f", math::degrees(home_lla.longitude));
        }
    }

    y+= 10;

    //distance from home
    if (state.mode != silk::stream::IMultirotor_Commands::Mode::IDLE) //h speed
    {
        float d = math::length(math::vec2f(enu_position));
        display.setCursor(0, y);
        display.printf("DIST:%.1fm", d);
    }

    //camera stuff
    {
        const char* quality_str = m_camera_commands.quality == silk::stream::ICamera_Commands::Quality::HIGH ? "H" : "L";
        display.setCursor(display.width() - 16, y);
        display.printf("%s", quality_str);

        const char* recording_str = m_camera_commands.recording ? "R" : " ";
        display.setTextColor(m_blink_color);
        display.setCursor(display.width() - 8, y);
        display.printf("%s", recording_str);
        display.setTextColor(1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
