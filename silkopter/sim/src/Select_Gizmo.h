#pragma once


class Select_Gizmo
{
public:
	Select_Gizmo(float length);

    void set_transform(math::trans3df const& transform);
    void render(Render_Context& context);

private:
	float m_length;
	math::trans3df m_transform;
    math::vec3f m_axis_ends[3];
	
};
