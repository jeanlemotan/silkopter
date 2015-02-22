#pragma once

class Camera_Controller3D
{
public:
	typedef std::function<float(const math::vec2u32&)> Depth_Getter;

	Camera_Controller3D(q::scene::Camera& camera);

	void set_depth_getter(Depth_Getter getter);

	void mouse_press_event(QMouseEvent* event);
	void mouse_release_event(QMouseEvent* event);
	void mouse_move_event(QMouseEvent* event);
	void wheel_event(QWheelEvent* event);

	math::vec2f const& get_pointer_2d() const;
	math::vec3f const& get_pointer_3d() const;
	float	get_screen_depth() const;

	void set_focus_point(math::vec3f const& point);
private:
	q::scene::Camera& m_camera;

	bool m_pointer_pressed;
	math::vec2f m_pointer_2d;
	math::vec2f m_pointer_delta_2d;
	math::vec3f m_pointer_3d;
	math::vec3f m_pointer_delta_3d;
	float m_screen_depth;

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

	Depth_Getter m_depth_getter;
};

