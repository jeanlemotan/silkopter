#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines RayIntersectPrimitive functions
///////////////////////////////////////////////////////////////////////////////

namespace math
{
	template<class T>
	bool ray_intersect_plane(
		vec3<T> const& origin, vec3<T> const& dir,
		plane<T> const& plane,
		T& t);

	template<class T>
	int ray_intersect_aabb(
		vec3<T> const& rayOrigin, vec3<T> const& rayDir,
		vec3<T> const& boxCenter, vec3<T> const& boxHalfsize,
		T& tmin, T& tmax);

	template<class T>
	int ray_intersect_sphere(
		vec3<T> const& rayOrigin, vec3<T> const& rayDirection,
		vec3<T> const& sphereCenter, T radius,
		T& t0, T& t1);

	template<class T>
	int ray_intersect_hemisphere(
		vec3<T> const& rayOrigin, vec3<T> const& rayDirection,
		vec3<T> const& sphereCenter, vec3<T> const& sphereNormal, T radius,
		T& t0, T& t1);

	template<class T>
	bool ray_intersect_disk(vec3<T> const& rayOrigin, vec3<T> const& rayDir, 
		vec3<T> const& circleCenter, vec3<T> const& circleNormal, T circleRadius,
		T& t);

	// Ray cylinder intersection test
	// Returns number of intersection points
	// rayOrigin,rayDir: ray origin and direction (Ray must be specified in cylinder object coordinates)
	// Cylinder center is (0,0,0) and cylinder axis is 0,0,1
	// length,radius: cylinder length and radius
	// t: reference 2-float array indicating distance to intersection points
	// intersectDisks: enables intersection with cone disks
	template<class T>
	int ray_intersect_cylinder(
		vec3<T> const& p, vec3<T> const& d,
		T length, T radius,
		T* t, bool intersectDisks);

	// Capsule center is (0,0,0) and capsule axis is 0,0,1
	template<class T>
	int ray_intersect_capsule(
		vec3<T> const& rayOrigin, vec3<T> const& rayDir,
		T height, T radius,
		T& t0, T& t1);


}//namespace math
