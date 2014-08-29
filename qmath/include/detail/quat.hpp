namespace math
{

template<typename T> quat<T> const quat<T>::zero(0, 0, 0, 1);																   \
template<typename T> quat<T> const quat<T>::one(1, 1, 1, 1);																   \
template<typename T> quat<T> const quat<T>::identity;																		   \

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> inline quat<T>::quat() : x(0), y(0), z(0), w(1) {}
template <typename T> inline quat<T>::quat(ZUninitialized) {}
template <typename T> inline quat<T>::quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
template <typename T> inline quat<T>::quat(quat<T> const& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
template <typename T> template <typename U> 
	inline quat<T>::quat(quat<U> const& q) : x(T(q.x)), y(T(q.y)), z(T(q.z)), w(T(q.w)) {}

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
	assert(n != 0);
	n = inv_sqrt<T, Policy>(n);
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}
template <typename T> template <class Policy>
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
	mat3<T> m(mat3<T>::uninitialized);
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
inline void quat<T>::set_from_euler_xyz(vec3<T> const& angles)
{
	vec3<T> a(angles*T(0.5));
	vec3<T> s(vec3<T>::uninitialized), c(vec3<T>::uninitialized);
	sin_cos<T, Policy>(a, s, c);
	x = c.z*s.x*c.y + s.z*c.x*s.y;
	y = c.z*c.x*s.y - s.z*s.x*c.y;
	z = s.z*c.x*c.y + c.z*s.x*s.y;
	w = c.z*c.x*c.y - s.z*s.x*s.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_xzy(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<T, Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_yxz(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<T, Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_yzx(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<T, Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_zxy(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<T, Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
}
template <typename T> template <class Policy>
inline void quat<T>::set_from_euler_zyx(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<T, Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_xyz(vec3<T>& res) const
{
	T const s = T(2)*(z*x + w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = asin<T, Policy>(s);
			res.x = atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(x*x + y*y));
			res.z = atan2<T, Policy>(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
			res.y = -angle<T>::pi2;
			res.x = -atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
			res.z = 0;
		}
	}
	else
	{
		res.y = angle<T>::pi2;
		res.x = atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		res.z = 0;
	} 
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_xzy(vec3<T>& res) const
{
	T const s = -T(2)*(x*y - w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
			res.x = atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(z*z + x*x));
			res.z = asin<T, Policy>(s);
		}
		else
		{
			res.y = -atan2<T, Policy>(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
			res.x = 0;
			res.z = -angle<T>::pi2;
		}
	}
	else
	{
		res.y = -atan2<T, Policy>(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
		res.x = 0;
		res.z = angle<T>::pi2;
	} 
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_yxz(vec3<T>& res) const
{
	T const s = -T(2)*(z*y - w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(x*x + y*y));
			res.x = asin<T, Policy>(s);
			res.z = atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
			res.y = 0;
			res.x = -angle<T>::pi2;
			res.z = atan2<T, Policy>(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
		}
	}
	else
	{
		res.y = 0;
		res.x = angle<T>::pi2;
		res.z = atan2<T, Policy>(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
	} 
}
template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_yzx(vec3<T>& res) const
{
	T const s = T(2)*(x*y + w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<T, Policy>(-T(2)*(z*x - w*y), T(1) - T(2)*(z*z + y*y));
			res.x = atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
			res.z = asin<T, Policy>(s);
		}
		else
		{
			res.y = -atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
			res.x = 0;
			res.z = -angle<T>::pi2;
		}
	}
	else
	{
		res.y = atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
		res.x = 0;
		res.z = angle<T>::pi2;
	} 
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_zxy(vec3<T>& res) const
{
	T const s = T(2)*(z*y + w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<T, Policy>(-T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
			res.x = asin<T, Policy>(s);
			res.z = atan2<T, Policy>(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
			res.y = 0;
			res.x = -angle<T>::pi2;
			res.z = -atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
		}
	}
	else
	{
		res.y = 0;
		res.x = angle<T>::pi2;
		res.z = atan2<T, Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
	} 
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_euler_zyx(vec3<T>& res) const
{
	T const s = -T(2)*(z*x - w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = asin<T, Policy>(s);
			res.x = atan2<T, Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
			res.z = atan2<T, Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
			res.y = -angle<T>::pi2;
			res.x = 0;
			res.z = atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
		}
	}
	else
	{
		res.y = angle<T>::pi2;
		res.x = 0;
		res.z = -atan2<T, Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
	} 
}

//! axis must be unit length
//! angle in radians
template <typename T> template <class Policy>
inline void quat<T>::set_from_angle_axis(T angle, vec3<T> const& axis)
{
	T const fHalfAngle = T(0.5)*angle;
	T fSin;
	sin_cos<T, Policy>(fHalfAngle, fSin, w);
	x = fSin*axis.x;
	y = fSin*axis.y;
	z = fSin*axis.z;
}

template <typename T> template <class Policy>
inline void quat<T>::get_as_angle_axis(T& angle, vec3<T> &axis) const
{
	T const safeW = clamp(w, (T)-1, (T)1);
	T const scale = sqrt<T, Policy>((T)1 - safeW * safeW);
	angle = 2 * acos<T, Policy>(safeW);
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
	quat<T> tmp(uninitialized);
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