#pragma once

#include "Render_State.h"

namespace q
{
namespace video
{

	class Render_State_Def
	{
	public:
        Render_State_Def() = default;
		Render_State_Def(Render_State_Def&& other);
		Render_State_Def(bool blending_locked
						, bool culling_locked
						, bool depth_locked
						, Render_State const& def);

        Render_State_Def(Render_State_Def const&) = default;
        Render_State_Def& operator=(Render_State_Def const&) = default;

		bool				is_blending_locked() const;
		bool				is_culling_locked() const;
		bool				is_depth_locked() const;
		Render_State const&	get_default_value() const;

	private:
        bool				m_is_blending_locked = false;
        bool				m_is_culling_locked = false;
        bool				m_is_deph_locked = false;
		Render_State		m_default;
	};

	inline Render_State_Def::Render_State_Def(Render_State_Def&& other)
		: m_is_blending_locked(other.m_is_blending_locked)
		, m_is_culling_locked(other.m_is_culling_locked)
		, m_is_deph_locked(other.m_is_deph_locked)
		, m_default(std::move(other.m_default))
	{
	}
	inline Render_State_Def::Render_State_Def(bool blending_locked
										, bool culling_locked
										, bool depth_locked
										, Render_State const& def)
		: m_is_blending_locked(blending_locked)
		, m_is_culling_locked(culling_locked)
		, m_is_deph_locked(depth_locked)
		, m_default(def)
	{
		;
	}
	inline bool Render_State_Def::is_blending_locked() const
	{
		return m_is_blending_locked;
	}
	inline bool Render_State_Def::is_culling_locked() const
	{
		return m_is_culling_locked;
	}
	inline bool Render_State_Def::is_depth_locked() const
	{
		return m_is_deph_locked;
	}
	inline Render_State const& Render_State_Def::get_default_value() const
	{
		return m_default;
	}

}
}
