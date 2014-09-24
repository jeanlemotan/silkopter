#include "stdafx.h"
#include "Rotate_Gizmo.h"

using namespace q;

static math::vec3f s_axes_vectors[3] = { math::vec3f(1.f, 0.f, 0.f), math::vec3f(0.f, 1.f, 0.f), math::vec3f(0.f, 0.f, 1.f) };
static uint32_t s_axes_colors[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
static math::quatf s_axes_quats[3] = { math::quatf::from_axis_y(1.570796f), math::quatf::from_axis_x(-1.570796f), math::quatf() };

Rotate_Gizmo::Rotate_Gizmo(float radius)
    : m_radius(radius)
    , m_select_gizmo(radius * 0.3f)
{

}

void Rotate_Gizmo::set_transform(math::trans3df const& transform)
{
    m_transform = transform;
    m_select_gizmo.set_transform(transform);
}
void Rotate_Gizmo::render(Render_Context& context, math::vec2f const& pointer)
{
	using namespace q::draw;

    m_select_gizmo.render(context);

	scene::Camera const& camera = context.camera;
	video::Material mat = context.materials.primitive;
	mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
	mat.get_render_state(0).set_depth_test(false);
	mat.get_render_state(0).set_depth_write(false);
	context.painter.set_post_clip_transform(math::trans3df::identity);

    math::vec3f center = m_transform.get_translation();
	math::vec3f camFront = camera.get_front_vector();

    float radius = m_radius;
	{
		math::vec3f vec1;
		camera.project(vec1, center);
		math::vec3f vec2(0, 0, vec1.z);
		math::vec3f vec3(radius, 0, vec1.z);
		camera.unproject(vec2, vec2);
		camera.unproject(vec3, vec3);
		radius = math::distance(vec3, vec2);
	}

	mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
	mat.get_render_state(0).set_culling(true);
	context.painter.set_material(mat);
    context.painter.fill_sphere(Vertex(center, 0x66999999), radius, 4);

	mat.get_render_state(0).set_culling(false);
	context.painter.set_material(mat);

    math::quatf rot;
    rot.set_from_mat3(m_transform.get_rotation());

	const int k_divisions = 70;
    const float step = math::anglef::_2pi / (float)k_divisions;

	std::vector<math::line3f> segments[3];
	segments[0].reserve(500);
	segments[1].reserve(500);
	segments[2].reserve(500);

	//compute the segments
    for (uint32_t k = 0; k < 3; k++)
	{
        math::quatf q = rot * s_axes_quats[k];

        math::trans3df mat;
        mat.set_rotation(q.get_as_mat3());
        for (uint32_t i = 0; i < k_divisions + 1; i++)
		{
			float x1 = (float)(math::cos(step * i) * radius);	
			float y1 = (float)(math::sin(step * i) * radius);	
			float x2 = (float)(math::cos(step * (i + 1)) * radius);	
			float y2 = (float)(math::sin(step * (i + 1)) * radius);	

			math::vec3f p0(x1, y1, 0);
			math::vec3f p1(x2, y2, 0);
			p0 = math::transform(mat, p0);
			p1 = math::transform(mat, p1);

			p0 += center;
			p1 += center;

			segments[k].push_back(math::line3f(p0, p1));
		}
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
        float thresholdSq = math::square(radius * 0.1f);
        for (uint32_t k = 0; k < 3; k++)
		{
            for (uint32_t i = 0; i < segments[k].size(); i++)
			{
				math::line3f const& line = segments[k][i];
                auto distSq = math::distance_sq(ray, line);
				math::vec3f dir = center - line.end;
                if (distSq <= thresholdSq && math::dot(dir, camFront) > 0.f)
				{
                    m_axis_filter[k] = 1;

					math::vec2f center, p0, p1;
					camera.project(center, line.get_middle());
					camera.project(p0, line.start - line.get_vector_normalized() * radius);
					camera.project(p1, line.end + line.get_vector_normalized() * radius);

					if (math::equals(p0, p1))
					{
                        m_tangent.start = m_tangent.end = math::vec2f();
					}
					else
					{
						math::vec2f dir = math::normalized(p1 - p0);
                        m_tangent.start = center - dir * 1000.f;
                        m_tangent.end = center + dir * 1000.f;
					}

					k = 999;
					break;
				}
			}
		}
	}

	//draw the segments
    for (uint32_t k = 0; k < 3; k++)
	{
        math::vec4f color = m_axis_filter[k] > 0 ? math::vec4f(1, 1, 0, 1) : math::color::u32_to_rgba<float>(s_axes_colors[k]);
        for (uint32_t i = 0; i < segments[k].size(); i++)
		{
			math::line3f const& line = segments[k][i];
			math::vec3f dir = math::normalized(center - line.end);
			float alpha = math::clamp(math::dot(dir, camFront) / 0.2f, 0.05f, 1.f);
			if (alpha > 0.f)
			{
                uint32_t c = math::color::rgba_to_u32(math::vec4f(color.x, color.y, color.z, color.w * alpha));
				Vertex a(line.start, c);
				Vertex b(line.end, c);
				context.painter.draw_line(a, b);
			}
		}
	}

//	context.painter.setMaterial(context.materials.primitive2D);
//	Vertex a(mTangent.start, 0xFF00FF00);
//	Vertex b(mTangent.end, 0xFF00FF00);
//	context.painter.drawLine(a, b);
}

math::vec3f Rotate_Gizmo::get_axis_filter() const
{
    return m_axis_filter;
}

void Rotate_Gizmo::set_override_axis_filter(boost::optional<math::vec3f> const& axisFilter)
{
    m_override_axis_filter = axisFilter;
}

math::line2f Rotate_Gizmo::get_tangent() const
{
    return m_tangent;
}

