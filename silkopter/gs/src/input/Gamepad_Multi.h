#pragma once

#include "Multi.h"

namespace input
{

class Gamepad_Multi : public Multi
{
public:
    Gamepad_Multi(q::input::Input_Mgr& input_mgr);

    ~Gamepad_Multi();


    virtual auto get_input() const -> silk::node::stream::IMulti_Input::Value const& override;
    virtual void process()  override;

private:
    q::input::Input_Mgr& m_input_mgr;
    silk::node::stream::IMulti_Input::Value m_input;
};

}
