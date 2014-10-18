namespace math
{

	///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
template <typename T>
aabb2<T>::aabb2(): minPoint(-1,-1), maxPoint(1,1)
{
}

template <typename T>
aabb2<T>::aabb2(const vec2<T>& minp, const vec2<T>& maxp) : minPoint(minp), maxPoint(maxp)
{
}
	
template <typename T>
aabb2<T>::aabb2(const vec2<T>& init): minPoint(init), maxPoint(init)
{
}
	
template <typename T>
aabb2<T>::aabb2(T minx, T miny, T maxx, T maxy): minPoint(minx, miny), maxPoint(maxx, maxy)
{
}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void aabb2<T>::add_internal_point(const vec2<T>& p)
{
	add_internal_point(p.x, p.y);
}

template <typename T>
void aabb2<T>::add_internal_box(const aabb2<T>& b)
{
	add_internal_point(b.maxPoint);
	add_internal_point(b.minPoint);
}

template <typename T>
void aabb2<T>::add_internal_box(const mat3<T>& mat, const aabb2<T>& b)
{
	aabb2<T> tmp;
	tmp.reset(mat, b);
	add_internal_box(tmp);
}

template <typename T>
void aabb2<T>::reset(T x, T y)
{
	maxPoint.set(x,y);
	minPoint = maxPoint;
}

template <typename T>
void aabb2<T>::reset(const aabb2<T>& initValue)
{
	*this = initValue;
}

template <typename T>
void aabb2<T>::reset(const vec2<T>& minp, const vec2<T>& maxp)
{
	minPoint = minp;
	maxPoint = maxp;
}

template <typename T>
void aabb2<T>::reset(const vec2<T>& initValue)
{
	maxPoint = initValue;
	minPoint = initValue;
}

template <typename T>
void aabb2<T>::reset(const mat3<T>& mat, const aabb2<T>& b)
{
	// compute projected radius interval
	vec2<T> center = b.getCenter();
	vec2<T> hs = b.getExtent() * 0.5f;
	vec2<T> tmp;
	tmp.x = abs<T>(mat.m[0])*hs.x + abs<T>(mat.m[1])*hs.y;
	tmp.y = abs<T>(mat.m[3])*hs.x + abs<T>(mat.m[4])*hs.y;

	center = mat * center;

	maxPoint = (center + tmp);
	minPoint = (center - tmp);
}

template <typename T>
vec2<T> aabb2<T>::get_extent() const
{
	return maxPoint - minPoint;
}

template <typename T>
T aabb2<T>::get_radius() const
{
	return length(get_extent()) * 0.5f;
}

template <typename T>
T aabb2<T>::get_radius_fast() const
{
	float radiusSq = length_sq(get_extent()) * 0.25f;
	return sqrt(radiusSq);
}

template <typename T>
T aabb2<T>::get_radius_sq() const
{
	return length_sq(get_extent()) * 0.25f;
}

template <typename T>
T aabb2<T>::get_inner_radius() const
{
	float dx = maxPoint.x - minPoint.x;
	float dy = maxPoint.y - minPoint.y;
	float max = math::max(dx, dy);
	return max * 0.5f;
}

template <typename T>
T aabb2<T>::get_inner_radius_sq() const
{
	float r = get_inner_radius();
	return r * r;
}

template <typename T>
void aabb2<T>::get_edges(vec2<T> edges[4]) const
{
	const vec2<T> middle = get_center();
	const vec2<T> diag = middle - maxPoint;

	/*
	Edges are stored in this way:
	Hey, am I an ascii artist, or what? :) niko.
          1---------2
          |         |
          |         |
          |         |
          0---------3
	*/

	edges[0] = vec2<T>(middle.x - diag.x, middle.y - diag.y);
	edges[1] = vec2<T>(middle.x - diag.x, middle.y + diag.y);
	edges[2] = vec2<T>(middle.x + diag.x, middle.y + diag.y);
	edges[3] = vec2<T>(middle.x + diag.x, middle.y - diag.y);
}

template <typename T>
bool aabb2<T>::is_empty() const
{
	return equals(minPoint, maxPoint);
}

template <typename T>
void aabb2<T>::repair()
{
	T t;

	if (minPoint.x > maxPoint.x)
		{ t=minPoint.x; minPoint.x = maxPoint.x; maxPoint.x=t; }
	if (minPoint.y > maxPoint.y)
		{ t=minPoint.y; minPoint.y = maxPoint.y; maxPoint.y=t; }
}

template <typename T>
aabb2<T> aabb2<T>::get_interpolated(const aabb2<T>& other, float d) const
{
	float inv = 1.0f - d;
	return aabb2<T>((other.minPoint*inv) + (minPoint*d),
		(other.maxPoint*inv) + (maxPoint*d));
}

template <typename T>
vec2<T> aabb2<T>::get_center() const
{
	return (minPoint + maxPoint) * 0.5f;
}

template <typename T>
T aabb2<T>::get_area() const
{
	const vec2<T> e = get_extent();
	return e.x * e.y;
}

///////////////////////////////////////////////////////////////////////////////
// containment
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aabb2<T>::is_point_inside(const vec2<T>& p) const
{
	return (
		p.x >= minPoint.x  &&  p.x <= maxPoint.x  &&
		p.y >= minPoint.y  &&  p.y <= maxPoint.y);
}

template <typename T>
bool aabb2<T>::is_point_totally_inside(const vec2<T>& p) const
{
	return (
		p.x > minPoint.x  &&  p.x < maxPoint.x  &&
		p.y > minPoint.y  &&  p.y < maxPoint.y);
}

template <typename T>
void aabb2<T>::add_internal_point(T x, T y)
{
	if (x>maxPoint.x) maxPoint.x = x;
	if (y>maxPoint.y) maxPoint.y = y;

	if (x<minPoint.x) minPoint.x = x;
	if (y<minPoint.y) minPoint.y = y;
}



///////////////////////////////////////////////////////////////////////////////
// intersection
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aabb2<T>::intersect_with_box(const aabb2<T>& other) const
{
	//return (minPoint <= other.maxPoint && maxPoint >= other.minPoint);
	return minPoint.x <= other.maxPoint.x && maxPoint.x >= other.minPoint.x
	&& minPoint.y <= other.maxPoint.y && maxPoint.y >= other.minPoint.y;
}

template <typename T>
bool aabb2<T>::is_fully_inside(const aabb2<T>& other) const
{
	//return minPoint >= other.minPoint && maxPoint <= other.maxPoint;
	return minPoint.x >= other.minPoint.x && maxPoint.x <= other.maxPoint.x
	&& minPoint.y >= other.minPoint.y && maxPoint.y <= other.maxPoint.y;
}

template <typename T>
bool aabb2<T>::intersect_with_line(const vec2<T>& linemiddle, const vec2<T>& linevect, T halflength) const
{
	const vec2<T> e = get_extent() * (T)0.5;
	const vec2<T> t = get_center() - linemiddle;

	if (
		(abs(t.x) > e.x + halflength * abs(linevect.x))  ||
		(abs(t.y) > e.y + halflength * abs(linevect.y)))
	{
		return false;
	}

	T r = e.x * (T)abs(linevect.y) + e.y * (T)abs(linevect.x);
	if (abs(t.x*linevect.y - t.y*linevect.x) > r)
	{
		return false;
	}

	return true;
}

template <typename T>
bool aabb2<T>::intersect_with_line(const line2<T>& line) const
{
	return intersect_with_line(line.get_middle(),
	line.get_vector_normalized(),
	line.get_length() * 0.5f);
}

template <typename T>
bool aabb2<T>::intersect_with_ray(const line2<T>& ray, vec2<T>& outIntersectionNear, vec2<T>& outIntersectionFar) const
{
	const vec2<T>& minBounds = minPoint;
	const vec2<T>& maxBounds = maxPoint;

	const vec2<T>& rayOrigin    = ray.start;
	const vec2<T>& rayDirection = ray.get_vector_normalized();

	T tnear = -std::numeric_limits<T>::max();
	T tfar  =  std::numeric_limits<T>::max();

	for (uint8_t i = 0; i < 2; ++i)
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
bool aabb2<T>::operator==(const aabb2<T>& other) const
{
	return (minPoint == other.minPoint && other.maxPoint == maxPoint);
}

template <typename T>
bool aabb2<T>::operator!=(const aabb2<T>& other) const
{
	return !(*this == other);
}

}