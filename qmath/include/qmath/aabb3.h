#pragma once

namespace math
{

//! Axis aligned bounding box in 3d dimensional space.
template <typename T>
struct aabb3
{
///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
	aabb3();
	//! Constructor with min edge and max edge.
	aabb3(const vec3<T>& min, const vec3<T>& max);
	//! Constructor with only one point.
	aabb3(const vec3<T>& init);
	//! Constructor with min edge and max edge as single values, not vectors.
	aabb3(T minx, T miny, T minz, T maxx, T maxy, T maxz);

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

	//! Resets the bounding box to a one-point box.
	/** \param x X coord of the point.
	\param y Y coord of the point.
	\param z Z coord of the point. */
	void reset(T x, T y, T z);

	//! Resets the bounding box.
	/** \param initValue New box to set this one to. */
	void reset(const aabb3<T>& initValue);

	//! Resets the bounding box.
	/** \param min Box min edge. 
		\param max Box max edge.
	*/
	void reset(const vec3<T>& min, const vec3<T>& max);

	//! Resets the bounding box to a one-point box.
	/** \param initValue New point. */
	void reset(const vec3<T>& initValue);

	//! Get extent of the box
	/** \return Extent of the bounding box. */
	vec3<T> get_extent() const;

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

	//! Stores all 8 edges/corners of the box into an array
	/** \param edges: an array of 8 edges. */
	void get_edges(vec3<T> edges[8]) const;

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
	aabb3<T> get_interpolaed(const aabb3<T>& other, float d) const;

	//! Get center of the bounding box
	/** \return Center of the bounding box. */
	vec3<T> get_center() const;

	//! Get the volume enclosed by the box in cubed units
	T get_volume() const;

	//! Get the surface area of the box in squared units
	T get_area() const;

///////////////////////////////////////////////////////////////////////////////
// containment
///////////////////////////////////////////////////////////////////////////////

	//! Determines if a point is within this box.
	/** \param p: Point to check.
	\return True if the point is within the box and false if not */
	bool is_point_inside(const vec3<T>& p) const;

	//! Determines if a point is within this box and its borders.
	/** \param p: Point to check.
	\return True if the point is within the box and false if not. */
	bool is_point_totally_inside(const vec3<T>& p) const;

	//! Check if this box is completely inside the 'other' box.
	/** \param other: Other box to check against.
	\return True if this box is completly inside the other box,
	otherwise false. */
	bool is_fully_inside(const aabb3<T>& other) const;

	//! Adds a point to the bounding box
	/** The box grows bigger, if point was outside of the box.
	\param p: Point to add into the box. */
	void add_internal_point(const vec3<T>& p);

	//! Adds a point to the bounding box
	/** The box grows bigger, if point is outside of the box.
	\param x X coordinate of the point to add to this box.
	\param y Y coordinate of the point to add to this box.
	\param z Z coordinate of the point to add to this box. */
	void add_internal_point(T x, T y, T z);

	//! Adds another bounding box
	/** The box grows bigger, if the new box was outside of the box.
	\param b: Other bounding box to add into this box. */
	void add_internal_box(const aabb3<T>& b);

	// ! Adds an oriented box
	void add_internal_box(const mat4<T>& mat, const aabb3<T>& b);

///////////////////////////////////////////////////////////////////////////////
// intersection
///////////////////////////////////////////////////////////////////////////////

	//! Determines if the box intersects with another box.
	/** \param other: Other box to check a intersection with.
	\return True if there is an intersection with the other box,
	otherwise false. */
	bool intersect_with_box(const aabb3<T>& other) const;

	//! Tests if the box intersects with a line
	/** \param linemiddle Center of the line.
	\param linevect Vector of the line.
	\param halflength Half length of the line.
	\return True if there is an intersection, else false. */
	bool intersect_with_line(const vec3<T>& linemiddle, const vec3<T>& linevect, T halflength) const;

	bool intersect_with_line(const line3<T>& line) const;

	/** Check the intersection of a 3d ray against the box3. 
	@param ray The ray to test for collision in the box local coordinate system.
	@param outIntersectionNear closest intersection point
	@param outIntersectionFar furthest intersection point
	@return true if there is intersection and the closest and furthest intersection are written if so; false is returned otherwise.
	*/
	bool intersect_with_ray(const line3<T>& ray, vec3<T>& outIntersectionNear, vec3<T>& outIntersectionFar) const;

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

	bool operator==(const aabb3<T>& other) const;
	bool operator!=(const aabb3<T>& other) const;

///////////////////////////////////////////////////////////////////////////////
// members
///////////////////////////////////////////////////////////////////////////////

	vec3<T> minPoint;
	vec3<T> maxPoint;
};

}
