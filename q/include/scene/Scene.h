#pragma once

#include "Ptr_Fw_Declaration.h"
#include "Camera.h"

namespace q
{
namespace scene
{
	inline math::vec3f get_front_vector()
	{
		return math::vec3f(0, 1, 0);
	}
	inline math::vec3f get_up_vector()
	{
		return math::vec3f(0, 0, 1);
	}
	inline math::vec3f get_right_vector()
	{
		return math::vec3f(1, 0, 0);
	}

	//////////////////////////////////////////////////////////////////////////

	class Scene : public util::Noncopyable
	{
	public:
		Scene();

		void		set_camera(Camera const& camera);
		void		add(Scene_Node_ptr const& node);

		void		render();

	private:
		Camera		m_camera;
		std::vector<Scene_Node_ptr>	m_to_render;
	};

}
}
