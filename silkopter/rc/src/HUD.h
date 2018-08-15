#pragma once

#include "Comms.h"

namespace silk
{
///////////////////////////////////////////////////////////////////////////////////////////////////

class IHAL;

class HUD
{
public:
    HUD(silk::IHAL& hal);

    void draw();

private:
    void draw_attitude(const stream::IMultirotor_State::Value& state);
    void draw_altitude(const stream::IMultirotor_State::Value& state);
    void draw_modes(const stream::IMultirotor_State::Value& state);

    silk::IHAL& m_hal;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
}
