#pragma once

namespace math
{

//! Axis aligned bounding box in 3d dimensional space.
template <typename T>
struct aabb2
{
///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
	aabb2();
	//! Constructor with min edge and max edge.
	aabb2(const vec2<T>& min, const vec2<T>& max);
	//! Constructor with only one point.
	aabb2(const vec2<T>& init);
	//! Constructor with min edge and max edge as single values, not vectors.
	aabb2(T minx, T miny, T minz, T maxx);

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

	//! Resets the bounding box to a one-point box.
	/** \param x X coord of the point.
	\param y Y coord of the point. */
	void reset(T x, T y);

	//! Resets the bounding box.
	/** \param initValue New box to set this one to. */
	void reset(const aabb2<T>& initValue);

	//! Resets the bounding box.
	/** \param min Box min edge. 
		\param max Box max edge.
	*/
	void reset(const vec2<T>& min, const vec2<T>& max);

	//! Resets the bounding box to a one-point box.
	/** \param initValue New point. */
	void reset(const vec2<T>& initValue);

	//! Resets the bounding box to an oriented bounding box
	void reset(const mat3<T>& mat, const aabb2<T>& localBox);

	//! Get extent of the box
	/** \return Extent of the bounding box. */
	vec2<T> get_extent() const;

	//! Get the radius of the sphere containing the box
	/** \return Radius of the sphere containing the box. */
	T get_radius() const;

	//! Get the radius of the sphere containing the box
	/** \return Radius of the sphere containing the box. */
	T get_radius_fast() const;

	//! Get the squared radius of the sphere containing the box
	/** \return Squared radius of the sphere containing the box. */
	T get_radius_sq() const;

	//! Get the radius of the sphere containing the box
	/** \return Radius of the sphere containing the box. */
	T get_inner_radius() const;

	//! Get the squared radius of the sphere containing the box
	/** \return Squared radius of the sphere containing the box. */
	T get_inner_radius_sq() const;

	//! Stores all 4 edges/corners of the box into an array
	/** \param edges: an array of 4 edges. */
	void get_edges(vec2<T> edges[4]) const;

	//! Check if the box is empty.
	/** This means that there is no space between the min and max edge.
	\return True if box is empty, else false. */
	bool is_empty() const;
	
	//! Repairs the box.
	/** Necessary if for example MinEdge and MaxEdge are swapped. */
	void repair();

	//! Calculates a new interpolated bounding box.
	/** \param other: other box to interpolate between
	\param d: value between 0.0f and 1.0f.
	\return Interpolated box. */
	aabb2<T> get_interpolated(const aabb2<T>& other, float d) const;

	//! Get center of the bounding box
	/** \return Center of the bounding box. */
	vec2<T> get_center() const;

	//! Get the surface area of the box in squared units
	T get_area() const;

///////////////////////////////////////////////////////////////////////////////
// containment
///////////////////////////////////////////////////////////////////////////////

	//! Determines if a point is within this box.
	/** \param p: Point to check.
	\return True if the point is within the box and false if not */
	bool is_point_inside(const vec2<T>& p) const;

	//! Determines if a point is within this box and its borders.
	/** \param p: Point to check.
	\return True if the point is within the box and false if not. */
	bool is_point_totally_inside(const vec2<T>& p) const;

	//! Check if this box is completely inside the 'other' box.
	/** \param other: Other box to check against.
	\return True if this box is completly inside the other box,
	otherwise false. */
	bool is_fully_inside(const aabb2<T>& other) const;

	//! Adds a point to the bounding box
	/** The box grows bigger, if point was outside of the box.
	\param p: Point to add into the box. */
	void add_internal_point(const vec2<T>& p);

	//! Adds a point to the bounding box
	/** The box grows bigger, if point is outside of the box.
	\param x X coordinate of the point to add to this box.
	\param y Y coordinate of the point to add to this box.
	\param z Z coordinate of the point to add to this box. */
	void add_internal_point(T x, T y);

	//! Adds another bounding box
	/** The box grows bigger, if the new box was outside of the box.
	\param b: Other bounding box to add into this box. */
	void add_internal_box(const aabb2<T>& b);

	// ! Adds an oriented box
	void add_internal_box(const mat3<T>& mat, const aabb2<T>& b);

///////////////////////////////////////////////////////////////////////////////
// intersection
///////////////////////////////////////////////////////////////////////////////

	//! Determines if the box intersects with another box.
	/** \param other: Other box to check a intersection with.
	\return True if there is an intersection with the other box,
	otherwise false. */
	bool intersect_with_box(const aabb2<T>& other) const;

	//! Tests if the box intersects with a line
	/** \param linemiddle Center of the line.
	\param linevect Vector of the line.
	\param halflength Half length of the line.
	\return True if there is an intersection, else false. */
	bool intersect_with_line(const vec2<T>& linemiddle, const vec2<T>& linevect, T halflength) const;

	bool intersect_with_line(const line2<T>& line) const;

	/** Check the intersection of a 3d ray against the box3. 
	@param ray The ray to test for collision in the box local coordinate system.
	@param outIntersectionNear closest intersection point
	@param outIntersectionFar furthest intersection point
	@return true if there is intersection and the closest and furthest intersection are written if so; false is returned otherwise.
	*/
	bool intersect_with_ray(const line2<T>& ray, vec2<T>& outIntersectionNear, vec2<T>& outIntersectionFar) const;

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

	bool operator==(const aabb2<T>& other) const;
	bool operator!=(const aabb2<T>& other) const;

///////////////////////////////////////////////////////////////////////////////
// members
///////////////////////////////////////////////////////////////////////////////

	vec2<T> minPoint;
	vec2<T> maxPoint;
};

}
