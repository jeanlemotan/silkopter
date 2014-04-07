namespace math
{


///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
triangle3<T>::triangle3() {}

template <typename T>
triangle3<T>::triangle3(vec3<T> v1, vec3<T> v2, vec3<T> v3) : pointA(v1), pointB(v2), pointC(v3) {}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool triangle3<T>::operator==(const triangle3<T>& other) const
{
	return other.pointA == pointA && other.pointB == pointB && other.pointC == pointC;
}

template <typename T>
bool triangle3<T>::operator!=(const triangle3<T>& other) const
{
	return !(*this==other);
}

template <typename T>
bool triangle3<T>::is_total_inside_box(const aabb3<T>& box) const
{
	return (box.is_point_inside(pointA) &&
		box.is_point_inside(pointB) &&
		box.is_point_inside(pointC));
}

template <typename T>
vec3<T> triangle3<T>::closest_point_on_triangle(const vec3<T>& p) const
{
	const vec3<T> rab = line3<T>(pointA, pointB).get_closest_point(p);
	const vec3<T> rbc = line3<T>(pointB, pointC).get_closest_point(p);
	const vec3<T> rca = line3<T>(pointC, pointA).get_closest_point(p);

	const T d1 = distance(rab, p);
	const T d2 = distance(rbc, p);
	const T d3 = distance(rca, p);

	if (d1 < d2)
		return d1 < d3 ? rab : rca;

	return d2 < d3 ? rbc : rca;
}

template <typename T>
bool triangle3<T>::is_point_inside(const vec3<T>& p) const
{
	return (is_on_same_side(p, pointA, pointB, pointC) &&
		is_on_same_side(p, pointB, pointA, pointC) &&
		is_on_same_side(p, pointC, pointA, pointB));
}

template <typename T>
bool triangle3<T>::is_point_inside_fast(const vec3<T>& p) const
{
	const vec3<T> f = pointB - pointA;
	const vec3<T> g = pointC - pointA;

	const float a = dot(f, f);
	const float b = dot(f, g);
	const float c = dot(g, g);

	const vec3<T> vp = p - pointA;
	const float d = dot(vp, f);
	const float e = dot(vp, g);

	float x = (d*c)-(e*b);
	float y = (e*a)-(d*b);
	const float ac_bb = (a*c)-(b*b);
	float z = x+y-ac_bb;

#define IR(x) ((uint32_t&)(x))
	// return sign(z) && !(sign(x)||sign(y))
	return (( (IR(z)) & ~((IR(x))|(IR(y))) ) & 0x80000000)!=0;
#undef IR
}

template <typename T>
bool triangle3<T>::get_intersection_with_limited_line(const line3<T>& line, vec3<T>& outIntersection) const
{
	return get_intersection_with_line(line.start,
		line.get_vector(), outIntersection) &&
		line.is_point_between_start_and_end(outIntersection);
		//outIntersection.isBetweenPoints(line.start, line.end)
		
}

template <typename T>
bool triangle3<T>::get_intersection_with_line(const vec3<T>& linePoint, const vec3<T>& lineVect, vec3<T>& outIntersection) const
{
	if (get_intersection_of__plane_with_line(linePoint, lineVect, outIntersection))
		return is_point_inside(outIntersection);

	return false;
}

template <typename T>
bool triangle3<T>::get_intersection_with_line_fast(const vec3<T>& linePoint, const vec3<T>& lineVect, vec3<T>& outIntersection) const
{
	T t,u,v;
	bool ret = intersect_ray(linePoint,lineVect,false,t,u,v);
	if (ret)
	{
		// calculate intersection point
		outIntersection = linePoint + t*lineVect;
	}
	return ret;
}

template <typename T>
bool triangle3<T>::intersect_ray(const vec3<T>& linePoint, const vec3<T>& lineVect, bool cullbackface, T& t, T& u, T& v) const
{
	// every point inside a triangle is given by the explicit formula
	//	t(u,v) = o + t*d = (1-u-v)v0 + u.v1 + v.v2)
	T det, inv_det;
	vec3<T> pvec, tvec, qvec;

	// find vectors for two edges sharing vert0
	const vec3<T>& edge1 = pointB - pointA;
	const vec3<T>& edge2 =  pointC - pointA;

	// backface culling
	if (cullbackface && dot(cross(edge1, edge2), linePoint - pointA) < 0.0)
	{
		return false;
	}

	// begin calculating determinant - also used to calculate U parameter
	pvec = cross(lineVect, edge2);

	// if determinant is near zero, ray lies in plane of triangle
	det = dot(edge1, pvec);
	if (is_zero(det))
		return false;
	inv_det = T(1) / det;

	// calculate distance from vert0 to ray origin
	tvec = linePoint - pointA;

	// calculate U parameter and test bounds
	u = dot(tvec,pvec) * inv_det;
	if (u < 0 || u > T(1))
		return false;

	// prepare to test V parameter 
	qvec = cross(tvec, edge1);

	// calculate V parameter and test bounds
	v = dot(lineVect, qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f)
		return false;

	// calculate t, ray intersects triangle
	t = dot(edge2, qvec) * inv_det;
	return true;
}

template <typename T>
bool triangle3<T>::get_intersection_of__plane_with_line(const vec3<T>& linePoint,
	const vec3<T>& lineVect, vec3<T>& outIntersection) const
{
	const vec3<T> normal = normalized(get_normal());
	T t2 = dot(normal,lineVect);
	if (is_zero(t2))
		return false;

	T d = dot(pointA,normal);
	T t = -(dot(normal,linePoint) - d) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}

template <typename T>
bool triangle3<T>::get_intersection_of_plane_with_line_fast(const vec3<T>& linePoint,
	const vec3<T>& lineVect, vec3<T>& outIntersection) const
{
	const vec3<T> normal = get_normal().normalize();
	T t2 = dot(normal,lineVect);

	if (is_zero(t2))
		return false;

	T d = dot(pointA, normal);
	T t = -(dot(normal, linePoint) - d) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}

template <typename T>
vec3<T> triangle3<T>::get_normal() const
{
	return cross(pointB - pointA, pointC - pointA);
}

template <typename T>
bool triangle3<T>::is_front_facing(const vec3<T>& lookDirection) const
{
	const vec3<T> n = get_normal();
	n.normalize();
	const T d = dot(n, lookDirection);
	return F32_LOWER_EQUAL_0(d);
}

template <typename T>
plane<T> triangle3<T>::get_plane() const
{
	return plane<T>(pointA, pointB, pointC);
}

template <typename T>
T triangle3<T>::get_area() const
{
	return math::length(cross((pointB - pointA),pointC - pointA)) * 0.5f;
}

template <typename T>
T triangle3<T>::get_area_sq() const
{
	return length_sq(cross((pointB - pointA),pointC - pointA)) * 0.25f;
}


template <typename T>
void triangle3<T>::set(const vec3<T>& a, const vec3<T>& b, const vec3<T>& c)
{
	pointA = a;
	pointB = b;
	pointC = c;
}

template <typename T>
bool triangle3<T>::is_on_same_side(const vec3<T>& p1, const vec3<T>& p2,
	const vec3<T>& a, const vec3<T>& b) const
{
	vec3<T> bminusa = b - a;
	vec3<T> cp1 = cross(bminusa,p1 - a);
	vec3<T> cp2 = cross(bminusa,p2 - a);
	return (dot(cp1,cp2) >= 0.0f);
}


}