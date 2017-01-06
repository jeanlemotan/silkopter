#pragma once

#include "IMenu_Page.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"


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
    silk::stream::IMultirotor_Commands::Value m_commands;
    Comms& m_comms;
    stream::IMultirotor_State::Value m_multirotor_state;

    float m_rx_strength = 0.f;
    float m_slow_rx_strength = 0.f;

    float m_tx_strength = 0.f;
    float m_slow_tx_strength = 0.f;

    q::Clock::time_point m_last_tp = q::Clock::now();

    q::Clock::time_point m_last_blink_tp = q::Clock::now();
    uint16_t m_blink_color = 0;
};

}
