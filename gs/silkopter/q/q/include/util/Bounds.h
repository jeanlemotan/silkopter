#pragma once

namespace q
{
namespace util
{


	class Bounds
	{
	public:
		Bounds();
		Bounds(math::vec3f const& center, float radius);

		void				set_center(math::vec3f const& center);
		math::vec3f const&	get_center() const;

		void				set_radius(float radius);
		float				get_radius() const;

	private:
		math::vec3f			m_center;
		float				m_radius;
	};

	inline Bounds::Bounds()
		: m_radius(0.f)
	{
		;
	}
	inline Bounds::Bounds(math::vec3f const& center, float radius)
		: m_center(center)
		, m_radius(radius)
	{
		;
	}

	inline void Bounds::set_center(math::vec3f const& center)
	{
		m_center = center;
	}
	inline math::vec3f const& Bounds::get_center() const
	{
		return m_center;
	}

	inline void Bounds::set_radius(float radius)
	{
		m_radius = radius;
	}
	inline float Bounds::get_radius() const
	{
		return m_radius;
	}
		
}
}