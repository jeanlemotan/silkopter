#pragma once

namespace q
{
namespace scene
{

	class Camera
	{
	public:
		Camera();

		void				set_viewport_and_aspect_ratio(video::Viewport const& viewport);
		void				set_viewport(video::Viewport const& viewport);
		video::Viewport const& get_viewport() const;
		void				set_aspect_ratio(float aspectRatio);
		float				get_aspect_ratio() const;

		//switches the camera to parallel projection
		void				set_parallel_zoom(float zoom);
		float				get_parallel_zoom() const;

		//switches the camera to perspective projection
		void				set_perspective_vertical_fov(float fov);
		float				get_perspective_vertical_fov() const;

		void				set_near_distance(float d);
		float				get_near_distance() const;

		void				set_far_distance(float d);
		float				get_far_distance() const;

		math::mat4f const&	get_projection_matrix() const;
		math::mat4f const&	get_view_matrix() const;
		math::mat4f const&	get_view_projection_matrix() const;

		Frustum const&		get_frustum() const;

		math::line3f		get_ray_from_screen_coordinates(math::vec2f const& pos) const;

		bool				project(math::vec2f& out, math::vec3f const& in) const;
		bool				project(math::vec3f& out, math::vec3f const& in) const;
		bool				unproject(math::vec3f& out, math::vec3f const& in) const;

		void				set_position(math::vec3f const& pos);
		math::vec3f const&	get_position() const;

		void				set_rotation(math::quatf const& rot);
		math::quatf const&	get_rotation() const;

		void				set_transform(math::vec3f const& pos, math::quatf const& rot);

		math::vec3f const&	get_front_vector() const;
		math::vec3f const&	get_up_vector() const;
		math::vec3f const&	get_right_vector() const;

	protected:
		void				update_frustum() const;
		void				update_view_projection_matrix() const;
		void				update_projection_matrix() const;
		void				update_view_matrix() const;

	private:
		video::Viewport		m_viewport;
		bool				m_is_parallel;

		float				m_aspect_ratio;
		float				m_fov_vertical;
		mutable float		m_fov_horizontal;
		float				m_zoom;
		float				m_near_distance;
		float				m_far_distance;

		mutable float		m_aspect_ratio_cached;

		mutable bool		m_view_projection_matrix_dirty;
		mutable bool		m_projection_matrix_dirty;
		mutable bool		m_view_matrix_dirty;

		mutable math::mat4f	m_view_projection_matrix;
		mutable math::mat4f	m_projection_matrix;
		mutable math::mat4f	m_view_matrix;
		mutable Frustum		m_frustum;

		math::vec3f			m_position;
		math::quatf			m_rotation;

		mutable math::vec3f	m_front_vector;
		mutable math::vec3f	m_up_vector;
		mutable math::vec3f	m_right_vector;
	};

}
}