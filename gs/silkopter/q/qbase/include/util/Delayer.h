#pragma once


namespace q
{
namespace util
{

	template<class T>
	auto lerp(T const& a, T const& b, T t) -> T
	{
		return a*(T(1) - t) + b*t;
	}

	template<class Tween, class Rep>
	class Delayer
	{
	public:
		Delayer(Rep const& start = Rep(0), Rep const& target = Rep(0), Duration duration = Duration(0));

		auto get_value() const -> Rep;
		auto get_target() const -> Rep;
		auto get_duration() const -> Duration;

		void reset(Rep const& target);
		void reset(Rep const& target, Duration duration);
		void reset(Rep const& start, Rep const& target, Duration duration);

		void rewind();
		auto is_done() const -> bool;

		auto update(Duration dt) -> bool;

	private:

		Rep m_value;
		Rep m_start;
		Rep m_target;
		Duration m_duration;
		Duration m_time;
	};


	template<class Tween, class Rep>
	Delayer<Tween, Rep>::Delayer(Rep const& start, Rep const& target, Duration duration)
		: m_value(start)
		, m_start(start)
		, m_target(target)
		, m_duration(duration)
		, m_time(0)
	{
	}

	template<class Tween, class Rep>
	auto Delayer<Tween, Rep>::get_value() const -> Rep
	{
		return m_value;
	}
	template<class Tween, class Rep>
	auto Delayer<Tween, Rep>::get_target() const -> Rep
	{
		return m_target;
	}
	template<class Tween, class Rep>
	auto Delayer<Tween, Rep>::get_duration() const -> Duration
	{
		return m_duration;
	}

	template<class Tween, class Rep>
	void Delayer<Tween, Rep>::reset(Rep const& start, Rep const& target, Duration duration)
	{
		m_start = start;
		m_value = start;
		m_target = target;
		m_duration = duration;
		m_time = Duration(0);
		if (is_done())
		{
			m_value = m_target;
		}
	}

	template<class Tween, class Rep>
	void Delayer<Tween, Rep>::reset(Rep const& target, Duration duration)
	{
		m_start = m_value;
		m_target = target;
		m_duration = duration;
		m_time = Duration(0);
		if (is_done())
		{
			m_value = m_target;
		}
	}

	template<class Tween, class Rep>
	void Delayer<Tween, Rep>::reset(Rep const& target)
	{
		m_start = m_value;
		m_target = target;
		m_time = Duration(0);
		if (is_done())
		{
			m_value = m_target;
		}
	}

	template<class Tween, class Rep>
	void Delayer<Tween, Rep>::rewind()
	{
		m_value = m_start;
		m_time = Duration(0);
		if (is_done())
		{
			m_value = m_target;
		}
	}

	template<class Tween, class Rep>
	auto Delayer<Tween, Rep>::is_done() const -> bool
	{
		return m_time >= m_duration;
	}

	template<class Tween, class Rep>
	auto Delayer<Tween, Rep>::update(Duration dt) -> bool
	{
		if (is_done())
		{
			return true;
		}
		QASSERT(m_duration.count() > 0);
		m_time = std::min(m_time + dt, m_duration);

		typedef typename Tween::value_t Ratio;

		Ratio t = Ratio(m_time.count()) / Ratio(m_duration.count());
		t = Tween()(t);

		m_value = lerp(m_start, m_target, t);

		return is_done();
	}

}
}

