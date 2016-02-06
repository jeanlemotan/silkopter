#pragma once

#include <stdint.h>
#include <util/delay.h>

extern unsigned long millis();

namespace chrono
{
	struct millis;
	struct micros;
    struct seconds;
	struct secondsf;

	template<class Rep, class T>
	struct duration
	{
		typedef Rep rep_t;
		rep_t count;

		inline duration() : count(0) {}
        inline explicit duration(rep_t count) : count(count) {}
		inline duration(duration<Rep, T> const& other) = default;
		inline duration& operator=(duration<Rep, T> const& other) { count = other.count; return *this; }
		inline T operator-(T const& other) const { T x(count - other.count); return x; }
		inline T operator+(T const& other) const { T x(count + other.count); return x; }
		inline duration<Rep, T>& operator-=(T const& other) { count -= other.count; return *this; }
		inline duration<Rep, T>& operator+=(T const& other) { count += other.count; return *this; }
		inline bool operator<(T const& other) const { return count < other.count; }
		inline bool operator<=(T const& other) const { return count <= other.count; }
		inline bool operator>(T const& other) const { return count > other.count; }
		inline bool operator>=(T const& other) const { return count >= other.count; }
		inline bool operator==(T const& other) const { return count == other.count; }
		inline bool operator!=(T const& other) const { return !operator==(other); }
	};

	struct millis : public duration<int32_t, millis>
	{
		inline millis() = default;
        inline explicit millis(micros const& other);
        inline explicit millis(secondsf const& other);
        inline explicit millis(seconds const& other);
        inline explicit millis(rep_t count) : duration<int32_t, millis>(count) {}
	};

	struct micros : public duration<int32_t, micros>
	{
		inline micros() = default;
        inline explicit micros(millis const& other) : duration<int32_t, micros>(other.count * 1000) {}
        inline explicit micros(secondsf const& other);
        inline explicit micros(seconds const& other);
        inline explicit micros(rep_t count) : duration<int32_t, micros>(count) {}
	};

    struct seconds : public duration<int32_t, seconds>
    {
        inline seconds() = default;
        inline explicit seconds(millis const& other) : duration<int32_t, seconds>(other.count * 1000) {}
        inline explicit seconds(micros const& other) : duration<int32_t, seconds>(other.count * 1000000) {}
        inline explicit seconds(secondsf const& other);
        inline explicit seconds(rep_t count) : duration<int32_t, seconds>(count) {}
    };

	struct secondsf : public duration<float, secondsf>
	{
		inline secondsf() = default;
        inline explicit secondsf(millis const& other) : duration<float, secondsf>(other.count * 0.001f) {}
        inline explicit secondsf(micros const& other) : duration<float, secondsf>(other.count * 0.000001f) {}
        inline explicit secondsf(seconds const& other) : duration<float, secondsf>(other.count) {}
        inline explicit secondsf(rep_t count) : duration<float, secondsf>(count) {}
	};

	inline millis::millis(micros const& other) : duration<int32_t, millis>(other.count / 1000) {}
	inline millis::millis(secondsf const& other) : duration<int32_t, millis>(other.count * 1000.f) {}
    inline millis::millis(seconds const& other) : duration<int32_t, millis>(other.count * 1000) {}
	inline micros::micros(secondsf const& other) : duration<int32_t, micros>(other.count * 1000000.f) {}
    inline micros::micros(seconds const& other) : duration<int32_t, micros>(other.count * 1000000) {}
    inline seconds::seconds(secondsf const& other) : duration<int32_t, seconds>(other.count) {}

	template<class Rep, class Duration>
	struct time
	{
		typedef Rep rep_t;
		typedef Duration duration_t;
		typedef time<Rep, Duration> time_t;
		rep_t ticks;

		inline time() : ticks(0) {}
        inline explicit time(rep_t ticks) : ticks(ticks) {}
		inline time(time_t const& other) : ticks(other.ticks) {}
		inline time_t& operator=(time_t const& other) { ticks = other.ticks; return *this; }
		inline time_t& operator=(volatile time_t const& other) { ticks = other.ticks; return *this; }
		inline time_t operator-(duration_t const& other) const { time_t x(ticks - other.count); return x; }
		inline time_t operator+(duration_t const& other) const { time_t x(ticks + other.count); return x; }
		inline time_t& operator-=(duration_t const& other) { ticks -= other.count; return *this; }
		inline time_t& operator+=(duration_t const& other) { ticks += other.count; return *this; }
		inline duration_t operator-(time_t const& other) const { duration_t x(ticks - other.ticks); return x; }
		inline duration_t operator-(volatile time_t const& other) const { duration_t x(ticks - other.ticks); return x; }
		inline bool operator<(time_t const& other) const { return ticks < other.ticks; }
		inline bool operator<=(time_t const& other) const { return ticks <= other.ticks; }
		inline bool operator>(time_t const& other) const { return ticks > other.ticks; }
		inline bool operator>=(time_t const& other) const { return ticks >= other.ticks; }
		inline bool operator==(time_t const& other) const { return ticks == other.ticks; }
		inline bool operator!=(time_t const& other) const { return !operator==(other); }

		inline Duration time_since_epoch() const { return Duration(static_cast<typename Duration::rep_t>(ticks)); }
	};

    typedef time<uint64_t, millis> time_ms;
	typedef time<uint64_t, micros> time_us;
    typedef time<uint32_t, seconds> time_s;


    template<class D>
    inline void delay(D duration)
    {
        millis ms(duration);
        _delay_ms(ms.count);
    }


    extern time_ms s_time_point;

    inline time_ms now()
    {
        static uint32_t s_last_millis = ::millis();
        uint32_t ms = ::millis();
        if (ms > s_last_millis)
        {
            s_time_point += millis(ms - s_last_millis);
        }
        s_last_millis = ms;
        return s_time_point;
    }
}
