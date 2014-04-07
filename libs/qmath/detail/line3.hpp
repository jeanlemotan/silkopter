namespace math
{


///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline line3<T>::line3() : start(0,0,0), end(1,1,1)
{
}

template <typename T>
inline line3<T>::line3(T xa, T ya, T za, T xb, T yb, T zb) : start(xa, ya, za), end(xb, yb, zb)
{
}

template <typename T>
inline line3<T>::line3(const vec3<T>& istart, const vec3<T>& iend) : start(istart), end(iend)
{
}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void line3<T>::set_line(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb)
{
	start.set(xa, ya, za);
	end.set(xb, yb, zb);
}

template <typename T>
inline void line3<T>::set_line(const vec3<T>& nstart, const vec3<T>& nend)
{
	start = nstart;
	end = nend;
}

template <typename T>
inline void line3<T>::set_line(const line3<T>& line)
{
	start.set(line.start);
	end.set(line.end);
}

template <typename T>
inline T line3<T>::get_length() const
{
	return distance(start,end);
}

template <typename T>
inline T line3<T>::get_length_sq() const
{
	return distance_sq(start,end);
}

template <typename T>
inline vec3<T> line3<T>::get_middle() const
{
	return (start + end) * (T)0.5;
}

template <typename T>
inline vec3<T> line3<T>::get_vector() const
{
	return end - start;
}

template <typename T>
inline vec3<T> line3<T>::get_vector_normalized() const
{
	vec3<T> s(start);
	vec3<T> e(end);
	vec3<T> dir = normalized(e - s);
	return vec3<T>(dir);
}

template <typename T>
inline bool line3<T>::is_between_points(const vec3<T>& point, const vec3<T>& begin, const vec3<T>& stop) const
{
	const T f = length_sq(stop - begin);
	return distance_sq(point, begin) <= f && distance_sq(point, stop) <= f;
}


template <typename T>
inline bool line3<T>::is_point_between_start_and_end(const vec3<T>& point) const
{
	return is_between_points(point, start, end);
}

template <typename T>
inline vec3<T> line3<T>::get_closest_point(const vec3<T>& point) const
{
	vec3<T> c = point - start;
	vec3<T> v = end - start;
	T d = (T)math::length(v);
	if (is_zero(d))// if line is a point, then the closest point is the point itself
	{
		return start;
	}
	v /= d;
	T t = dot(v,c);

	if (t < (T)0.0)
		return start;
	if (t > d)
		return end;

	v *= t;
	return start + v;
}

template <typename T>
inline T line3<T>::get_closest_point_projected(const vec3<T>& point) const
{
	vec3<T> c = point - start;
	vec3<T> v = end - start;
	T d = (T)math::length(v);
	v /= d;
	T t = dot(v,c);

	if (t < (T)0)
		return (T)0;
	if (t > d)
		return (T)1;
	else
		return t / d;
}

template <typename T>
inline bool line3<T>::is_intersetion_with_sphere(const vec3<T>& sphereCenter, T sphereRadius) const
{
	const vec3<T> q = sphereCenter - start;
	T c = length_sq(q);
	T v = dot(q, get_vector_normalized());
	T d = sphereRadius * sphereRadius - (c - v*v);
	return d > 0;
}

template <typename T>
inline bool line3<T>::get_intersection_with_sphere(const vec3<T>& sphereCenter, T sphereRadius, T& outdistance) const
{
	const vec3<T> q = sphereCenter - start;
	T c = length_sq(q);
	T v = dot(q, get_vector_normalized());
	T d = sphereRadius * sphereRadius - (c - v*v);

	if (d < 0.0)
		return false;

	outdistance = v - sqrt(d);
	return true;
}

template <typename T>
inline bool line3<T>::get_intersection_with_sphere( const vec3<T>& sphereCenter, T sphereRadius, T& outdistance1, T& outdistance2 ) const
{
	// http://www.devmaster.net/wiki/Ray-sphere_intersection
	const vec3<T> D = get_vector_normalized();

	vec3<T> v = start - sphereCenter;
	T b = T(2) * dot(v,D);
	T c = -sphereRadius*sphereRadius + dot(v,v);
	T d = b * b - T(4) * c;

	if( d >= (T)0 )
	{
		d = sqrt( d );
		outdistance1 = ( -b - d ) * T(0.5);
		outdistance2 = ( -b + d ) * T(0.5);
		return true;
	}

	return false;
}

template <typename T>
inline bool line3<T>::is_point_on_line(const vec3<T>& point) const
{
	/*the point is on the line if there exists some t such that
	start + getVector()*t==point =>
	getVector()*t==point-start =>
	getVector().normalize()==(point-start).normalize()
	we also need to account for a negative T, however
	*/
	vec3<T> vec1(get_vector().normalize());
	vec3<T> vec2((point - start).normalize());

	bool onInfiniteLine = equals(vec1, vec2) || equals(vec1, -vec2);	
	return onInfiniteLine;	
}

//returns true if there is an intersection and fills outPoint
//algorithm taken from http://www.mc.edu/campus/users/travis/maa/proceedings/spring2001/bard.himel.pdf
template <typename T>
inline bool line3<T>::get_intersection_with_line(const line3<T>& other, vec3<T>& intersection) const
{
	//handle the case of being parallel separately
	vec3<T> dir = get_vector().normalize();
	vec3<T> otherDir = other.getVector().normalize();

	T dotf = dot(dir, otherDir);

	if(equals(abs(dotf - 1.0f), 0.0f))
	{
		//std::cout<<"getIntersection(): is parallel\n";
		//the lines are parallel, special case
		if(!is_point_on_line(other.start))
		{
			//std::cout<<"doesn't lie on infinite line\n";
			return false;
		}
		//there are multiple intersection points
		//we don't try to figure out which one the user wants,
		//and just give him the first one we find
		if(is_point_between_start_and_end(other.start))
		{
			intersection = other.start;
			return true;
		}
		if(is_point_between_start_and_end(other.end))
		{
			intersection = other.end;
			return true;
		}
		if(other.isPointBetweenStartAndEnd(start))
		{
			intersection = start;
			return true;
		}
		//std::cout<<"none were between start and end\n";
		return false;
	}
	else
	{
		T A = length_sq(get_vector());

		T B = 2*(dot(get_vector(), start) - dot(get_vector(), other.start));
		T C = 2*(dot(get_vector(), other.getVector()));
		T D = 2*(dot(other.getVector(), other.start) - dot(other.getVector(), start));
		T E = length_sq(other.getVector());
		T s = (2*A*D+B*C) / (C*C-4*A*E);
		T t=(C*s-B)/(2*A);
		vec3<T> point1 = start + get_vector() * t;
		vec3<T> point2 = other.start + other.getVector() * s;
		//std::cout<<"points are "<<point1<<",   "<<point2<<"\n";
		if(equals(point1, point2))
		{
			if(is_point_between_start_and_end(point1) && other.isPointBetweenStartAndEnd(point1))
			{
				intersection = point1;
				return true;
			}
			return false;

		}
		else
		{
			return false;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline line3<T> line3<T>::operator+(const vec3<T>& point) const
{
	return line3<T>(start + point, end + point);
}

template <typename T>
inline line3<T>& line3<T>::operator+=(const vec3<T>& point)
{
	start += point; 
	end += point;
	return *this;
}

template <typename T>
inline line3<T> line3<T>::operator-(const vec3<T>& point) const
{
	return line3<T>(start - point, end - point);
}

template <typename T>
inline line3<T>& line3<T>::operator-=(const vec3<T>& point)
{
	start -= point;
	end -= point;
	return *this;
}

template <typename T>
inline bool line3<T>::operator==(const line3<T>& other) const
{
	return (start==other.start && end==other.end) || (end==other.start && start==other.end);
}

template <typename T>
inline bool line3<T>::operator!=(const line3<T>& other) const
{
	return !(*this == other);
}


}