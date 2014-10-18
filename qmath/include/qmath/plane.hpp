namespace math
{


///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
template <class T>
inline plane<T>::plane(): normal(0,0,1), d(0)
{
}
template <class T>
inline plane<T>::plane(const vec3<T>& ipoint, const vec3<T>& inormal)
{
	set_plane(ipoint,inormal);
}

template <class T>
inline plane<T>::plane(T px, T py, T pz, T nx, T ny, T nz) : normal(nx, ny, nz)
{
	recalculate_d(vec3<T>(px, py, pz));
}

template <class T>
inline plane<T>::plane(const vec3<T>& point1, const vec3<T>& point2, const vec3<T>& point3)
{
	set_plane(point1, point2, point3);
}

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

template <class T>
inline bool plane<T>::operator==(const plane<T>& other) const
{
	return equals(d,other.d) && equals(normal,other.normal);
}

template <class T>
inline bool plane<T>::operator!=(const plane<T>& other) const
{
	return !(*this == other);
}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <class T>
inline void plane<T>::set_plane(const vec3<T>& ipoint, const vec3<T>& inormal)
{
	normal = inormal;
	recalculate_d(ipoint);
}

template <class T>
inline void plane<T>::set_plane(const vec3<T>& inormal, T id)
{
	normal = inormal;
	d = id;
}

template <class T>
inline void plane<T>::set_plane(const vec3<T>& point1, const vec3<T>& point2, const vec3<T>& point3)
{
	// creates the plane from 3 memberpoints
	normal = cross(point2 - point1,point3 - point1);
	normal.normalize();
	recalculate_d(point1);
}


template <class T>
inline void plane<T>::recalculate_d(const vec3<T>& ipoint)
{
	d = - dot(ipoint, normal);
}

template <class T>
inline vec3<T> plane<T>::get_member_point() const
{
	return normal * -d;
}


template <class T>
inline bool plane<T>::is_front_facing(const vec3<T>& lookDirection) const
{
	const T dt = dot(normal, lookDirection);
	return F32_LOWER_EQUAL_0(dt);
}

template <class T>
inline T plane<T>::get_distance_to(const vec3<T>& point) const
{
	return dot(point,normal) + d;
}

template <class T>
vec3<T> plane<T>::project_point(const vec3<T>& point) const
{
	T dist = get_distance_to(point);
	vec3<T> result = point - normal * dist;
	return result;
}

template <class T>
plane<T>& plane<T>::invert()
{
	plane<T> tmp = *this;
	tmp.normal = -normal;
	tmp.d = -dot(tmp.normal,get_member_point());
	*this = tmp;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// intersection methods
///////////////////////////////////////////////////////////////////////////////

template <class T>
inline bool plane<T>::get_intersection_with_line(const vec3<T>& linePoint, const vec3<T>& lineVect, vec3<T>& outIntersection) const
{
	T t2 = dot(normal,lineVect);

	if (t2 == 0)
		return false;

	T t =- (dot(normal,linePoint) + d) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}

template <class T>
inline float plane<T>::get_known_intersection_with_line(const vec3<T>& linePoint1, const vec3<T>& linePoint2) const
{
	vec3<T> vect = linePoint2 - linePoint1;
	T t2 = (float)dot(normal,vect);
	return (float)-((dot(normal,linePoint1) + d) / t2);
}

template <class T>
inline bool plane<T>::get_intersection_with_limited_line(const vec3<T>& linePoint1, const vec3<T>& linePoint2, vec3<T>& outIntersection) const
{
	return (
		get_intersection_with_line(linePoint1, linePoint2 - linePoint1, outIntersection) && outIntersection.IsBetweenPoints(linePoint1, linePoint2)
		);
}

template <class T>
inline RelationEnum plane<T>::classify_point_relation(const vec3<T>& point) const
{
	const T dist = dot(normal,point) + d;

	if (dist < -std::numeric_limits<T>::epsilon())
		return k_relBack;

	if (dist > std::numeric_limits<T>::epsilon())
		return k_relFront;

	return k_relPlanar;
}

template <class T>
inline bool plane<T>::exists_intersection(const plane<T>& other) const
{
	vec3<T> cross = cross(other.normal,normal);
	return math::length(cross) > std::numeric_limits<T>::epsilon();
}

template <class T>
inline bool plane<T>::get_intersection_with_plane(const plane<T>& other, vec3<T>& outLinePoint, vec3<T>& outLineVect) const
{
	const T fn00 = math::length(normal);
	const T fn01 = dot(normal,other.normal);
	const T fn11 = math::length(other.normal);
	const T det = fn00*fn11 - fn01*fn01;

	if (abs(det) < std::numeric_limits<T>::epsilon() )
		return false;

	const T invdet = 1.0 / det;
	const T fc0 = (fn11*-d + fn01*other.d) * invdet;
	const T fc1 = (fn00*-other.d + fn01*d) * invdet;

	outLineVect = cross(normal,other.normal);
	outLinePoint = normal*(T)fc0 + other.normal*(T)fc1;
	return true;
}

template <class T>
inline bool plane<T>::get_intersection_with_planes(const plane<T>& o1, const plane<T>& o2, vec3<T>& outPoint) const
{
	vec3<T> linePoint, lineVect;
	if (get_intersection_with_plane(o1, linePoint, lineVect))
		return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// overlap methods
///////////////////////////////////////////////////////////////////////////////

template <class T>
int8_t plane<T>::overlap(const vec3<T>& boxCenter, vec3<T>& boxHalfsize) const
{
	const T tmp[3] = {
		abs( boxHalfsize.x * normal.x),
		abs( boxHalfsize.y * normal.y),
		abs( boxHalfsize.z * normal.z)
	};

	float radius = tmp[0]+tmp[1]+tmp[2];
	float distance = get_distance_to(boxCenter);
	if (distance <= - radius) return 0;
	else if (distance <= radius) return 2;
	else return 1;
}


}