#include "QStdAfx.h"
#include "scene/Camera.h"

using namespace q;
using namespace scene;

// static math::mat4f gl_to_q(const math::mat4f& im)
// {
// 	auto mat = im;
// 	auto axisX = im.get_axis_x();
// 	auto axisY = im.get_axis_y();
// 	auto axisZ = im.get_axis_z();
// 	mat.set_axis_x(-axisX);
// 	mat.set_axis_y(axisZ);
// 	mat.set_axis_z(axisY);
// 
// 	return mat;
// }

//////////////////////////////////////////////////////////////////////////

static math::mat4f q_to_gl(const math::mat4f& im)
{
	auto mat = im;
	auto axisX = im.get_axis_x();
	auto axisY = im.get_axis_y();
	auto axisZ = im.get_axis_z();
	mat.set_axis_x(-axisX);
	mat.set_axis_y(axisZ);
	mat.set_axis_z(axisY);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

Camera::Camera()
	: m_viewport(math::vec2u32(0), math::vec2u32(100))
	, m_is_parallel(false)
	, m_aspect_ratio(1.f)
	, m_fov_vertical(0.785375f) //45 degrees
	, m_zoom(1.f)
	, m_near_distance(1.f)
	, m_far_distance(1000.f)
	, m_view_projection_matrix_dirty(true)
	, m_projection_matrix_dirty(true)
	, m_view_matrix_dirty(true)
{
	m_front_vector = scene::get_front_vector();
	m_up_vector = scene::get_up_vector();
	m_right_vector = scene::get_right_vector();
}

void Camera::set_parallel_zoom(float zoom)
{
	m_is_parallel = true;
	m_projection_matrix_dirty = true;
	m_view_projection_matrix_dirty = true;
	QASSERT(zoom >= 0.f);
	m_zoom = math::max(zoom, 0.f);
}
float Camera::get_parallel_zoom() const
{
	return m_zoom;
}

void Camera::set_perspective_vertical_fov(float fov)
{
	m_is_parallel = false;
	m_projection_matrix_dirty = true;
	m_view_projection_matrix_dirty = true;
	m_fov_vertical = fov;
}
float Camera::get_perspective_vertical_fov() const
{
	return m_fov_vertical;
}

void Camera::set_near_distance(float d)
{
	m_projection_matrix_dirty = true;
	m_view_projection_matrix_dirty = true;
	m_near_distance = d;
}
float Camera::get_near_distance() const
{
	return m_near_distance;
}

void Camera::set_far_distance(float d)
{
	m_projection_matrix_dirty = true;
	m_view_projection_matrix_dirty = true;
	m_far_distance = d;
}
float Camera::get_far_distance() const
{
	return m_far_distance;
}

math::line3f Camera::get_ray_from_screen_coordinates(math::vec2f const& pos) const
{
	math::line3f ln(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

	auto vp = get_viewport();
	math::vec2f screen_size((float)vp.size.x, (float)vp.size.y);

	float dx = pos.x / screen_size.x;
	float dy = pos.y / screen_size.y;

	auto const& f = get_frustum();

	auto near_left_up  = f.get_near_left_up();
	auto left_to_right = f.get_near_right_up()  - near_left_up;
	auto up_to_down    = f.get_near_left_down() - near_left_up;

	ln.start = near_left_up + (left_to_right * dx) + (up_to_down * dy);

	auto farLeftUp = f.get_far_left_up();
	left_to_right = f.get_far_right_up()  - farLeftUp;
	up_to_down    = f.get_far_left_down() - farLeftUp;

	ln.end = farLeftUp + (left_to_right * dx) + (up_to_down * dy);
	return ln;
}

math::mat4f const& Camera::get_view_projection_matrix() const
{
	update_view_projection_matrix();
	return m_view_projection_matrix;
}
math::mat4f const& Camera::get_projection_matrix() const
{
	update_projection_matrix();
	return m_projection_matrix;
}
math::mat4f const& Camera::get_view_matrix() const
{
	update_view_matrix();
	return m_view_matrix;
}
Frustum const& Camera::get_frustum() const
{
	update_view_projection_matrix();
	return m_frustum;
}

void Camera::set_transform(math::vec3f const& pos, math::quatf const& rot)
{
	m_view_matrix_dirty = m_view_projection_matrix_dirty = true;
	m_position = pos;
	m_rotation = rot;
	//refresh vectors
	m_front_vector = math::normalized(math::rotate(rot, scene::get_front_vector()));
	m_up_vector = math::normalized(math::rotate(rot, scene::get_up_vector()));
	m_right_vector = math::normalized(math::rotate(rot, scene::get_right_vector()));
}
void Camera::set_position(math::vec3f const& pos)
{
	m_view_matrix_dirty = true;
	m_view_projection_matrix_dirty = true;
	m_position = pos;
}
math::vec3f const& Camera::get_position() const
{
	return m_position;
}

math::quatf const& Camera::get_rotation() const
{
	return m_rotation;
}
math::vec3f const& Camera::get_front_vector() const
{
	return m_front_vector;
}
math::vec3f const& Camera::get_up_vector() const
{
	return m_up_vector;
}
math::vec3f const& Camera::get_right_vector() const
{
	return m_right_vector;
}

void Camera::update_view_projection_matrix() const
{
	if (m_view_projection_matrix_dirty)
	{
		math::multiply(m_view_projection_matrix, get_projection_matrix(), get_view_matrix());
		m_frustum.set_from(m_view_projection_matrix);
		m_view_projection_matrix_dirty = false;
	}
}

void Camera::update_projection_matrix() const
{
	float ar = get_aspect_ratio();
	if (m_projection_matrix_dirty || ar != m_aspect_ratio_cached)
	{
		m_aspect_ratio_cached = ar;
		if (m_is_parallel)
		{
			float yHalfSize = m_zoom * 0.5f;
			float xHalfSize = m_zoom * ar * 0.5f;

			m_projection_matrix = math::ortho(-xHalfSize, xHalfSize, -yHalfSize, yHalfSize, m_near_distance, m_far_distance);
		}
		else
		{
			m_projection_matrix = math::perspective(m_fov_vertical, ar, m_near_distance, m_far_distance);
			m_fov_horizontal = 2.f * math::atan(math::tan(m_fov_vertical * 0.5f) * ar);
		}

		m_projection_matrix_dirty = false;
		m_view_projection_matrix_dirty = true;
	}
}

void Camera::set_viewport_and_aspect_ratio(video::Viewport const& viewport)
{
	set_viewport(viewport);
	set_aspect_ratio((float)viewport.size.x / (float)viewport.size.y);
}

void Camera::set_viewport(video::Viewport const& viewport)
{
	using namespace std::rel_ops;

	if (m_viewport != viewport)
	{	 
		m_viewport = viewport;
		m_projection_matrix_dirty = true;
	}
}

void Camera::set_aspect_ratio(float aspectRatio)
{
	if (m_aspect_ratio != aspectRatio)
	{	 
		m_aspect_ratio = aspectRatio;
	}
}

video::Viewport const& Camera::get_viewport() const
{
	return m_viewport;
}
void Camera::set_rotation(math::quatf const& rot)
{
	m_view_matrix_dirty = m_view_projection_matrix_dirty = true;

	m_rotation = rot;
	//refresh vectors
	m_front_vector = math::normalized(math::rotate(rot, scene::get_front_vector()));
	m_up_vector = math::normalized(math::rotate(rot, scene::get_up_vector()));
	m_right_vector = math::normalized(math::rotate(rot, scene::get_right_vector()));
}

float Camera::get_aspect_ratio() const
{
	//auto vp = getViewport().size;
	//return vp.y > 0 ? (float)vp.x / (float)vp.y : 1.f;
	return m_aspect_ratio;
}

void Camera::update_view_matrix() const
{
	if (!m_view_matrix_dirty)
	{
		return;
	}

	auto const& pos = get_position();
	auto const& rot = get_rotation();

	// the View matrix is created by taking the world rotation and translation and then computing the inverse of that matrix.
	// we also have to take into account the axis change from jet to opengl coordinate system just before making the inverse.
	math::trans3df tmp_view_transform(math::vec3f::zero, rot, math::vec3f::one);
	tmp_view_transform.set_translation(pos);

	// now we have the view transform in jet world coordinates... convert it to opengl world coordinates
	auto tmp_view_matrix = q_to_gl(tmp_view_transform.mat);

	// and now make the inverse in order to get a view matrix
	m_view_matrix = math::inverse(tmp_view_matrix);

	m_view_matrix_dirty = false;
	m_view_projection_matrix_dirty = true;
}

//////////////////////////////////////////////////////////////////////////

bool Camera::project(math::vec2f& out, math::vec3f const& in) const
{
	math::vec3f out3;
	bool res = project(out3, in);
	out.set(out3.x, out3.y);
	return res;
}

//////////////////////////////////////////////////////////////////////////

bool Camera::project(math::vec3f& out, math::vec3f const& in) const
{
	auto const& viewproj = get_view_projection_matrix();
	auto vp = get_viewport();

	math::vec4f in4, out4;
	in4 = math::transform(viewproj, math::vec4f(in, 1.f));

	if (math::is_zero(in4.w))
	{
		return false;
	}

	math::vec3f in3(in4.x, in4.y, in4.z);
	in3 /= in4.w;

	out.x = vp.position.x + (1.0f + in3.x) * vp.size.x * 0.5f;
	out.y = vp.size.y - (vp.position.y + (1.0f + in3.y) * vp.size.y * 0.5f);
	out.z = (1.0f + in3.z) * 0.5f;
	return in4.w > 0.0f;
}

bool Camera::unproject(math::vec3f& out, math::vec3f const& in) const
{
	math::mat4f inv;
	math::vec4f in4;

	auto const& viewproj = get_view_projection_matrix();
	auto vp = get_viewport();

	in4.x = (in.x - vp.position.x) * 2.0f / vp.size.x - 1.0f;
	in4.y = ((vp.size.y - in.y) - vp.position.y) * 2.0f / vp.size.y - 1.0f;
	in4.z = 2.0f * in.z - 1.0f;
	in4.w = 1.0f;

	in4 = math::transform(math::mat4f(math::inverse(math::mat4d(viewproj))), in4);
	if (!math::is_zero(in4.w))
	{
		in4 /= in4.w;
	}

	out = math::vec3f(in4.x, in4.y, in4.z);
	return true;
}
