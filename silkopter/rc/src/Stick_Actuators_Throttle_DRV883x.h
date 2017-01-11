#pragma once

#include "ISticks.h"
#include "IHaptic.h"
#include "IStick_Actuators.h"
#include "utils/PID.h"

namespace silk
{

class Stick_Actuators_Throttle_DRV883x : public IStick_Actuators, public IHaptic
{
public:
    Stick_Actuators_Throttle_DRV883x(ISticks const& sticks);
    ~Stick_Actuators_Throttle_DRV883x();

    ts::Result<void> init();

    void vibrate(std::vector<Note> const& notes) override;

    bool set_target_yaw(boost::optional<float> value) override;
    bool set_target_pitch(boost::optional<float> value) override;
    bool set_target_roll(boost::optional<float> value) override;
    bool set_target_throttle(boost::optional<float> value) override;
    void process() override;

//    void set_p(float a, float b);

private:
    ts::Result<void> detect_direction();
    void set_enabled(bool yes);

    ISticks const& m_sticks;

    typedef util::PID<float, float, float> PID;
    PID m_throttle_pid;

    boost::optional<float> m_target_throttle;

    uint8_t m_enable_gpio = 0;
    uint8_t m_phase_gpio = 0;

    uint8_t m_enabled_ref_count = 0;
    std::vector<Note> m_haptic_notes;
    q::Clock::time_point m_last_note_tp = q::Clock::now();

    q::Clock::time_point m_last_tp = q::Clock::now();
};

}
