#pragma once

namespace silk
{

    //This clock represents the epoch of the remote side.
    //For the Ground Station it represents the UAV clock and vice-versa
    //It can be used to compare time_points sent through the comms channel

    class Manual_Clock
    {
    public:
        typedef q::Clock::rep							rep;
        typedef q::Clock::period						period;
        typedef q::Clock::duration                      duration;
        typedef q::Clock::time_point                    time_point;
        const bool is_steady = q::Clock::is_steady;

        //this returns the last 'now' that was set into the clock
        auto now() const -> time_point;

        //this returns the current approximated 'now' of the remote clock. It's based on the last 'now' adjusted for realtime
        auto now_rt() const -> time_point;

        void advance(duration dt);

        //this is called when the comms are estalished
        void set_epoch(time_point now);
    private:
        time_point m_epoch = time_point{duration{0}};
        duration m_delta = duration{0};
        q::Clock::time_point m_rt_epoch;
    };

    inline auto Manual_Clock::now() const -> time_point
    {
        return m_epoch + m_delta;
    }
    inline auto Manual_Clock::now_rt() const -> time_point
    {
        return m_epoch + (q::Clock::now() - m_rt_epoch);
    }
    inline void Manual_Clock::set_epoch(time_point now)
    {
        m_delta = duration{0};
        m_epoch = now;
        m_rt_epoch = q::Clock::now();
    }
    inline void Manual_Clock::advance(duration dt)
    {
        m_delta += dt;
    }
}
