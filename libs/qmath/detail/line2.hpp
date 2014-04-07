namespace math
{


///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline line2<T>::line2() : start(0,0), end(1,1)
{
}

template <typename T>
inline line2<T>::line2(T xa, T ya, T xb, T yb) : start(xa, ya), end(xb, yb)
{

}

template <typename T>
inline line2<T>::line2(const vec2<T>& istart, const vec2<T>& iend) : start(istart), end(iend)
{
}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void line2<T>::set_line(const T& xa, const T& ya, const T& xb, const T& yb)
{
	start.set(xa, ya);
	end.set(xb, yb);
}

template <typename T>
inline void line2<T>::set_line(const vec2<T>& nstart, const vec2<T>& nend)
{
	start.set(nstart); 
	end.set(nend);
}

template <typename T>
inline void line2<T>::set_line(const line2<T>& line)
{
	start.set(line.start);
	end.set(line.end);
}

template <typename T>
inline T line2<T>::get_length() const
{
	return distance(start,end);
}

template <typename T>
inline T line2<T>::get_length_sq() const
{
	return distance_sq(start,end);
}

template <typename T>
inline vec2<T> line2<T>::get_middle() const
{
	return (start + end) * (T)0.5;
}

template <typename T>
inline vec2<T> line2<T>::get_vector() const
{
	return end - start;
}

template <typename T>
inline vec2<T> line2<T>::get_vector_normalized() const
{
	vec2<T> start64(start.x, start.y);
	vec2<T> end64(end.x, end.y);

	vec2<T> dir64 = end64 - start64;
	dir64.normalize();

	return vec2<T>((T)dir64.x, (T)dir64.y);
}

///////////////////////////////////////////////////////////////////////////////
// intersection
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline bool line2<T>::is_point_between_start_and_end(const vec2<T>& point) const
{
	return isBetweenPoints(point, start, end);
}

template <typename T>
inline vec2<T> line2<T>::get_closest_point(const vec2<T>& point) const
{
	vec2<T> c = point - start;
	vec2<T> v = end - start;
	T d = (T)math::length(v);
	v /= d;
	T t = dot(v, c);

	if (t < (T)0.0)
	{
		return start;
	}
	if (t > d)
	{
		return end;
	}

	v *= t;
	return start + v;
}

template <typename T>
bool line2<T>::get_intersection_with_line(const line2<T>& other, vec2<T>* point, T* outMu)
{
	//http://www.faqs.org/faqs/graphics/algorithms-faq/
	//this line is from point n0 to point n1
	//the other line is from point m0 to point m1
	const vec2<T>& n0 = start;
	const vec2<T>& n1 = end;
	const vec2<T>& m0 = other.start;
	const vec2<T>& m1 = other.end;

	vec2<T> n0n1 = n1-n0;
	vec2<T> m1m0 = m0-m1;
	T D0 = cross(n0n1,m1m0);
	if (is_zero(D0))
	{
		// Segments are parallel
		return false;
	}
	else
	{
		vec2<T> n0m0 = m0-n0;
		T D1 = cross(n0m0,m1m0);
		T mu = D1 / D0;
		if (mu >= (T)0 && mu <= (T)1)
		{
			T D2 = cross(n0n1,n0m0);
			T rb = D2 / D0;
			if (rb >= (T)0 && rb <= (T)1)
			{
				if (point)
					*point = n0 + n0n1 * mu;
				if (outMu)
					*outMu = mu;
				return true;
			}
		}
		return false;
	}
}

template <typename T>
inline bool line2<T>::get_intersection_with_circle(vec2<T> sorigin, T sradius, T* outdistance) const
{
	const vec2<T> q = sorigin - start;
	T c = math::length(q);
	T v = dot(q, get_vector().normalize());
	T d = sradius * sradius - (c*c - v*v);

	if (d < 0.0)
		return false;

	if (outdistance)
		*outdistance = v - sqrt(d);
	return true;
}

template <typename T>
inline bool line2<T>::get_intersection_with_rect(const rect<T>& rect, vec2<T>* point, T* outMu)
{
	vec2<T> ul = rect.upperLeftCorner;
	vec2<T> ur = vec2<T>(rect.lowerRightCorner.x, rect.upperLeftCorner.y);
	vec2<T> ll = vec2<T>(rect.upperLeftCorner.x, rect.lowerRightCorner.y);
	vec2<T> lr = rect.lowerRightCorner;

	return get_intersection_with_line(line2<T>(ul, ur), point, outMu)
		|| get_intersection_with_line(line2<T>(ur, lr), point, outMu)
		|| get_intersection_with_line(line2<T>(lr, ll), point, outMu)
		|| get_intersection_with_line(line2<T>(ll, ul), point, outMu);
}

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline line2<T> line2<T>::operator+(const vec2<T>& point) const
{
	return line2<T>(start + point, end + point);
}

template <typename T>
inline line2<T> line2<T>::operator-(const vec2<T>& point) const
{
	return line2<T>(start - point,end - point);
}

template <typename T>
inline line2<T>& line2<T>::operator+=(const vec2<T>& point)
{
	start += point;
	end += point;
	return *this;
}

template <typename T>
inline line2<T>& line2<T>::operator-=(const vec2<T>& point)
{
	start -= point;
	end -= point;
	return *this;
}

template <typename T>
inline bool line2<T>::operator==(const line2<T>& other) const
{
	return (start==other.start && end==other.end) || (end==other.start && start==other.end);
}

template <typename T>
inline bool line2<T>::operator!=(const line2<T>& other) const
{
	return !(*this == other);
}


}