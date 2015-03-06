namespace math
{

template<typename T> quat<T> const quat<T>::zero(0, 0, 0, 1);																   \
template<typename T> quat<T> const quat<T>::one(1, 1, 1, 1);																   \
template<typename T> quat<T> const quat<T>::identity;																		   \

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> inline constexpr quat<T>::quat() : x(0), y(0), z(0), w(1) {}
template <typename T> inline constexpr quat<T>::quat(math::ZUninitialized) {}
template <typename T> inline constexpr quat<T>::quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
template <typename T> inline constexpr quat<T>::quat(quat<T> const& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
template <typename T> template <typename U>
inline constexpr quat<T>::quat(quat<U> const& q) : x(T(q.x)), y(T(q.y)), z(T(q.z)), w(T(q.w)) {}

template<typename T>
template<class Policy>
inline quat<T> quat<T>::look_at(vec3<T> const& front, vec3<T> const& up)
{
    auto mat = mat3<T>::template look_at<Policy>(front, up);
    quat<T> q;
    q.set_from_mat3(mat);
    return q;
}

template<typename T>
template<class Policy>
inline quat<T> quat<T>::from_axis_x(T const& a)
{
    quat<T> q(math::uninitialized);
    q.template set_from_euler_xyz<Policy>(a, T(), T());
    return q;
}

template<typename T>
template<class Policy>
inline quat<T> quat<T>::from_axis_y(T const& a)
{
    quat<T> q(math::uninitialized);
    q.template set_from_euler_xyz<Policy>(T(), a, T());
    return q;
}

template<typename T>
template<class Policy>
inline quat<T> quat<T>::from_axis_z(T const& a)
{
    quat<T> q(math::uninitialized);
    q.template set_from_euler_xyz<Policy>(T(), T(), a);
    return q;
}

template <typename T>
template<class Policy>
quat<T> quat<T>::from_a_to_b(vec3<T> const& a, vec3<T> const& b)
{
    vec3<T> axis = cross(a, b);
    T dot = math::dot(a, b);
    if (dot < (T)(-1.0) + std::numeric_limits<T>::epsilon())// vectors are parallel and facing in the opposite direction
    {
        // Try crossing with x axis.
        vec3<T> t = cross(a,vec3<T>(1, 0, 0));
        // If not ok, cross with y axis.
        if(is_zero(math::length<T, Policy>(t)))
        {
            t = cross(a,vec3<T>(0, 1, 0));
        }

        t.template normalize<Policy>();
        return quat<T>(t[0], t[1], t[2], 0);
    }

    // if vectors are parallel and are facing in the same direction
    //	the axis is zero and quaternion is the identity
    quat<T> result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (dot + 1.0f) * 0.5f);
    result.template normalize<Policy>();
    return result;
}

template <typename T>
template<class Policy>
quat<T> quat<T>::from_a_to_b(quat<T> const& a, quat<T> const& b)
{
    auto ia = a;
    ia.invert();
    return ia * b;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// sets new quat
template <typename T> inline void quat<T>::set(T _x, T _y, T _z, T _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template <typename T> template <class Policy>
inline void quat<T>::normalize()
{
	T n = x*x + y*y + z*z + w*w;
	n = inv_sqrt<T, Policy>(n);
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}
template <typename T>
inline void quat<T>::invert()
{
	x = -x;
	y = -y;
	z = -z;
}

template <typename T> inline void quat<T>::set_identity()
{
	x = (T)0;
	y = (T)0;
	z = (T)0;
	w = (T)1;
}

template <typename T> template <class Policy>
inline void quat<T>::set_from_mat3(mat3<T> const& mat)
{
	T trace = mat(0,0) + mat(1,1) + mat(2,2);
	if (trace > 0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		T fRoot = sqrt<T, Policy>(trace + 1);  // 2w
		w = T(0.5)*fRoot;
		fRoot = T(0.5)/fRoot;  // 1/(4w)
		x = (mat(1,2)-mat(2,1))*fRoot;
		y = (mat(2,0)-mat(0,2))*fRoot;
		z = (mat(0,1)-mat(1,0))*fRoot;
	}
	else
	{
		// |w| <= 1/2
		int i = 0;
		if (mat(1,1) > mat(0,0))
		{
			i = 1;
		}
		if (mat(2,2) > mat(i,i))
		{
			i = 2;
		}
		int j = (i+1) % 3;
		int k = (j+1) % 3;

		T fRoot = sqrt<T, Policy>(mat(i,i) - mat(j,j) - mat(k,k)+1);
		T* apfQuat[3] = { &x, &y, &z };
		*apfQuat[i] = T(0.5)*fRoot;
		fRoot = T(0.5)/fRoot;
		w = (mat(j,k)-mat(k,j))*fRoot;
		*apfQuat[j] = (mat(i,j)+mat(j,i))*fRoot;
		*apfQuat[k] = (mat(i,k)+mat(k,i))*fRoot;
	}
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_mat3(mat3<T>& ret) const
{
	T *r = ret.data();

	T const tx = x*T(2);
	T const ty = y*T(2);
	T const tz = z*T(2);
	T const txx = tx*x;
	T const tyy = ty*y;
	T const tzz = tz*z;
	T const txy = tx*y;
	T const txz = tx*z;
	T const txw = tx*w;
	T const tyz = ty*z;
	T const tyw = ty*w;
	T const tzw = tz*w;

	r[0] = 1 -	tyy - tzz;
	r[3] =		txy - tzw;
	r[6] =		txz + tyw;

	r[1] =		txy + tzw;
	r[4] = 1 -	txx - tzz;
	r[7] =		tyz - txw;

	r[2] =		txz - tyw;
	r[5] =		tyz + txw;
	r[8] = 1 -	tyy - txx;
}

template <typename T> template <class Policy>
inline mat3<T> quat<T>::get_as_mat3() const
{
	mat3<T> m(math::uninitialized);
	get_as_mat3<Policy>(m);
	return m;
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_mat3_and_inv(mat3<T>& ret, mat3<T>& inv) const
{
	T *r = ret.data();
	T *i = inv.data();

	T const tx = x*T(2);
	T const ty = y*T(2);
	T const tz = z*T(2);
	T const txx = tx*x;
	T const tyy = ty*y;
	T const tzz = tz*z;
	T const txy = tx*y;
	T const txz = tx*z;
	T const txw = tx*w;
	T const tyz = ty*z;
	T const tyw = ty*w;
	T const tzw = tz*w;

	r[0] = 1 -	tyy - tzz;
	r[3] =		txy - tzw;
	r[6] =		txz + tyw;

	r[1] =		txy + tzw;
	r[4] = 1 -	txx - tzz;
	r[7] =		tyz - txw;

	r[2] =		txz - tyw;
	r[5] =		tyz + txw;
	r[8] = 1 -	tyy - txx;


	i[0] = r[0];
	i[1] = r[3];
	i[2] = r[6];

	i[3] = r[1];
	i[4] = r[4];
	i[5] = r[7];

	i[6] = r[2];
	i[7] = r[5];
	i[8] = r[8];
}

template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_xyz(T const& ax, T const& ay, T const& az)
{
    vec3<T> a(ax*T(0.5), ay*T(0.5), az*T(0.5));
	vec3<T> s(math::uninitialized), c(math::uninitialized);
	sin_cos<T, Policy>(a, s, c);
	x = c.z*s.x*c.y + s.z*c.x*s.y;
	y = c.z*c.x*s.y - s.z*s.x*c.y;
	z = s.z*c.x*c.y + c.z*s.x*s.y;
	w = c.z*c.x*c.y - s.z*s.x*s.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_xzy(T const& ax, T const& ay, T const& az)
{
    vec3<T> a(ax*T(0.5), ay*T(0.5), az*T(0.5));
    vec3<T> s(math::uninitialized), c(math::uninitialized);
    sin_cos<T, Policy>(a, s, c);
    x = c.z*s.x*c.y - s.z*c.x*s.y;
    y = c.z*c.x*s.y - s.z*s.x*c.y;
    z = s.z*c.x*c.y + c.z*s.x*s.y;
    w = c.z*c.x*c.y + s.z*s.x*s.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_yxz(T const& ax, T const& ay, T const& az)
{
    vec3<T> a(ax*T(0.5), ay*T(0.5), az*T(0.5));
    vec3<T> s(math::uninitialized), c(math::uninitialized);
    sin_cos<T, Policy>(a, s, c);
    x = c.z*s.x*c.y + s.z*c.x*s.y;
    y = c.z*c.x*s.y - s.z*s.x*c.y;
    z = s.z*c.x*c.y - c.z*s.x*s.y;
    w = c.z*c.x*c.y + s.z*s.x*s.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_yzx(T const& ax, T const& ay, T const& az)
{
    vec3<T> a(ax*T(0.5), ay*T(0.5), az*T(0.5));
    vec3<T> s(math::uninitialized), c(math::uninitialized);
    sin_cos<T, Policy>(a, s, c);
    x = c.z*s.x*c.y + s.z*c.x*s.y;
    y = c.z*c.x*s.y + s.z*s.x*c.y;
    z = s.z*c.x*c.y - c.z*s.x*s.y;
    w = c.z*c.x*c.y - s.z*s.x*s.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_zxy(T const& ax, T const& ay, T const& az)
{
    vec3<T> a(ax*T(0.5), ay*T(0.5), az*T(0.5));
    vec3<T> s(math::uninitialized), c(math::uninitialized);
    sin_cos<T, Policy>(a, s, c);
    x = c.z*s.x*c.y - s.z*c.x*s.y;
    y = c.z*c.x*s.y + s.z*s.x*c.y;
    z = s.z*c.x*c.y + c.z*s.x*s.y;
    w = c.z*c.x*c.y - s.z*s.x*s.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_zyx(T const& ax, T const& ay, T const& az)
{
    vec3<T> a(ax*T(0.5), ay*T(0.5), az*T(0.5));
    vec3<T> s(math::uninitialized), c(math::uninitialized);
    sin_cos<T, Policy>(a, s, c);
    x = c.z*s.x*c.y - s.z*c.x*s.y;
    y = c.z*c.x*s.y + s.z*s.x*c.y;
    z = s.z*c.x*c.y - c.z*s.x*s.y;
    w = c.z*c.x*c.y + s.z*s.x*s.y;
}


template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_xyz(vec3<T> const& euler)
{
    set_from_euler_xyz(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_xzy(vec3<T> const& euler)
{
    set_from_euler_xzy(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_yxz(vec3<T> const& euler)
{
    set_from_euler_yxz(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_yzx(vec3<T> const& euler)
{
    set_from_euler_yzx(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_zxy(vec3<T> const& euler)
{
    set_from_euler_zxy(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_zyx(vec3<T> const& euler)
{
    set_from_euler_zyx(euler.x, euler.y, euler.z);
}


template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_xyz(T& ax, T& ay, T& az) const
{
	T const s = T(2)*(z*x + w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
            ay = asin<T, Policy>(s);
            ax = atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(x*x + y*y));
            az = atan2<T, Policy>(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
            ay = -angle<T>::pi2;
            ax = -atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
            az = 0;
		}
	}
	else
	{
        ay = angle<T>::pi2;
        ax = atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
        az = 0;
	} 
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_xzy(T& ax, T& ay, T& az) const
{
	T const s = -T(2)*(x*y - w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
            ay = atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
            ax = atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(z*z + x*x));
            az = asin<T, Policy>(s);
		}
		else
		{
            ay = -atan2<T, Policy>(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
            ax = 0;
            az = -angle<T>::pi2;
		}
	}
	else
	{
        ay = -atan2<T, Policy>(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
        ax = 0;
        az = angle<T>::pi2;
	} 
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_yxz(T& ax, T& ay, T& az) const
{
	T const s = -T(2)*(z*y - w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
            ay = atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(x*x + y*y));
            ax = asin<T, Policy>(s);
            az = atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
            ay = 0;
            ax = -angle<T>::pi2;
            az = atan2<T, Policy>(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
		}
	}
	else
	{
        ay = 0;
        ax = angle<T>::pi2;
        az = atan2<T, Policy>(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
	} 
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_yzx(T& ax, T& ay, T& az) const
{
	T const s = T(2)*(x*y + w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
            ay = atan2<T, Policy>(-T(2)*(z*x - w*y), T(1) - T(2)*(z*z + y*y));
            ax = atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
            az = asin<T, Policy>(s);
		}
		else
		{
            ay = -atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
            ax = 0;
            az = -angle<T>::pi2;
		}
	}
	else
	{
        ay = atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
        ax = 0;
        az = angle<T>::pi2;
	} 
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_zxy(T& ax, T& ay, T& az) const
{
	T const s = T(2)*(z*y + w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
            ay = atan2<T, Policy>(-T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
            ax = asin<T, Policy>(s);
            az = atan2<T, Policy>(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
            ay = 0;
            ax = -angle<T>::pi2;
            az = -atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
		}
	}
	else
	{
        ay = 0;
        ax = angle<T>::pi2;
        az = atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
	} 
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_zyx(T& ax, T& ay, T& az) const
{
	T const s = -T(2)*(z*x - w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
            ay = asin<T, Policy>(s);
            ax = atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
            az = atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
            ay = -angle<T>::pi2;
            ax = 0;
            az = atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
		}
	}
	else
	{
        ay = angle<T>::pi2;
        ax = 0;
        az = -atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
	} 
}


template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_xyz(vec3<T>& euler) const
{
    get_as_euler_xyz(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_xzy(vec3<T>& euler) const
{
    get_as_euler_xzy(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_yxz(vec3<T>& euler) const
{
    get_as_euler_yxz(euler.x, euler.y, euler.z);
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_yzx(vec3<T>& euler) const
{
    get_as_euler_yzx(euler.x, euler.y, euler.z);
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_zxy(vec3<T>& euler) const
{
    get_as_euler_zxy(euler.x, euler.y, euler.z);
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_zyx(vec3<T>& euler) const
{
    get_as_euler_zyx(euler.x, euler.y, euler.z);
}


//! axis must be unit length
//! angle in radians
template <typename T> template <class Policy>
inline void quat<T>::set_from_angle_axis(T const& angle, vec3<T> const& axis)
{
    T const half = T(0.5)*angle;
    T s;
    sin_cos<T, Policy>(half, s, w);
    x = s*axis.x;
    y = s*axis.y;
    z = s*axis.z;
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_angle_axis(T& angle, vec3<T> &axis) const
{
    T const safe_w = clamp(w, (T)-1, (T)1);
    T const scale = sqrt<T, Policy>((T)1 - safe_w * safe_w);
    angle = acos<T, Policy>(safe_w) * 2;
	if (scale < std::numeric_limits<T>::epsilon())
	{
		axis.x = x;
		axis.y = y;
		axis.z = z;
	}
	else
	{
		T const invscale = 1 / scale;
		axis.x = x * invscale;
		axis.y = y * invscale;
		axis.z = z * invscale;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline bool quat<T>::operator==(quat<T> const& v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

template<typename T> inline bool quat<T>::operator!=(quat<T> const& v) const
{
	return !operator==(v);
}

template<typename T> inline quat<T>& quat<T>::operator=(quat<T> const& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

template<typename T> inline quat<T> quat<T>::operator+(quat<T> const& b) const
{
	return quat<T>(x + b.x, y + b.y, z + b.z, w + b.w);
}
template<typename T> inline quat<T>& quat<T>::operator+=(quat<T> const& b)
{
	x += b.x;
	y += b.y;
	z += b.z;
	w += b.w;
	return *this;
}

template<typename T> inline quat<T> quat<T>::operator-(quat<T> const& b) const
{
	return quat<T>(x - b.x, y - b.y, z - b.z, w - b.w);
}
template<typename T> inline quat<T>& quat<T>::operator-=(quat<T> const& b)
{
	x -= b.x;
	y -= b.y;
	z -= b.z;
	w -= b.w;
	return *this;
}


template <typename T> inline quat<T> quat<T>::operator*(quat<T> const& other) const
{
	quat<T> tmp(math::uninitialized);
	tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
	tmp.x = (other.x * w) + (other.w * x) + (other.z * y) - (other.y * z);
	tmp.y = (other.y * w) + (other.w * y) + (other.x * z) - (other.z * x);
	tmp.z = (other.z * w) + (other.w * z) + (other.y * x) - (other.x * y);
	return tmp;
}
template <typename T> inline quat<T>& quat<T>::operator*=(quat<T> const& other)
{
	return (*this = (*this) * other);
}

template <typename T> template <typename U>
inline quat<T> quat<T>::operator*(U s) const
{
	return quat<T>((T)s*x, (T)s*y, (T)s*z, (T)s*w);
}

template <typename T> template <typename U>
inline quat<T>& quat<T>::operator*=(U s)
{
	x*=s;
	y*=s;
	z*=s;
	w*=s;
	return *this;
}

template <typename T> inline quat<T> quat<T>::operator-() const
{
	return quat<T>(-x, -y, -z, -w);
}

template <typename T> inline quat<T> quat<T>::operator~() const
{
	return quat<T>(-x, -y, -z, +w);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global methods
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> inline T dot(quat<T> const& q1, quat<T> const& q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}




}
