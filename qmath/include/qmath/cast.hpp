namespace math
{

template<typename T>
inline constexpr vec2<T>::vec2(const vec3<T>& v) : x(v.x), y(v.y) {}

template<typename T>
inline constexpr vec2<T>::vec2(const vec4<T>& v) : x(v.x), y(v.y) {}

template<typename T>
inline vec2<T>& vec2<T>::operator=(const vec3<T>& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator=(const vec4<T>& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
inline mat4<T>::mat4(const mat3<T>& v)
{
	*this = v;
}

template<typename T>
inline mat4<T>& mat4<T>::operator=(const mat3<T>& v)
{
	m[ 0] = v.m[0];
	m[ 1] = v.m[1];
	m[ 2] = v.m[2];
	m[ 3] = 0;

	m[ 4] = v.m[3];
	m[ 5] = v.m[4];
	m[ 6] = v.m[5];
	m[ 7] = 0;

	m[ 8] = v.m[6];
	m[ 9] = v.m[7];
	m[10] = v.m[8];
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
	return *this;
}

template<typename T>
inline mat4<T>::mat4(const mat2<T>& v)
{
	*this = v;
}

template<typename T>
inline mat4<T>& mat4<T>::operator=(const mat2<T>& v)
{
	m[ 0] = v.m[0];
	m[ 1] = v.m[1];
	m[ 2] = 0;
	m[ 3] = 0;

	m[ 4] = v.m[2];
	m[ 5] = v.m[3];
	m[ 6] = 0;
	m[ 7] = 0;

	m[ 8] = 0;
	m[ 9] = 0;
	m[10] = 1;
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
inline mat3<T>::mat3(const mat4<T>& v)
{
	*this = v;
}

template<typename T>
inline mat3<T>& mat3<T>::operator=(const mat4<T>& v)
{			    
	m[0] = v.m[ 0];
	m[1] = v.m[ 1];
	m[2] = v.m[ 2];

	m[3] = v.m[ 4];
	m[4] = v.m[ 5];
	m[5] = v.m[ 6];

	m[6] = v.m[ 8];
	m[7] = v.m[ 9];
	m[8] = v.m[10];
	return *this;
}

template<typename T>
inline mat3<T>::mat3(const mat2<T>& v)
{
	*this = v;
}

template<typename T>
inline mat3<T>& mat3<T>::operator=(const mat2<T>& v)
{			    
	m[0] = v.m[ 0];
	m[1] = v.m[ 1];
	m[2] = 0;

	m[3] = v.m[ 2];
	m[4] = v.m[ 3];
	m[5] = 0;

	m[6] = 0;
	m[7] = 0;
	m[8] = 1;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
inline mat2<T>::mat2(const mat3<T>& v)
{
	*this = v;
}

template<typename T>
inline mat2<T>& mat2<T>::operator=(const mat3<T>& v)
{			    
	m[0] = v.m[ 0];
	m[1] = v.m[ 1];

	m[2] = v.m[ 3];
	m[3] = v.m[ 4];
	return *this;
}

template<typename T>
inline mat2<T>::mat2(const mat4<T>& v)
{
	*this = v;
}

template<typename T>
inline mat2<T>& mat2<T>::operator=(const mat4<T>& v)
{			    
	*this = mat3<T>(v);
	return *this;
}


}