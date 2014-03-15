#pragma once

namespace math
{
	template<typename T> struct vec2;
	template<typename T> struct vec3;
	template<typename T> struct vec4;
	template<typename T> struct packet2;
	template<typename T> struct packet3;
	template<typename T> struct packet4;

	//GUARANTEED sizeof(packet2<T>) == sizeof(T) * 2
	template<typename T> struct packet2
	{
		T x, y;

		packet2(T x, T y);
		explicit packet2(T s);

		template<typename U> explicit packet2(packet2<U> const& v);
		explicit packet2(packet3<T> const& v);
		explicit packet2(packet4<T> const& v);
		explicit packet2(vec2<T> const& v);
		packet2<T>& operator=(vec2<T> const& v);
		bool operator==(packet2<T> const& v) const;
		bool operator!=(packet2<T> const& v) const;
	};

	//GUARANTEED sizeof(packet3<T>) == sizeof(T) * 3
	template<typename T> struct packet3
	{
		T x, y, z;

		packet3(T x, T y, T z);
		explicit packet3(T s);

		template<typename U> explicit packet3(packet3<U> const& v);
		explicit packet3(packet2<T> const& v);
		explicit packet3(packet2<T> const& v, T z);
		explicit packet3(packet4<T> const& v);
		explicit packet3(vec3<T> const& v);
		packet3<T>& operator=(vec3<T> const& v);
		bool operator==(packet3<T> const& v) const;
		bool operator!=(packet3<T> const& v) const;
	};

	//GUARANTEED sizeof(packet4<T>) == sizeof(T) * 4
	template<typename T> struct packet4
	{
		T x, y, z, w;

		packet4(T x, T y, T z, T w);
		explicit packet4(T s);

		template<typename U> explicit packet4(packet4<U> const& v);
		explicit packet4(packet2<T> const& v);
		explicit packet4(packet2<T> const& v, T z, T w);
		explicit packet4(packet3<T> const& v);
		explicit packet4(packet3<T> const& v, T w);
		explicit packet4(vec4<T> const& v);
		packet4<T>& operator=(vec4<T> const& v);
		bool operator==(packet4<T> const& v) const;
		bool operator!=(packet4<T> const& v) const;
	};
}