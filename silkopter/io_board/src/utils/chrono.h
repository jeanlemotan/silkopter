#pragma once

#include <stdint.h>

namespace chrono
{
	struct millis;
	struct micros;
	struct secondsf;
	
	template<class Rep, class T>
	struct duration
	{
		typedef Rep rep_t;
		rep_t count;
		
		inline duration() : count(0) {}
		inline duration(rep_t count) : count(count) {}
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
		inline millis(micros const& other);
		inline millis(secondsf const& other);
		inline millis(rep_t count) : duration<int32_t, millis>(count) {}
	};
	
	struct micros : public duration<int32_t, micros>
	{
		inline micros() = default;
		inline micros(millis const& other) : duration<int32_t, micros>(other.count * 1000) {}
		inline micros(secondsf const& other);
		inline micros(rep_t count) : duration<int32_t, micros>(count) {}
	};

	struct secondsf : public duration<float, secondsf>
	{
		inline secondsf() = default;
		inline secondsf(millis const& other) : duration<float, secondsf>(other.count * 0.001f) {}
		inline secondsf(micros const& other) : duration<float, secondsf>(other.count * 0.000001f) {}
		inline secondsf(rep_t count) : duration<float, secondsf>(count) {}
	};
	
	inline millis::millis(micros const& other) : duration<int32_t, millis>(other.count / 1000) {}
	inline millis::millis(secondsf const& other) : duration<int32_t, millis>(static_cast<int32_t>(other.count * 1000.f)) {}
	inline micros::micros(secondsf const& other) : duration<int32_t, micros>(static_cast<int32_t>(other.count * 1000000.f)) {}
	
	template<class Rep, class Duration>
	struct time
	{
		typedef Rep rep_t;
		typedef Duration duration_t;
		typedef time<Rep, Duration> time_t;
		rep_t ticks;
		
		inline time() : ticks(0) {}
		inline time(rep_t ticks) : ticks(ticks) {}
		inline time(time_t const& other) : ticks(other.ticks) {}
		inline time_t& operator=(time_t const& other) { ticks = other.ticks; return *this; }
		inline time_t operator-(duration_t const& other) const { time_t x(ticks - other.count); return x; }
		inline time_t operator+(duration_t const& other) const { time_t x(ticks + other.count); return x; }
		inline time_t& operator-=(duration_t const& other) { ticks -= other.count; return *this; }
		inline time_t& operator+=(duration_t const& other) { ticks += other.count; return *this; }
		inline duration_t operator-(time_t const& other) const { duration_t x(static_cast<typename duration_t::rep_t>(ticks - other.ticks)); return x; }
		inline bool operator<(time_t const& other) const { return ticks < other.ticks; }
		inline bool operator<=(time_t const& other) const { return ticks <= other.ticks; }
		inline bool operator>(time_t const& other) const { return ticks > other.ticks; }
		inline bool operator>=(time_t const& other) const { return ticks >= other.ticks; }
		inline bool operator==(time_t const& other) const { return ticks == other.ticks; }
		inline bool operator!=(time_t const& other) const { return !operator==(other); }

		inline Duration time_since_epoch() const { return Duration(static_cast<typename Duration::rep_t>(ticks)); }
	};
	
	typedef time<uint32_t, millis> time_ms;
	typedef time<uint64_t, micros> time_us;
	
}

namespace q
{
	namespace util
	{
		namespace formatting
		{
			template<class Dst_Adapter, class Placeholder, class Rep, class Duration>
			void format_string(Dst_Adapter& dst, Placeholder const& ph, chrono::time<Rep, Duration> const& time)
			{
				format_string(dst, ph, time.time_since_epoch());
			}
			template<class Dst_Adapter, class Placeholder>
			void format_string(Dst_Adapter& dst, Placeholder const& ph, chrono::micros us)
			{
				format_string(dst, ph, us.count);
				format_string(dst, ph, "us");
			}
			template<class Dst_Adapter, class Placeholder>
			void format_string(Dst_Adapter& dst, Placeholder const& ph, chrono::millis ms)
			{
				format_string(dst, ph, ms.count);
				format_string(dst, ph, "ms");
			}
			template<class Dst_Adapter, class Placeholder>
			void format_string(Dst_Adapter& dst, Placeholder const& ph, chrono::secondsf s)
			{
				format_string(dst, ph, s.count);
				format_string(dst, ph, "s");
			}
		}
	}
}

