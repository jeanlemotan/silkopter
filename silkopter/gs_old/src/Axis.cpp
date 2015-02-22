#include "stdafx.h"
#include "Axis.h"

using namespace q;

static const math::vec3f s_axes_vectors[3] = { math::vec3f(1.f, 0.f, 0.f), math::vec3f(0.f, 1.f, 0.f), math::vec3f(0.f, 0.f, 1.f) };
static const uint32_t s_axes_colors[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
static math::trans3df s_axes_transforms[3];
static bool s_is_initialized = false;

//////////////////////////////////////////////////////////////////////////

void render_arrow(math::vec3f const& start, math::vec3f const& end, q::video::Material const& material, uint32_t color, q::draw::Painter& painter)
{
    if (!s_is_initialized)
    {
        s_axes_transforms[0] = math::trans3df(math::vec3f::zero, math::quatf::from_axis_y(1.570796f).get_as_mat3(), math::vec3f::one);
        s_axes_transforms[1] = math::trans3df(math::vec3f::zero, math::quatf::from_axis_x(-1.570796f).get_as_mat3(), math::vec3f::one);
        s_is_initialized = true;
    }

    using namespace draw;

    video::Material mat = material;
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
/*	mat.get_render_state(0).set_depth_test(false);*/
    painter.set_material(mat);
    painter.set_pre_clip_transform(math::trans3df::identity);
    painter.set_post_clip_transform(math::trans3df::identity);

    painter.draw_line(Vertex(start, color), Vertex(end, color));

    float length = math::length(end - start);
    math::vec3f front = math::normalized(end - start);
    math::vec3f up = math::abs(math::dot(front, math::vec3f(0, 0, 1))) > 0.93 ? math::vec3f(0, 1, 0) : math::vec3f(0, 0, 1);
    math::mat3f rot;
    math::multiply(rot, math::mat3f::look_at(front, up), math::quatf::from_axis_x(-1.570796f).get_as_mat3());

    math::trans3df trans(end, rot, math::vec3f::one);
    painter.set_post_clip_transform(trans);
    painter.fill_cone(Vertex(math::vec3f::zero, color), length * 0.05f, length * 0.15f);
    painter.set_post_clip_transform(math::trans3df::identity);
}

//////////////////////////////////////////////////////////////////////////

void render_axes(q::draw::Painter& painter, float length)
{
    if (!s_is_initialized)
    {
        s_axes_transforms[0] = math::trans3df(math::vec3f::zero, math::quatf::from_axis_y(1.570796f).get_as_mat3(), math::vec3f::one);
        s_axes_transforms[1] = math::trans3df(math::vec3f::zero, math::quatf::from_axis_x(-1.570796f).get_as_mat3(), math::vec3f::one);
        s_is_initialized = true;
    }

    using namespace draw;

    for (size_t i = 0; i < 3; i++)
    {
        math::vec3f p = s_axes_vectors[i] * length;
        painter.draw_line(Vertex(math::vec3f::zero, s_axes_colors[i]), Vertex(p, s_axes_colors[i]));
    }
    for (size_t i = 0; i < 3; i++)
    {
        math::trans3df trans = s_axes_transforms[i];
        painter.push_post_clip_transform(trans);
        painter.fill_cone(Vertex(math::vec3f(0, 0, 1)*length, s_axes_colors[i]), length * 0.05f, length * 0.15f);
        painter.pop_post_clip_transform();
    }
}

