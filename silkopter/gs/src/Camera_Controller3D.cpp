#include "stdafx.h"
#include "Camera_Controller3D.h"

///////////////////////////////////////////////////////////////////////////////

Camera_Controller3D::Camera_Controller3D(q::scene::Camera& camera)
	: m_camera(camera)
{
	m_screen_depth = 1.f;
	m_pointer_pressed = false;
}


//////////////////////////////////////////////////////////////////////////

void Camera_Controller3D::set_focus_point(math::vec3f const& point)
{
	m_pointer_delta_3d = point;
	m_press_point_3d = point;
}

///////////////////////////////////////////////////////////////////////////////

void Camera_Controller3D::mouse_press_event(QMouseEvent* event)
{
	if ((event->buttons() & Qt::LeftButton) != 0)
	{
	}

	// update pointer 2d
	m_pointer_2d.set(event->x(), event->y());
	m_pointer_delta_2d.set(0.f, 0.f);

	//math::line3f ray = mCamera.getRayFromScreenCoordinates(mPointer2D);
	//math::vec3f const* point = mModel->raycast(ray);
	//if (point)

	{
		math::vec3f out;
		//mCamera.project(out, *point);
// 		auto depth = m_depth_getter(math::vec2u32(m_pointer_2d));
// 		if (math::is_one(depth))
// 		{
// 			depth = m_screen_depth;
// 		}
// 		if (math::is_one(depth))
// 		{
// 			depth = 0.1f;
// 		}
// 		m_camera.unproject(out, math::vec3f(m_pointer_2d, depth));

//		m_screen_depth = depth;
		m_pointer_delta_3d.set(0.f, 0.f, 0.f);
		m_pointer_3d = out;

		m_press_point_3d = out;
	}

	//else
	{
		//	mPressPoint3D = ray.start + ray.getVectorNormalized() * 20.f;
	}

	m_pointer_pressed = true;
}

///////////////////////////////////////////////////////////////////////////////

void Camera_Controller3D::mouse_release_event(QMouseEvent* /*event*/)
{
	m_press_point_3d = m_pointer_3d;
	m_pointer_pressed = false;
}

///////////////////////////////////////////////////////////////////////////////

void Camera_Controller3D::mouse_move_event(QMouseEvent* event)
{
	m_pointer_delta_2d = math::vec2f(event->x(), event->y()) - m_pointer_2d;
	m_pointer_2d = math::vec2f(event->x(), event->y());

	math::vec3f out;
// 	m_camera.unproject(out, math::vec3f(m_pointer_2d, m_screen_depth));
	m_pointer_delta_3d = out - m_pointer_3d;
	m_pointer_3d = out;

	if (m_pointer_pressed && (event->buttons() & Qt::RightButton) != 0)
	{
		//if (QApplication::keyboardModifiers() & Qt::AltModifier)
		{
			math::vec2d rot(radians(m_pointer_delta_2d));

			math::quatd qx, qy;
            qx.set_from_angle_axis(-rot.x, math::vec3d(0, 0, 1));
            qy.set_from_angle_axis(-rot.y, math::vec3d(m_camera.get_right_vector()));
			math::quatd qdelta = qx * qy;
			math::vec3d p(m_camera.get_position());
			math::vec3d dir = math::vec3d(m_press_point_3d) - p;
			p = math::vec3d(m_press_point_3d) - math::rotate(qdelta, dir);
			math::quatd q = qdelta * math::quatd(m_camera.get_rotation());
			m_camera.set_transform(math::vec3f(p), math::quatf(q));
		}
// 		else
// 		{
// 			math::planef plane(m_press_point_3d, m_camera.get_front_vector());
// 			math::line3f ray = m_camera.get_ray_from_screen_coordinates(m_pointer_2d);
// 			math::vec3f dir = ray.get_vector_normalized();
// 			math::vec3f point;
// 			if (plane.get_intersection_with_line(ray.start, dir, point))
// 			{
// 				math::vec3f delta = m_press_point_3d - point;
// 				m_camera.set_position(m_camera.get_position() + delta);
// 			}
// 		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void Camera_Controller3D::wheel_event(QWheelEvent* event)
{
	if ((event->buttons() & Qt::MiddleButton) != 0 || (QApplication::keyboardModifiers() & Qt::AltModifier))
	{
		return;
	}

	int spd = 0;
	if (QApplication::keyboardModifiers() & Qt::ControlModifier)
	{
		spd--;
	}
	if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
	{
		spd++;
	}

	math::vec3f dir = m_camera.get_front_vector();

	float speed = 2.5f;
	if (spd > 0) speed = 10;
	else if (spd < 0) speed = 0.5f;

	speed = speed * (event->delta() > 0 ? 1.0f : -1.0f);

	m_camera.set_position(m_camera.get_position() + dir*speed);
}

///////////////////////////////////////////////////////////////////////////////

math::vec2f const& Camera_Controller3D::get_pointer_2d() const
{
	return m_pointer_2d;
}

///////////////////////////////////////////////////////////////////////////////

math::vec3f const& Camera_Controller3D::get_pointer_3d() const
{
	return m_pointer_3d;
}

///////////////////////////////////////////////////////////////////////////////

float Camera_Controller3D::get_screen_depth() const
{
	return m_screen_depth;
}

///////////////////////////////////////////////////////////////////////////////

void Camera_Controller3D::set_depth_getter(Depth_Getter getter)
{
	m_depth_getter = getter;
}

///////////////////////////////////////////////////////////////////////////////
