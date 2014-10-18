namespace math
{


///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
template <typename T>
aabb3<T>::aabb3(): minPoint(-1,-1,-1), maxPoint(1,1,1)
{
}

template <typename T>
aabb3<T>::aabb3(const vec3<T>& minp, const vec3<T>& maxp) : minPoint(minp), maxPoint(maxp)
{
}
	
template <typename T>
aabb3<T>::aabb3(const vec3<T>& init): minPoint(init), maxPoint(init)
{
}
	
template <typename T>
aabb3<T>::aabb3(T minx, T miny, T minz, T maxx, T maxy, T maxz): minPoint(minx, miny, minz), maxPoint(maxx, maxy, maxz)
{
}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void aabb3<T>::add_internal_point(const vec3<T>& p)
{
	add_internal_point(p.x, p.y, p.z);
}

template <typename T>
void aabb3<T>::add_internal_box(const aabb3<T>& b)
{
	add_internal_point(b.maxPoint);
	add_internal_point(b.minPoint);
}

template <typename T>
void aabb3<T>::add_internal_box(const mat4<T>& mat, const aabb3<T>& b)
{
	aabb3<T> tmp;
	tmp.reset(mat, b);
	add_internal_box(tmp);
}

template <typename T>
void aabb3<T>::reset(T x, T y, T z)
{
	maxPoint.set(x,y,z);
	minPoint = maxPoint;
}

template <typename T>
void aabb3<T>::reset(const aabb3<T>& initValue)
{
	*this = initValue;
}

template <typename T>
void aabb3<T>::reset(const vec3<T>& minp, const vec3<T>& maxp)
{
	minPoint = minp;
	maxPoint = maxp;
}

template <typename T>
void aabb3<T>::reset(const vec3<T>& initValue)
{
	maxPoint = initValue;
	minPoint = initValue;
}

template <typename T>
vec3<T> aabb3<T>::get_extent() const
{
	return maxPoint - minPoint;
}

template <typename T>
T aabb3<T>::get_radius() const
{
	return length(get_extent()) * 0.5f;
}

template <typename T>
T aabb3<T>::get_radius_fast() const
{
	return length(get_extent()) * T(0.5);
}

template <typename T>
T aabb3<T>::get_radius_sq() const
{
	return length_sq(get_extent()) * 0.25f;
}

template <typename T>
T aabb3<T>::get_inner_radius() const
{
	float dx = maxPoint.x - minPoint.x;
	float dy = maxPoint.y - minPoint.y;
	float dz = maxPoint.z - minPoint.z;
	float max = math::max(math::max(dx, dy), dz);
	return max * 0.5f;
}

template <typename T>
T aabb3<T>::get_inner_radius_sq() const
{
	float r = get_inner_radius();
	return r * r;
}

template <typename T>
void aabb3<T>::get_edges(vec3<T> edges[8]) const
{
	const vec3<T> middle = get_center();
	const vec3<T> diag = middle - maxPoint;

	/*
	Edges are stored in this way:
	Hey, am I an ascii artist, or what? :) niko.
            /3--------/7
           /  |      / |
          /   |     /  |
          1---------5  |
          |   2- - -| -6
          |  /      |  /
          |/        | /
          0---------4/
	*/

	edges[0] = vec3<T>(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
	edges[1] = vec3<T>(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
	edges[2] = vec3<T>(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
	edges[3] = vec3<T>(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
	edges[4] = vec3<T>(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
	edges[5] = vec3<T>(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
	edges[6] = vec3<T>(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
	edges[7] = vec3<T>(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
}

template <typename T>
bool aabb3<T>::is_empty() const
{
	return equals(minPoint, maxPoint);
}

template <typename T>
void aabb3<T>::repair()
{
	T t;

	if (minPoint.x > maxPoint.x)
		{ t=minPoint.x; minPoint.x = maxPoint.x; maxPoint.x=t; }
	if (minPoint.y > maxPoint.y)
		{ t=minPoint.y; minPoint.y = maxPoint.y; maxPoint.y=t; }
	if (minPoint.z > maxPoint.z)
		{ t=minPoint.z; minPoint.z = maxPoint.z; maxPoint.z=t; }
}

template <typename T>
aabb3<T> aabb3<T>::get_interpolaed(const aabb3<T>& other, float d) const
{
	float inv = 1.0f - d;
	return aabb3<T>((other.minPoint*inv) + (minPoint*d),
		(other.maxPoint*inv) + (maxPoint*d));
}

template <typename T>
vec3<T> aabb3<T>::get_center() const
{
	return (minPoint + maxPoint) * 0.5f;
}

template <typename T>
T aabb3<T>::get_volume() const
{
	const vec3<T> e = get_extent();
	return e.x * e.y * e.z;
}

template <typename T>
T aabb3<T>::get_area() const
{
	const vec3<T> e = get_extent();
	return 2*(e.x*e.y + e.x*e.z + e.y*e.z);
}

///////////////////////////////////////////////////////////////////////////////
// containment
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aabb3<T>::is_point_inside(const vec3<T>& p) const
{
	return (
		p.x >= minPoint.x  &&  p.x <= maxPoint.x  &&
		p.y >= minPoint.y  &&  p.y <= maxPoint.y  &&
		p.z >= minPoint.z  &&  p.z <= maxPoint.z);
}

template <typename T>
bool aabb3<T>::is_point_totally_inside(const vec3<T>& p) const
{
	return (
		p.x > minPoint.x  &&  p.x < maxPoint.x  &&
		p.y > minPoint.y  &&  p.y < maxPoint.y  &&
		p.z > minPoint.z  &&  p.z < maxPoint.z);
}

template <typename T>
void aabb3<T>::add_internal_point(T x, T y, T z)
{
	if (x>maxPoint.x) maxPoint.x = x;
	if (y>maxPoint.y) maxPoint.y = y;
	if (z>maxPoint.z) maxPoint.z = z;

	if (x<minPoint.x) minPoint.x = x;
	if (y<minPoint.y) minPoint.y = y;
	if (z<minPoint.z) minPoint.z = z;
}



///////////////////////////////////////////////////////////////////////////////
// intersection
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aabb3<T>::intersect_with_box(const aabb3<T>& other) const
{
	//return (minPoint <= other.maxPoint && maxPoint >= other.minPoint);
	return minPoint.x <= other.maxPoint.x && maxPoint.x >= other.minPoint.x
	&& minPoint.y <= other.maxPoint.y && maxPoint.y >= other.minPoint.y
	&& minPoint.z <= other.maxPoint.z && maxPoint.z >= other.minPoint.z;
}

template <typename T>
bool aabb3<T>::is_fully_inside(const aabb3<T>& other) const
{
	//return minPoint >= other.minPoint && maxPoint <= other.maxPoint;
	return minPoint.x >= other.minPoint.x && maxPoint.x <= other.maxPoint.x
	&& minPoint.y >= other.minPoint.y && maxPoint.y <= other.maxPoint.y
	&& minPoint.z >= other.minPoint.z && maxPoint.z <= other.maxPoint.z;
}

template <typename T>
bool aabb3<T>::intersect_with_line(const vec3<T>& linemiddle, const vec3<T>& linevect, T halflength) const
{
	const vec3<T> e = get_extent() * (T)0.5;
	const vec3<T> t = get_center() - linemiddle;

	if (
		(abs(t.x) > e.x + halflength * abs(linevect.x))  ||
		(abs(t.y) > e.y + halflength * abs(linevect.y))  ||
		(abs(t.z) > e.z + halflength * abs(linevect.z)))
		return false;

	T r = e.y * (T)abs(linevect.z) + e.z * (T)abs(linevect.y);
	if (abs(t.y*linevect.z - t.z*linevect.y) > r)
		return false;

	r = e.x * (T)abs(linevect.z) + e.z * (T)abs(linevect.x);
	if (abs(t.z*linevect.x - t.x*linevect.z) > r)
		return false;

	r = e.x * (T)abs(linevect.y) + e.y * (T)abs(linevect.x);
	if (abs(t.x*linevect.y - t.y*linevect.x) > r)
		return false;

	return true;
}

template <typename T>
bool aabb3<T>::intersect_with_line(const line3<T>& line) const
{
	return intersect_with_line(line.get_middle(),
	line.get_vector_normalized(),
	line.get_length() * 0.5f);
}

template <typename T>
bool aabb3<T>::intersect_with_ray(const line3<T>& ray, vec3<T>& outIntersectionNear, vec3<T>& outIntersectionFar) const
{
	const vec3<T>& minBounds = minPoint;
	const vec3<T>& maxBounds = maxPoint;

	const vec3<T>& rayOrigin    = ray.start;
	const vec3<T>& rayDirection = ray.get_vector_normalized();

	T tnear = -std::numeric_limits<T>::max();
	T tfar  =  std::numeric_limits<T>::max();

	for (uint8_t i = 0; i < 3; ++i)
	{
		T rayOriginI = rayOrigin[i];
		T rayDirectionI = rayDirection[i];
		T minBoundsI = minBounds[i];
		T maxBoundsI = maxBounds[i];
		if (rayDirectionI == 0) // the ray is parallel to the planes so: 
		{
			if (rayOriginI < minBoundsI  ||  rayOriginI > maxBoundsI) //origin not between planes
				return false;
		}
		else
		{
			T t1 = (minBoundsI - rayOriginI) / rayDirectionI; // time at which ray intersects minimum i plane
			T t2 = (maxBoundsI - rayOriginI) / rayDirectionI; // time at which ray intersects maximum i plane
			if (t1 > t2)
				std::swap( t1, t2 ); // make t1 the lower value
			if (t1 > tnear)
				tnear = t1; // update tnear
			if (t2 < tfar)
				tfar = t2; // update tfar
			if (tnear > tfar) //box is missed so return FALSE 
				return false;
			if (tfar < 0) //box is behind ray so return FALSE
				return false;
		}
	}

	outIntersectionNear = rayOrigin + rayDirection * tnear;
	outIntersectionFar  = rayOrigin + rayDirection * tfar;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aabb3<T>::operator==(const aabb3<T>& other) const
{
	return (minPoint == other.minPoint && other.maxPoint == maxPoint);
}

template <typename T>
bool aabb3<T>::operator!=(const aabb3<T>& other) const
{
	return !(*this == other);
}


}