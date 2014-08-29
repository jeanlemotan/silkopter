#pragma once

namespace q
{
namespace video
{

	class Post_Fx
	{
	public:
		void			set_material(Material const& material);
		Material const&	get_material() const;

	private:
		Material		m_material;
	};

	//////////////////////////////////////////////////////////////////////////

	inline Material const& Post_Fx::get_material() const
	{
		return m_material;
	}
	inline void Post_Fx::set_material(Material const& material)
	{
		m_material = material;
		for (size_t i = 0; i < m_material.get_pass_count(); i++)
		{
			auto& rs = m_material.get_render_state(i);
			rs.set_culling(false);
			rs.set_depth_test(false);
			rs.set_depth_write(false);
		}
	}

}
}
