#pragma once

namespace q
{
namespace util
{

	class Rand
	{
	public:
		Rand();
		Rand(int seed);

		int		get();
		int		get(int lower_bound, int upper_bound);

		float	get_float();
		float	get_float(float lower_bound, float upper_bound);
		float	get_positive_float();

	private:
		int64_t m_rnd_x;
		int64_t m_rnd_y;
		int64_t m_rnd_z;
		int64_t m_rnd_w;
		uint32_t	m_seed_float;
		static uint32_t s_extra_seed;
	};


	inline Rand::Rand(int seed)
	{
		m_rnd_x = seed;
		m_rnd_y = 842502087u;
		m_rnd_z = 3579807591u;
		m_rnd_w = 273326509u;
		m_seed_float = seed;
	}

	//////////////////////////////////////////////////////////////////////////

	inline Rand::Rand()
	{
		m_rnd_x = Clock::now().time_since_epoch().count() + s_extra_seed;
		m_rnd_y = 842502087u;
		m_rnd_z = 3579807591u;
		m_rnd_w = 273326509u;
		m_seed_float = (uint32_t)m_rnd_x;
		s_extra_seed *= 126509U;
		s_extra_seed ^= (s_extra_seed << 7);
	}

	//////////////////////////////////////////////////////////////////////////

	inline int Rand::get()
	{
		int64_t t	=(m_rnd_x^(m_rnd_x<<11));
		m_rnd_x	= m_rnd_y; m_rnd_y = m_rnd_z; m_rnd_z = m_rnd_w;
		m_rnd_w	=(m_rnd_w^(m_rnd_w>>19))^(t^(t>>8));

		int64_t rtn = m_rnd_w&0x7FFFFFFF;
		return rtn == 0x7FFFFFFF ? get() : (int)rtn;
	}

	//////////////////////////////////////////////////////////////////////////

	inline int Rand::get(int lowerBound, int upperBound)
	{
		if (lowerBound > upperBound)
		{
			std::swap(lowerBound, upperBound);
		}

		int64_t rnd = get() >> 2;
		int range = upperBound - lowerBound;
		return range ? (lowerBound + ((int)(rnd&0x7FFFFFFF) % range)) : lowerBound;
	}

	//////////////////////////////////////////////////////////////////////////

	inline float Rand::get_float()
	{
		m_seed_float *= 16807;
		uint32_t a = (m_seed_float & 0x007fffff) | 0x40000000;
		return (*((float*)&a) - 3.0f);
	}

	//////////////////////////////////////////////////////////////////////////

	inline float Rand::get_positive_float()
	{
		return std::abs(get_float());
	}

	//////////////////////////////////////////////////////////////////////////

	inline float Rand::get_float(float lowerBound, float upperBound)
	{
		if (lowerBound > upperBound)
		{
			std::swap(lowerBound, upperBound);
		}

		float diff = upperBound - lowerBound;
		return lowerBound + (get_positive_float() * diff);
	}

} //util
} //q