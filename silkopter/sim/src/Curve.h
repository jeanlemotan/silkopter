#pragma once


template <class T>
class Curve
{
public:

	void add_point(float mu, T const& value)
	{
		mu = math::clamp(mu, 0.f, 1.f);
		auto it = std::lower_bound(m_points.begin(), m_points.end(), mu, [](Point const& p, float mu) { return p.first < mu; });
		if (it == m_points.end())
		{
			m_points.push_back(Point(mu, value));
		}
		else if (it->first == mu)
		{
			it->second = value;
		}
		else
		{
			m_points.insert(it, Point(mu, value));
		}
	}
	T get_value(float mu) const
	{
		mu = math::clamp(mu, 0.f, 1.f);
		auto it = std::lower_bound(m_points.begin(), m_points.end(), mu, [](Point const& p, float mu) { return p.first < mu; });
		if (it == m_points.end())
		{
			return T();
		}
		else if (it == m_points.begin())
		{
			return it->second;
		}
		else
		{
			float d = it->first - (it - 1)->first;
			float r = (mu - (it - 1)->first) / d;
			QASSERT(r >= 0 && r <= 1.f);
			return math::lerp((it - 1)->second, it->second, r);
		}
		return T();
	}

private:
	typedef std::pair<float, T> Point;
	std::vector<Point> m_points;
};
