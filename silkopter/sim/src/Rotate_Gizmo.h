#pragma once

#include "Select_Gizmo.h"

class Rotate_Gizmo
{
public:
	Rotate_Gizmo(float radius);

    void set_transform(math::trans3df const& transform);
    void render(Render_Context& context, math::vec2f const& pointer);

    math::vec3f get_axis_filter() const;
    void set_override_axis_filter(boost::optional<math::vec3f> const& axisFilter = boost::optional<math::vec3f>());

    math::line2f get_tangent() const;

private:
    float m_radius;
    math::trans3df m_transform;
    math::vec3f m_axis_ends[3];
    math::vec3f m_axis_middle[3];
    math::vec3f m_axis_middle2[3];
    math::vec3f m_axis_filter;
    boost::optional<math::vec3f> m_override_axis_filter;
    math::line2f m_tangent;

    Select_Gizmo m_select_gizmo;
};
