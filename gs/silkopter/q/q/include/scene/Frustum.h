#pragma once

namespace q
{


namespace scene
{

//! Defines the view frustum. That's the space visible by the camera.
/** The view frustum is enclosed by 6 planes. These six planes share
four points. A bounding box around these four points is also stored in
this structure.
*/
enum class Cull_Status : uint8_t
{
	INSIDE = 0,
	OUTSIDE,
	INTERSECTING
};

struct Frustum
{
	enum class Plane
	{
		//! Far plane of the frustum. That is the plane farest away from the eye.
		FAR_PLANE = 0,
		//! Near plane of the frustum. That is the plane nearest to the eye.
		NEAR_PLANE,
		//! Left plane of the frustum.
		LEFT_PLANE,
		//! Right plane of the frustum.
		RIGHT_PLANE,
		//! Bottom plane of the frustum.
		BOTTOM_PLANE,
		//! Top plane of the frustum.
		TOP_PLANE,
		//! Amount of planes enclosing the view frustum. Should be 6.
		COUNT
	};

	Frustum();
	//Frustum(Frustum const& other);

	//! This constructor creates a view frustum based on a view * projection matrix.
	Frustum(math::mat4f const& view_projection_matrix);

	//! Sets the view * projection matrix to the frustum
	void set_from(math::mat4f const& view_projection_matix);

	//! returns a plane of the frustum.
	math::planef const& get_plane(Plane plane) const;

	//! returns the pointer to the planes
	math::planef const* get_all_planes() const;

	//! returns the point which is on the far left upper corner inside the the view frustum.
	math::vec3f get_far_left_up() const;

	//! returns the point which is on the far left bottom corner inside the the view frustum.
	math::vec3f get_far_left_down() const;

	//! returns the point which is on the far right top corner inside the the view frustum.
	math::vec3f get_far_right_up() const;

	//! returns the point which is on the far right bottom corner inside the the view frustum.
	math::vec3f get_far_right_down() const;

	//! returns the point which is on the near left upper corner inside the the view frustum.
	math::vec3f get_near_left_up() const;

	//! returns the point which is on the near left bottom corner inside the the view frustum.
	math::vec3f get_near_left_down() const;

	//! returns the point which is on the near right top corner inside the the view frustum.
	math::vec3f get_near_right_up() const;

	//! returns the point which is on the near right bottom corner inside the the view frustum.
	math::vec3f get_near_right_down() const;

	void		set_origin(math::vec3f const& pos);
	math::vec3f const& get_origin() const;

	Cull_Status cull_bounds(util::Bounds const& bounds) const;
	Cull_Status cull_boundsAround(util::Bounds const& bounds) const;
	Cull_Status cull_aabb(math::aabb3f const& aabb) const;

protected:
	void	compute_planes() const;
	void	computePoints() const;

protected:
	math::mat4f m_view_projection_matrix;

	math::vec3f m_origin;

	//! all planes enclosing the view frustum.
	mutable bool m_dirty_planes;
	mutable math::planef m_planes[(int)Plane::COUNT];

	mutable bool m_dirty_points;
	mutable math::vec3f m_far_left_up;
	mutable math::vec3f m_far_left_down;
	mutable math::vec3f m_far_right_up;
	mutable math::vec3f m_far_right_down;
	mutable math::vec3f m_near_left_up;
	mutable math::vec3f m_near_left_down;
	mutable math::vec3f m_near_right_up;
	mutable math::vec3f m_near_right_down;
};

//////////////////////////////////////////////////////////////////////////

inline Frustum::Frustum()
: m_dirty_planes(false)
, m_dirty_points(false)
{}

//////////////////////////////////////////////////////////////////////////

/*
inline Frustum::Frustum(Frustum const& other)
{
	m_view_projection_matrix = other.m_view_projection_matrix;
	m_origin = other.m_origin;

	m_dirty_planes = other.m_dirty_planes;
	if (m_dirty_planes) // if the planes are dirty there's no need to copy them
	{
		for (size_t i = 0; i < (size_t)Planes::COUNT; ++i)
		{
			m_planes[i] = other.m_planes[i];
		}
	}

	m_dirty_points = other.m_dirty_points;
	if (m_dirty_points) // if the points are dirty there's no need to copy them
	{
		m_far_left_up = other.m_far_left_up;
		m_far_left_down = other.m_far_left_down;
		m_far_right_up = other.m_far_right_up;
		m_far_right_down = other.m_far_right_down;
		m_near_left_up = other.m_near_left_up;
		m_near_left_down = other.m_near_left_down;
		m_near_right_up = other.m_near_right_up;
		m_near_right_down = other.m_near_right_down;
	}
}
*/

//////////////////////////////////////////////////////////////////////////

inline Frustum::Frustum(math::mat4f const& viewProjTrans)
: m_dirty_planes(true)
, m_dirty_points(true)
{
	m_view_projection_matrix = viewProjTrans;
}

//////////////////////////////////////////////////////////////////////////

inline void Frustum::set_from(math::mat4f const& viewProjTrans)
{
	m_view_projection_matrix = viewProjTrans;
	m_dirty_planes = true;
	m_dirty_points = true;
}

//////////////////////////////////////////////////////////////////////////

inline math::planef const& Frustum::get_plane(Plane plane) const
{
	if (m_dirty_planes)
	{
		compute_planes();
	}
	return m_planes[(size_t)plane];
}

//////////////////////////////////////////////////////////////////////////

inline math::planef const* Frustum::get_all_planes() const
{
	if (m_dirty_planes)
	{
		compute_planes();
	}
	return m_planes;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_far_left_up() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_far_left_up;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_far_left_down() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_far_left_down;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_far_right_up() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_far_right_up;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_far_right_down() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_far_right_down;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_near_left_up() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_near_left_up;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_near_left_down() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_near_left_down;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_near_right_up() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_near_right_up;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f Frustum::get_near_right_down() const
{
	if (m_dirty_points)
	{
		computePoints();
	}
	return m_near_right_down;
}

//////////////////////////////////////////////////////////////////////////

inline void Frustum::set_origin(math::vec3f const& pos)
{
	m_origin = pos;
}

//////////////////////////////////////////////////////////////////////////

inline math::vec3f const& Frustum::get_origin() const
{
	return m_origin;
}

//////////////////////////////////////////////////////////////////////////

inline Cull_Status Frustum::cull_aabb(math::aabb3f const& aabb) const
{
	if (m_dirty_planes)
	{
		compute_planes();
	}

	Cull_Status status = Cull_Status::INSIDE;
	math::vec3f vmin, vmax; 

	auto const* plane = m_planes;
	for (int i = (int)Frustum::Plane::COUNT - 1; i >= 0; i--)
	{
		auto const& n = plane->normal;

		// X axis 
		if (n.x > 0) 
		{ 
			vmin.x = aabb.minPoint.x; 
			vmax.x = aabb.maxPoint.x; 
		} 
		else 
		{ 
			vmin.x = aabb.maxPoint.x; 
			vmax.x = aabb.minPoint.x; 
		} 
		// Y axis 
		if(n.y > 0) 
		{ 
			vmin.y = aabb.minPoint.y; 
			vmax.y = aabb.maxPoint.y; 
		} 
		else 
		{ 
			vmin.y = aabb.maxPoint.y; 
			vmax.y = aabb.minPoint.y; 
		} 
		// Z axis 
		if(n.z > 0) 
		{ 
			vmin.z = aabb.minPoint.z; 
			vmax.z = aabb.maxPoint.z; 
		} 
		else 
		{ 
			vmin.z = aabb.maxPoint.z; 
			vmax.z = aabb.minPoint.z; 
		} 

		float d = plane->get_distance_to(vmin);
		if (d > 0) 
		{
			return Cull_Status::OUTSIDE; 
		}
		d = plane->get_distance_to(vmax);
		if (d >= 0) 
		{
			status = Cull_Status::INTERSECTING; 
		}
		plane++;
	}
	return status;
}

//////////////////////////////////////////////////////////////////////////

inline Cull_Status Frustum::cull_bounds(util::Bounds const& bounds) const
{
	if (m_dirty_planes)
	{
		compute_planes();
	}

	Cull_Status status = Cull_Status::INSIDE;

	auto center = bounds.get_center();
	float radius = bounds.get_radius();

	auto const* plane = m_planes;
	for (size_t i = 0; i < static_cast<int>(Frustum::Plane::COUNT); i++)
	{
		float dist = plane->get_distance_to(center);
		if (dist > radius)
		{
			return Cull_Status::OUTSIDE;
		}
		if (dist > -radius)
		{
			status = Cull_Status::INTERSECTING;
		}
		plane++;
	}
	return status;
}

//////////////////////////////////////////////////////////////////////////

inline Cull_Status Frustum::cull_boundsAround(util::Bounds const& bounds) const
{
	if (m_dirty_planes)
	{
		compute_planes();
	}

	Cull_Status status = Cull_Status::INSIDE;

	auto center = bounds.get_center();
	float radius = bounds.get_radius();

	auto const* planes = m_planes;
	for (int i = (int)Frustum::Plane::COUNT - 1; i > (int)Plane::NEAR_PLANE; i--)
	{
		float dist = planes[i].get_distance_to(center);
		if (dist > radius)
		{
			return Cull_Status::OUTSIDE;
		}
		if (dist > -radius)
		{
			status = Cull_Status::INTERSECTING;
		}
	}
	return status;
}

//////////////////////////////////////////////////////////////////////////

inline void Frustum::compute_planes() const
{
	auto const& mat = m_view_projection_matrix;

	// left clipping plane
	m_planes[(int)Plane::LEFT_PLANE].normal.x = mat.m[3 ] + mat.m[0];
	m_planes[(int)Plane::LEFT_PLANE].normal.y = mat.m[7 ] + mat.m[4];
	m_planes[(int)Plane::LEFT_PLANE].normal.z = mat.m[11] + mat.m[8];
	m_planes[(int)Plane::LEFT_PLANE].d		  = mat.m[15] + mat.m[12];

	// right clipping plane
	m_planes[(int)Plane::RIGHT_PLANE].normal.x = mat.m[3 ] - mat.m[0];
	m_planes[(int)Plane::RIGHT_PLANE].normal.y = mat.m[7 ] - mat.m[4];
	m_planes[(int)Plane::RIGHT_PLANE].normal.z = mat.m[11] - mat.m[8];
	m_planes[(int)Plane::RIGHT_PLANE].d		   = mat.m[15] - mat.m[12];

	// top clipping plane
	m_planes[(int)Plane::TOP_PLANE].normal.x	= mat.m[3 ] - mat.m[1];
	m_planes[(int)Plane::TOP_PLANE].normal.y	= mat.m[7 ] - mat.m[5];
	m_planes[(int)Plane::TOP_PLANE].normal.z	= mat.m[11] - mat.m[9];
	m_planes[(int)Plane::TOP_PLANE].d			= mat.m[15] - mat.m[13];

	// bottom clipping plane
	m_planes[(int)Plane::BOTTOM_PLANE].normal.x	= mat.m[3 ] + mat.m[1];
	m_planes[(int)Plane::BOTTOM_PLANE].normal.y = mat.m[7 ] + mat.m[5];
	m_planes[(int)Plane::BOTTOM_PLANE].normal.z = mat.m[11] + mat.m[9];
	m_planes[(int)Plane::BOTTOM_PLANE].d		= mat.m[15] + mat.m[13];

	// far clipping plane
	m_planes[(int)Plane::FAR_PLANE].normal.x	= mat.m[3 ] - mat.m[2];
	m_planes[(int)Plane::FAR_PLANE].normal.y	= mat.m[7 ] - mat.m[6];
	m_planes[(int)Plane::FAR_PLANE].normal.z	= mat.m[11] - mat.m[10];
	m_planes[(int)Plane::FAR_PLANE].d			= mat.m[15] - mat.m[14];

	// near clipping plane
	m_planes[(int)Plane::NEAR_PLANE].normal.x	= mat.m[2];
	m_planes[(int)Plane::NEAR_PLANE].normal.y	= mat.m[6];
	m_planes[(int)Plane::NEAR_PLANE].normal.z	= mat.m[10];
	m_planes[(int)Plane::NEAR_PLANE].d			= mat.m[14];

	// normalize normals
	for (size_t i = 0; i < (size_t)Plane::COUNT; ++i)
	{
		const float len = -1.0f / math::length(m_planes[i].normal);
		m_planes[i].normal *= len;
		m_planes[i].d *= len;
	}
	m_dirty_planes = false;
}


//////////////////////////////////////////////////////////////////////////

inline void Frustum::computePoints() const
{
	//bool inv = m_viewProjMatrix.GetInverse(invMat);
	//PASSERT(inv);
	auto invMat = math::inverse(m_view_projection_matrix);

	math::vec4f nlu(-1, 1,-1, 1);
	math::vec4f nru( 1, 1,-1, 1);
	math::vec4f nld(-1,-1,-1, 1);
	math::vec4f nrd( 1,-1,-1, 1);
	math::vec4f flu(-1, 1, 1, 1);
	math::vec4f fru( 1, 1, 1, 1);
	math::vec4f fld(-1,-1, 1, 1);
	math::vec4f frd( 1,-1, 1, 1);

	nlu = math::transform(invMat, nlu);
	nru = math::transform(invMat, nru);
	nld = math::transform(invMat, nld);
	nrd = math::transform(invMat, nrd);
	flu = math::transform(invMat, flu);
	fru = math::transform(invMat, fru);
	fld = math::transform(invMat, fld);
	frd = math::transform(invMat, frd);

	m_near_left_up = math::vec3f(nlu.x, nlu.y, nlu.z) / nlu.w;
	m_near_right_up = math::vec3f(nru.x, nru.y, nru.z) / nru.w;
	m_near_left_down = math::vec3f(nld.x, nld.y, nld.z) / nld.w;
	m_near_right_down = math::vec3f(nrd.x, nrd.y, nrd.z) / nrd.w;
	m_far_left_up = math::vec3f(flu.x, flu.y, flu.z) / flu.w;
	m_far_right_up = math::vec3f(fru.x, fru.y, fru.z) / fru.w;
	m_far_left_down = math::vec3f(fld.x, fld.y, fld.z) / fld.w;
	m_far_right_down = math::vec3f(frd.x, frd.y, frd.z) / frd.w;

	m_dirty_points = false;
}


}
}
