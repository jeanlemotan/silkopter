#pragma once

#include "IMenu_Page.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "utils/Clock.h"

namespace silk
{

class Comms;

class Fly_Menu_Page : public IMenu_Page
{
public:
    Fly_Menu_Page(Comms& comms);

    bool process(Input& input, Menu_System& menu_system);
    void render(Adafruit_GFX& display);

private:
    void init(Input& input);
    void set_mode(Input& input, stream::IMultirotor_Commands::Mode mode);
    void set_vertical_mode(Input& input, stream::IMultirotor_Commands::Vertical_Mode mode);
    void set_horizontal_mode(Input& input, stream::IMultirotor_Commands::Horizontal_Mode mode);
    void set_yaw_mode(Input& input, stream::IMultirotor_Commands::Yaw_Mode mode);

    void process_mode_idle(Input& input);
    void process_mode_take_off(Input& input);
    void process_mode_fly(Input& input);
    void process_mode_return_home(Input& input);
    void process_mode_land(Input& input);

    silk::stream::IMultirotor_Commands::Value m_commands;
    Comms& m_comms;
    stream::IMultirotor_State::Value m_multirotor_state;

    bool m_is_initialized = false;

    float m_rx_strength = 0.f;
    float m_slow_rx_strength = 0.f;

    float m_tx_strength = 0.f;
    float m_slow_tx_strength = 0.f;

    Clock::time_point m_last_tp = Clock::now();

    Clock::time_point m_last_blink_tp = Clock::now();
    uint16_t m_blink_color = 0;
    Clock::time_point m_last_fast_blink_tp = Clock::now();
    uint16_t m_fast_blink_color = 0;

    Clock::time_point m_last_mode_change_tp = Clock::now();
    Clock::time_point m_last_vertical_mode_change_tp = Clock::now();
    Clock::time_point m_last_horizontal_mode_change_tp = Clock::now();
    Clock::time_point m_last_yaw_mode_change_tp = Clock::now();


    struct Idle_Mode_Data
    {
        bool is_pressed = false;
        Clock::time_point pressed_tp = Clock::now();
    } m_idle_mode_data;
};

}
