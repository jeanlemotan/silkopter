#include "stdafx.h"
#include "Move_Gizmo.h"

using namespace q;

static math::vec3f s_axes_vectors[3] = { math::vec3f(1.f, 0.f, 0.f), math::vec3f(0.f, 1.f, 0.f), math::vec3f(0.f, 0.f, 1.f) };
static uint32_t s_axes_colors[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
static math::quatf s_axes_quats[3] = { math::quatf::from_axis_y(1.570796f), math::quatf::from_axis_x(-1.570796f), math::quatf() };

Move_Gizmo::Move_Gizmo(float length)
    : m_length(length)
{

}

void Move_Gizmo::set_transform(math::trans3df const& transform)
{
    m_transform = transform;
}
void Move_Gizmo::render(Render_Context& context, math::vec2f const& pointer)
{
	using namespace q::draw;

	scene::Camera const& camera = context.camera;
	video::Material mat = context.materials.primitive;
	mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
	mat.get_render_state(0).set_depth_test(false);
	mat.get_render_state(0).set_depth_write(false);
	mat.get_render_state(0).set_culling(false);
	context.painter.set_material(mat);
	context.painter.set_post_clip_transform(math::trans3df());

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

	math::vec3f axest[3];
	math::vec3f axesm[3];
	math::vec3f axesm2[3];
    for (uint32_t i = 0; i < 3; i++)
	{
        axest[i] = math::transform(m_transform, s_axes_vectors[i]*length);
		axesm[i] = (axest[i] + center)*0.5f;
	}
    for (uint32_t i = 0; i < 3; i++)
	{
		int i2 = (i + 1)%3;
		axesm2[i] = (axest[i] + axest[i2])*0.5f;
	}

    if (m_override_axis_filter)
	{
        m_axis_filter = m_override_axis_filter.get();
	}
	else
	{
		math::line3f ray = camera.get_ray_from_screen_coordinates(pointer);
		//compute selected axis
        m_axis_filter.set(0, 0, 0);
		float thresholdSq = math::square(length * 0.1f);
        for (uint32_t i = 0; i < 3; i++)
		{
			auto distSq = math::distance_sq(ray, math::line3f(center, axest[i]));
			if (distSq < thresholdSq)
			{
                m_axis_filter[i] = 1;
				break;
			}
		}
	}

    for (uint32_t i = 0; i < 3; i++)
	{
		math::vec3f p0(center);
		math::vec3f p1(axest[i]);
        uint32_t color = m_axis_filter[i] > 0 ? 0xFFFFFF00 : s_axes_colors[i];
		context.painter.draw_line(Vertex(p0, color), Vertex(p1, color));

		int ip = ((int)i - 1) < 0 ? 2 : (i - 1);

		p0 = axesm[i];
		p1 = axesm2[ip];
		context.painter.draw_line(Vertex(p0, color), Vertex(p1, color));

		p0 = axesm[i];
		p1 = axesm2[i];
		context.painter.draw_line(Vertex(p0, color), Vertex(p1, color));
	}

	mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
	context.painter.set_material(mat);
    for (uint32_t i = 0; i < 3; i++)
	{
        uint32_t color = 0x66999999;
		int in = (i + 1)%3;

		context.painter.fill_quad(Vertex(center, color), Vertex(axesm[i], color), Vertex(axesm2[i], color), Vertex(axesm[in], color));
	}

    for (uint32_t i = 0; i < 3; i++)
	{
        math::trans3df trans, t;
        t.set_rotation(s_axes_quats[i].get_as_mat3());
        math::multiply(trans, m_transform, t);
		context.painter.set_post_clip_transform(trans);
        context.painter.fill_cone(Vertex(math::vec3f(0, 0, 1)*length, s_axes_colors[i]), length * 0.05f, length * 0.15f);
	}
	context.painter.set_post_clip_transform(math::trans3df());
}

math::vec3f Move_Gizmo::get_axis_filter() const
{
    return m_axis_filter;
}

void Move_Gizmo::set_override_axis_filter(boost::optional<math::vec3f> const& axisFilter)
{
    m_override_axis_filter = axisFilter;
}
