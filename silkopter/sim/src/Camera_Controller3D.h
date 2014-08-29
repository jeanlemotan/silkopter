#pragma once

class Camera_Controller3D
{
public:
	Camera_Controller3D(q::scene::Camera& camera);

    void mouse_press_event(Mouse_Event const& event);
    void mouse_release_event(Mouse_Event const& event);
    void mouse_move_event(Mouse_Event const& event);
    void mouse_wheel_event(Mouse_Event const& event);

	math::vec2f const& get_pointer_2d() const;
	math::vec3f const& get_pointer_3d() const;

	void set_focus_point(math::vec3f const& point);

private:
	q::scene::Camera& m_camera;

	bool m_pointer_pressed;
	math::vec2f m_pointer_2d;
	math::vec2f m_pointer_delta_2d;
	math::vec3f m_pointer_3d;
	math::vec3f m_pointer_delta_3d;

	math::vec3f m_press_point_3d;

	struct 
	{
		enum type
		{
			ORBIT,
			PAN,
		};
		type interaction;
		math::vec3f pan_point;
	} m_camera_params;
};

