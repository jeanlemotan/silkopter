namespace math
{

	//////////////////////////////////////////////////////////////////////////
	// packet2
	template<typename T> MATH_FORCE_INLINE packet2<T>::packet2(T _x, T _y) : x(_x), y(_y) {}
	template<typename T> MATH_FORCE_INLINE packet2<T>::packet2(T s) : x(s), y(s) {}
	template<typename T> template<typename U> MATH_FORCE_INLINE packet2<T>::packet2(packet2<U> const& v) : x(T(v.x)), y(T(v.y)) {}
	template<typename T> MATH_FORCE_INLINE packet2<T>::packet2(packet3<T> const& v) : x(v.x), y(v.y) {}
	template<typename T> MATH_FORCE_INLINE packet2<T>::packet2(packet4<T> const& v) : x(v.x), y(v.y) {}
	template<typename T> MATH_FORCE_INLINE packet2<T>::packet2(vec2<T> const& v) : x(v.x), y(v.y) {}
	template<typename T> MATH_FORCE_INLINE packet2<T>& packet2<T>::operator=(vec2<T> const& v) { x = v.x; y = v.y; return *this; }
	template<typename T> MATH_FORCE_INLINE bool packet2<T>::operator==(packet2<T> const& v) const { return x == v.x && y == v.y; }
	template<typename T> MATH_FORCE_INLINE bool packet2<T>::operator!=(packet2<T> const& v) const { return !operator==(v); }

	//////////////////////////////////////////////////////////////////////////
	// packet3
	template<typename T> MATH_FORCE_INLINE packet3<T>::packet3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	template<typename T> MATH_FORCE_INLINE packet3<T>::packet3(T s) : x(s), y(s), z(s) {}
	template<typename T> template<typename U> MATH_FORCE_INLINE packet3<T>::packet3(packet3<U> const& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
	template<typename T> MATH_FORCE_INLINE packet3<T>::packet3(packet2<T> const& v) : x(v.x), y(v.y), z(0) {}
	template<typename T> MATH_FORCE_INLINE packet3<T>::packet3(packet2<T> const& v, T _z) : x(v.x), y(v.y), z(_z) {}
	template<typename T> MATH_FORCE_INLINE packet3<T>::packet3(packet4<T> const& v) : x(v.x), y(v.y), z(v.z) {}
	template<typename T> MATH_FORCE_INLINE packet3<T>::packet3(vec3<T> const& v) : x(v.x), y(v.y), z(v.z) {}
	template<typename T> MATH_FORCE_INLINE packet3<T>& packet3<T>::operator=(vec3<T> const& v) { x = v.x; y = v.y; z = v.z; return *this; }
	template<typename T> MATH_FORCE_INLINE bool packet3<T>::operator==(packet3<T> const& v) const { return x == v.x && y == v.y && z == v.z; }
	template<typename T> MATH_FORCE_INLINE bool packet3<T>::operator!=(packet3<T> const& v) const { return !operator==(v); }

	//////////////////////////////////////////////////////////////////////////
	// packet4
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(T s) : x(s), y(s), z(s), w(s) {}
	template<typename T> template<typename U> MATH_FORCE_INLINE packet4<T>::packet4(packet4<U> const& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(packet2<T> const& v) : x(v.x), y(v.y), z(0), w(0) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(packet2<T> const& v, T _z, T _w) : x(v.x), y(v.y), z(_z), w(_w) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(packet3<T> const& v) : x(v.x), y(v.y), z(v.z), w(0) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(packet3<T> const& v, T _w) : x(v.x), y(v.y), z(v.z), w(_w) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>::packet4(vec4<T> const& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	template<typename T> MATH_FORCE_INLINE packet4<T>& packet4<T>::operator=(vec4<T> const& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	template<typename T> MATH_FORCE_INLINE bool packet4<T>::operator==(packet4<T> const& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	template<typename T> MATH_FORCE_INLINE bool packet4<T>::operator!=(packet4<T> const& v) const { return !operator==(v); }

}