#include "stdafx.h"
#include "Select_Gizmo.h"

using namespace q;

static math::vec3f s_axes_vectors[3] = { math::vec3f(1.f, 0.f, 0.f), math::vec3f(0.f, 1.f, 0.f), math::vec3f(0.f, 0.f, 1.f) };
static uint32_t s_axes_colors[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
static math::quatf s_axes_quats[3] = { math::quat_axis_y(1.570796f), math::quat_axis_x(-1.570796f), math::quatf() };

Select_Gizmo::Select_Gizmo(float length)
    : m_length(length)
{

}

void Select_Gizmo::set_transform(math::trans3df const& transform)
{
    m_transform = transform;
}
void Select_Gizmo::render(Render_Context& context)
{
	using namespace draw;

	scene::Camera& camera = context.camera;
	video::Material mat = context.materials.primitive;
	mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
	mat.get_render_state(0).set_depth_test(false);
	context.painter.set_material(mat);
	context.painter.set_post_clip_transform(math::trans3df::identity);

    math::vec3f center = m_transform.get_translation();

    float length = m_length;
	{
		math::vec3f vec1;
		camera.project(vec1, center);
		math::vec3f vec2(0, 0, vec1.z);
		math::vec3f vec3(length, 0, vec1.z);
		camera.unproject(vec2, vec2);
		camera.unproject(vec3, vec3);
		length = math::distance(vec3, vec2);
	}

    for (uint32_t i = 0; i < 3; i++)
	{
        math::vec3f p = math::transform(m_transform, s_axes_vectors[i]*length);
		if (math::equals(center, p))
		{
			continue;
		}
        context.painter.draw_line(Vertex(center, s_axes_colors[i]), Vertex(p, s_axes_colors[i]));
	}
    for (uint32_t i = 0; i < 3; i++)
	{
        math::trans3df trans, t;
        t.set_rotation(s_axes_quats[i].get_as_mat3());
        math::multiply(trans, m_transform, t);
		context.painter.set_post_clip_transform(trans);
        context.painter.fill_cone(Vertex(math::vec3f(0, 0, 1)*length, s_axes_colors[i]), length * 0.05f, length * 0.15f);
	}
	context.painter.set_post_clip_transform(math::trans3df::identity);
}

