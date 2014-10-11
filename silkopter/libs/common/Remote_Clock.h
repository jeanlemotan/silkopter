#pragma once

namespace silk
{

    //This clock represents the epoch of the remote side.
    //For the Ground Station it represents the UAV clock and vice-versa
    //It can be used to compare time_points sent through the comms channel

    class Remote_Clock
    {
    public:
        typedef int64_t									rep;
        typedef std::micro								period;
        typedef std::chrono::duration<rep, period>      duration;
        typedef std::chrono::time_point<Remote_Clock>	time_point;
        const bool is_steady = true;

        //this returns the last 'now' that was set into the clock
        auto now() const -> time_point;

        //this returns the current approximated 'now' of the remote clock. It's based on the last 'now' adjusted for realtime
        auto now_rt() const -> time_point;

        //this is called when the comms are estalished
        void set_now(duration now);
        auto is_set() const -> bool;

        //call this to apply micro adjustments to the clock.
        // The adjusted value is applied in time as to never set the clock backwards. In the worst case the clock will stop at the same value for d amount of time
        //void set_offset(duration d);
    private:
        duration m_now;

        q::Clock::time_point m_rt_now;
    };

    inline auto Remote_Clock::now() const -> time_point
    {
        return time_point(m_now);
    }
    inline auto Remote_Clock::now_rt() const -> time_point
    {
        return time_point(std::chrono::microseconds(m_now) + std::chrono::microseconds(q::Clock::now() - m_rt_now));
    }
    inline void Remote_Clock::set_now(duration now)
    {
        m_now = now;
        m_rt_now = q::Clock::now();
    }
    inline auto Remote_Clock::is_set() const -> bool
    {
        return m_now.count() != 0;
    }
    // void Remote_Clock::set_offset(std::chrono::microseconds us)
    // {
    //     QASERT(0); 
    // }
}
