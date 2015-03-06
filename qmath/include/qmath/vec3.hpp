namespace math
{
template<typename T> vec3<T> const vec3<T>::zero(T(0));																		   \
template<typename T> vec3<T> const vec3<T>::one(T(1));																		   \

    template<typename T> inline constexpr vec3<T>::vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    template<typename T> inline constexpr vec3<T>::vec3(math::ZUninitialized) {}
    template<typename T> inline constexpr vec3<T>::vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    template<typename T> inline constexpr vec3<T>::vec3(T v) : x(v), y(v), z(v) {}
    template<typename T> inline constexpr vec3<T>::vec3(vec3<T> const& v) : x(v.x), y(v.y), z(v.z) {}
    template<typename T> inline constexpr vec3<T>::vec3(vec2<T> const& v) : x(v.x), y(v.y), z(0) {}
    template<typename T> inline constexpr vec3<T>::vec3(vec2<T> const& v, T z) : x(v.x), y(v.y), z(z) {}
    template<typename T> inline constexpr vec3<T>::vec3(vec4<T> const& v) : x(v.x), y(v.y), z(v.z) {}

    template<typename T> template<typename U> inline constexpr vec3<T>::vec3(vec3<U> const& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
    template<typename T> inline void vec3<T>::set(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	template<typename T> template<typename U> inline void vec3<T>::set(vec3<U> const& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	template <typename T> inline void vec3<T>::set_length(T len)
	{
		T oldLen = length(*this);
        QASSERT(oldLen != 0);
		*this *= (len/oldLen);
	}

	template <typename T> template <class Policy> inline void vec3<T>::normalize()
	{
		*this = normalized<T, Policy>(*this);
	}
	template<typename T> inline bool vec3<T>::operator==(vec3<T> const& v) const
	{
		return equals(*this, v);
	}

	template<typename T> inline bool vec3<T>::operator!=(vec3<T> const& v) const
	{
		return !equals(*this, v);
	}

	template<typename T> inline vec3<T> vec3<T>::operator-() const
	{
		return vec3<T>(-x, -y, -z);
	}

	template<typename T> inline T& vec3<T>::operator[](uint8_t i) 
	{
        QASSERT(i < 3);
		return ((T*)this)[i];
	}

	template<typename T> inline T const& vec3<T>::operator[](uint8_t i) const
	{
        QASSERT(i < 3);
		return ((T*)this)[i];
	}

	template<typename T> inline vec3<T> vec3<T>::operator+(T s) const
	{
		return vec3<T>(x + s, y + s, z + s);
	}
	template<typename T> inline vec3<T> vec3<T>::operator+(vec3<T> const& v) const
	{
		return vec3<T>(x + v.x, y + v.y, z + v.z);
	}

	template<typename T> inline vec3<T> vec3<T>::operator-(T s) const
	{
		return vec3<T>(x - s, y - s, z - s);
	}
	template<typename T> inline vec3<T> vec3<T>::operator-(vec3<T> const& v) const
	{
		return vec3<T>(x - v.x, y - v.y, z - v.z);
	}

	template<typename T> inline vec3<T> vec3<T>::operator*(T s) const
	{
		return vec3<T>(x * s, y * s, z * s);
	}

	template<typename T> inline vec3<T> vec3<T>::operator/(T s) const
	{
        QASSERT(s != T(0));
		return vec3<T>(x / s, y / s, z / s);
	}

	template<> inline vec3<float> vec3<float>::operator/(float s) const
	{
        QASSERT(s != float(0));
		float ts = float(1) / s;
		return vec3<float>(x * ts, y * ts, z * ts);
	}

	template<typename T> inline vec3<T>& vec3<T>::operator=(vec3<T> const& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator=(vec2<T> const& v)
	{
		x = v.x;
		y = v.y;
		z = 0;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator=(vec4<T> const& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator+=(T s)
	{
		x += s;
		y += s;
		z += s;
		return *this;
	}
	template<typename T> inline vec3<T>& vec3<T>::operator+=(vec3<T> const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator-=(T s)
	{
		x -= s;
		y -= s;
		z -= s;
		return *this;
	}
	template<typename T> inline vec3<T>& vec3<T>::operator-=(vec3<T> const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator*=(T s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	template<typename T> inline vec3<T>& vec3<T>::operator*=(vec3<T> const& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator/=(T s)
	{
        QASSERT(s != T(0));
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	template<> inline vec3<float>& vec3<float>::operator/=(float s)
	{
        QASSERT(s != float(0));
		float ts = float(1) / s;
		x *= ts;
		y *= ts;
		z *= ts;
		return *this;
	}

	template<typename T> inline vec3<T>& vec3<T>::operator/=(vec3<T> const& v)
	{
        QASSERT(v.x != T(0) && v.y != T(0) && v.z != T(0));
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	template<typename T> inline vec3<T> operator*(T s, vec3<T> const& v)
	{
		return v*s;
	}
	template<typename T> inline vec3<T> operator/(T s, vec3<T> const& v)
	{
		return vec3<T>(s) / v;
	}



	template<typename T> inline vec3<T> operator*(vec3<T> const& v0, vec3<T> const& v1)
	{
		return vec3<T>(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
	}

	template<typename T> inline vec3<T> operator/(vec3<T> const& u, vec3<T> const& v)
	{
        QASSERT(v.x != T(0) && v.y != T(0) && v.z != T(0));
		return vec3<T>(u.x / v.x, u.y / v.y, u.z / v.z);
	}


} //math

#if !defined ARDUINO

template<typename T> inline std::ostream& operator<<(std::ostream& stream, math::vec3<T> const& v)
{
	stream << v.x << ',' << v.y << ',' << v.z;
	return stream;
}
template<typename T> inline std::istream& operator>>(std::istream& stream, math::vec3<T>& v)
{
	char comma;
	stream >> v.x >> comma >> v.y >> comma >> v.z;
	return stream;
}

#endif
