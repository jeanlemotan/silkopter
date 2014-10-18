#pragma once

namespace math
{

//! Enumeration for intersection relations of 3d objects
enum RelationEnum
{
	k_relFront = 0,
	k_relBack,
	k_relPlanar,
	k_relSpanning,
	k_relClipping
};

//! Template plane class with some intersection testing methods.
template <class T>
struct plane
{
///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
	plane();
	plane(const vec3<T>& point, const vec3<T>& normal);
	plane(T px, T py, T pz, T nx, T ny, T nz);
	plane(const vec3<T>& point1, const vec3<T>& point2, const vec3<T>& point3);

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////
	bool operator==(const plane<T>& other) const;
	bool operator!=(const plane<T>& other) const;

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////
	void set_plane(const vec3<T>& point, const vec3<T>& normal);
	void set_plane(const vec3<T>& normal, T d);
	void set_plane(const vec3<T>& point1, const vec3<T>& point2, const vec3<T>& point3);

	//! Recalculates the distance from origin by applying a new member point to the plane.
	void recalculate_d(const vec3<T>& MPoint);
	//! Gets a member point of the plane.
	vec3<T> get_member_point() const;

	//! Test if the triangle would be front or backfacing from any point.
	/** Thus, this method assumes a camera position from
	which the triangle is definitely visible when looking into
	the given direction.
	Note that this only works if the normal is Normalized.
	Do not use this method with points as it will give wrong results!
	\param lookDirection: Look direction.
	\return True if the plane is front facing and
	false if it is backfacing. */
	bool is_front_facing(const vec3<T>& lookDirection) const;

	//! Get the distance to a point.
	/** Note that this only works if the normal is normalized. */
	T get_distance_to(const vec3<T>& point) const;

	//! Project a point on the plane.
	vec3<T> project_point(const vec3<T>& point) const;

	///! Computes the inverse halfspace
	plane<T>& invert();

///////////////////////////////////////////////////////////////////////////////
// intersection methods
///////////////////////////////////////////////////////////////////////////////
	//! Get an intersection with a 3d line.
	/** \param lineVect Vector of the line to intersect with.
	\param linePoint Point of the line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if there was not.
	*/
	bool get_intersection_with_line(const vec3<T>& linePoint,const vec3<T>& lineVect, vec3<T>& outIntersection) const;

	//! Get percentage of line between two points where an intersection with this plane happens.
	/** Only useful if known that there is an intersection.
	\param linePoint1 Point1 of the line to intersect with.
	\param linePoint2 Point2 of the line to intersect with.
	\return Where on a line between two points an intersection with this plane happened.
	For example, 0.5 is returned if the intersection happened exactly in the middle of the two points.
	*/
	float get_known_intersection_with_line(const vec3<T>& linePoint1,const vec3<T>& linePoint2) const;

	//! Get an intersection with a 3d line, limited between two 3d points.
	/** \param linePoint1 Point 1 of the line.
	\param linePoint2 Point 2 of the line.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if there was not.
	*/
	bool get_intersection_with_limited_line(const vec3<T>& linePoint1, const vec3<T>& linePoint2, vec3<T>& outIntersection) const;

	//! Classifies the relation of a point to this plane.
	/** \param point Point to classify its relation.
	\return ISREL3D_FRONT if the point is in front of the plane,
	ISREL3D_BACK if the point is behind of the plane, and
	ISREL3D_PLANAR if the point is within the plane. */
	RelationEnum classify_point_relation(const vec3<T>& point) const;

	//! Tests if there is an intersection with the other plane
	/** \return True if there is a intersection. */
	bool exists_intersection(const plane<T>& other) const;

	//! Intersects this plane with another.
	/** \param other Other plane to intersect with.
	\param outLinePoint Base point of intersection line.
	\param outLineVect Vector of intersection.
	\return True if there is a intersection, false if not. */
	bool get_intersection_with_plane(const plane<T>& other, vec3<T>& outLinePoint, vec3<T>& outLineVect) const;

	//! Get the intersection point with two other planes if there is one.
	bool get_intersection_with_planes(const plane<T>& o1, const plane<T>& o2, vec3<T>& outPoint) const;

///////////////////////////////////////////////////////////////////////////////
// overlap methods
///////////////////////////////////////////////////////////////////////////////
	///! Checks for plane AABB overlap
	// Return values:
	// 0: box is totally outside of the plane
	// 1: box is totally inside of the plane
	// 2: box is partially inside of the plane
	int8_t overlap(const vec3<T>& boxCenter, vec3<T>& boxHalfsize) const;

	//! Normal of the plane.
	vec3<T>  normal;
	//! Distance from origin.
	T             d;
};

typedef plane<float>  planef;

}
