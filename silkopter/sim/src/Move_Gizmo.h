#pragma once


class Move_Gizmo
{
public:
	Move_Gizmo(float length);

    void set_transform(math::trans3df const& transform);
    void render(Render_Context& context, math::vec2f const& pointer);

    math::vec3f get_axis_filter() const;
    void set_override_axis_filter(boost::optional<math::vec3f> const& axisFilter = boost::optional<math::vec3f>());

private:
    float m_length;
    math::trans3df m_transform;
    math::vec3f m_axis_filter;
    boost::optional<math::vec3f> m_override_axis_filter;
	
};
