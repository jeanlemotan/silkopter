#pragma once

namespace math
{

///////////////////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////////////////
template<typename T> struct vec2;
template<typename T> struct vec3;
template<typename T> struct mat3;

//column-major (OpenGL) layout

//	.--------------- packet0 (column0) - x axis
//	|   .----------- packet1 (column1) - y axis
//  |	|	.------- packet2 (column2) - translation
//  0   3   6
//  1   4   7
//  2   5   8

template <typename T>
struct mat3
{
	typedef T value_t;
	static struct ZUninitialized {} uninitialized;
	static mat3<T> const zero;
	static mat3<T> const one;
	static mat3<T> const identity;
	static uint8_t const rowCount = 3;
	static uint8_t const columnCount = 3;
	static uint8_t const elementCount = 9;


	///////////////////////////////////////////////////////////////////////////////
	// constructors
	///////////////////////////////////////////////////////////////////////////////
	mat3();
	mat3(ZUninitialized);
	explicit mat3(T value);
	mat3(mat3<T> const& other);
	template<typename U> explicit mat3(mat3<U> const& v);

	//casting
	explicit mat3(mat2<T> const& v);
	explicit mat3(mat4<T> const& v);

	mat3(T const v0, T const v1, T const v2, 
				T const v3, T const v4, T const v5, 
				T const v6, T const v7, T const v8);

	mat3(vec3<T> const& column0, vec3<T> const& column1, vec3<T> const& column2);

	///////////////////////////////////////////////////////////////////////////////
	// methods
	///////////////////////////////////////////////////////////////////////////////

	void set(T const values[9]);

	void set_identity();
	bool invert();
	void transpose();

public:
	//Rows are NOT linearly in memory. First row is m[0], m[3], m[6]
	vec3<T> get_row(uint8_t row) const;
	void set_row(uint8_t row, vec3<T> const& v);

	//Columns are linearly in memory. First row is m[0], m[1], m[2] and is the X axis of the matrix
	vec3<T> const& get_column(uint8_t column) const;
	void set_column(uint8_t column, vec3<T> const& v);

	vec3<T> const& get_axis_x() const;
	void set_axis_x(vec3<T> const& axis);

	vec3<T> const& get_axis_y() const;
	void set_axis_y(vec3<T> const& axis);

	vec3<T> const& get_axis_z() const;
	void set_axis_z(vec3<T> const& axis);

	vec3<T> get_scale() const;
	void set_scale(vec3<T> const& scale);

	void post_scale(vec3<T> const& scale);

	///////////////////////////////////////////////////////////////////////////////
	// operators

	bool operator==(mat3<T> const& v) const;
	bool operator!=(mat3<T> const& v) const;

	//casting from mat3
	mat3<T>& operator=(mat2<T> const& m);
	mat3<T>& operator=(mat4<T> const& m);

	///////////////////////////////////////////////////////////////////////////////
	// indexing operators
	///////////////////////////////////////////////////////////////////////////////
	T* data();
	T const* data() const;

	T& operator()(uint8_t column, uint8_t row);
	T const& operator()(uint8_t column, uint8_t row) const;

	///////////////////////////////////////////////////////////////////////////////
	// arithmetic operators
	///////////////////////////////////////////////////////////////////////////////

	mat3<T> operator*(mat3<T> const& other) const;
	mat3<T> operator+(mat3<T> const& other) const;
	mat3<T> operator-(mat3<T> const& other) const;
	mat3<T> operator*(T scalar) const;
	mat3<T> operator+(T scalar) const;
	mat3<T> operator-(T scalar) const;
	mat3<T>& operator*=(mat3<T> const& other);
	mat3<T>& operator+=(mat3<T> const& other);
	mat3<T>& operator-=(mat3<T> const& other);
	mat3<T>& operator*=(T scalar);
	mat3<T>& operator+=(T scalar);
	mat3<T>& operator-=(T scalar);

	///////////////////////////////////////////////////////////////////////////////
	//members
	///////////////////////////////////////////////////////////////////////////////
	T m[elementCount];
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

template <typename T>
typename mat3<T>::ZUninitialized mat3<T>::uninitialized;

///////////////////////////////////////////////////////////////////////////////

}//namespace math
