#pragma once

namespace chrono
{
	struct millis;
	struct micros;

	struct millis
	{
		typedef int32_t rep_t;
		rep_t count;
		
		inline millis() : count(0) {}
		inline millis(rep_t count) : count(count) {}
		inline millis(millis const& other) = default;
		inline millis(micros const& other);
		inline millis& operator=(millis const& other) { count = other.count; return *this; }
		inline millis operator-(millis const& other) const { millis x(count - other.count); return x; }
		inline millis operator+(millis const& other) const { millis x(count + other.count); return x; }
		inline millis& operator-=(millis const& other) { count -= other.count; return *this; }
		inline millis& operator+=(millis const& other) { count += other.count; return *this; }
		inline bool operator<(millis const& other) const { return count < other.count; }
		inline bool operator<=(millis const& other) const { return count <= other.count; }
		inline bool operator>(millis const& other) const { return count > other.count; }
		inline bool operator>=(millis const& other) const { return count >= other.count; }
		inline bool operator==(millis const& other) const { return count == other.count; }
		inline bool operator!=(millis const& other) const { return !operator==(other); }
	};
	
	struct micros
	{
		typedef int32_t rep_t;
		rep_t count;

		inline micros() : count(0) {}
		inline micros(rep_t count) : count(count) {}
		inline micros(micros const& other) = default;
		inline micros(millis const& other) : count(other.count * 1000) {}
		inline micros& operator=(micros const& other) { count = other.count; return *this; }
		inline micros operator-(micros const& other) const { micros x(count - other.count); return x; }
		inline micros operator+(micros const& other) const { micros x(count + other.count); return x; }
		inline micros& operator-=(micros const& other) { count -= other.count; return *this; }
		inline micros& operator+=(micros const& other) { count += other.count; return *this; }
		inline bool operator<(micros const& other) const { return count < other.count; }
		inline bool operator<=(micros const& other) const { return count <= other.count; }
		inline bool operator>(micros const& other) const { return count > other.count; }
		inline bool operator>=(micros const& other) const { return count >= other.count; }
		inline bool operator==(micros const& other) const { return count == other.count; }
		inline bool operator!=(micros const& other) const { return !operator==(other); }
	};
	
	inline millis::millis(micros const& other) : count(other.count / 1000) {}
	
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
	
	typedef time<uint32_t, millis> time_ms;
	typedef time<uint64_t, micros> time_us;
	
}
