#pragma once

namespace math
{

///////////////////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////////////////
template<typename T> struct vec2;
template<typename T> struct mat3;
template<typename T> struct mat4;


//column-major (OpenGL) layout

//	 .----------- packet0 (column0) - x axis
//   |   .------- packet1 (column1) - y axis
//   0   2
//   1   3


template <typename T>
struct mat2
{
	typedef T value_t;
	static struct ZUninitialized {} uninitialized;
	static struct ZRotation {} rotation;
	static mat2<T> const zero;
	static mat2<T> const one;
	static mat2<T> const identity;
	static uint8_t const rowCount = 2;
	static uint8_t const columnCount = 2;
	static uint8_t const elementCount = 4;

	///////////////////////////////////////////////////////////////////////////////
	// constructors
	///////////////////////////////////////////////////////////////////////////////
	mat2();
	mat2(ZUninitialized);
	mat2(ZRotation, T angle);
	explicit mat2(T value);
	mat2(mat2<T> const& other);
	template<typename U> explicit mat2(mat2<U> const& v);

	//casting
	explicit mat2(mat3<T> const& v);
	explicit mat2(mat4<T> const& v);


	mat2(T const v0, T const v1,
		T const v2, T const v3);

	mat2(vec2<T> const& column0, vec2<T> const& column1);

	///////////////////////////////////////////////////////////////////////////////
	// methods
	///////////////////////////////////////////////////////////////////////////////

	mat2<T>& set(T const values[4]);

	mat2<T>& set_identity();
	bool	invert();
	mat2<T>&transpose();

public:
	//Rows are NOT linearly in memory. First row is m[0], m[2]
	vec2<T> get_row(uint8_t row) const;
	mat2<T>&set_row(uint8_t row, vec2<T> const& v);

	//Columns are linearly in memory. First row is m[0], m[1],  and is the X axis of the matrix
	vec2<T> const& get_column(uint8_t column) const;
	mat2<T>&set_column(uint8_t column, vec2<T> const& v);

	vec2<T> const& get_axis_x() const;
	mat2<T>&set_axis_x(vec2<T> const& axis);

	vec2<T> const& get_axis_y() const;
	mat2<T>&set_axis_y(vec2<T> const& axis);

	vec2<T> get_scale() const;
	mat2<T>&set_scale(vec2<T> const& scale);

	///////////////////////////////////////////////////////////////////////////////
	// operators

	bool operator==(mat2<T> const& v) const;
	bool operator!=(mat2<T> const& v) const;

	//casting
	mat2<T>& operator=(mat4<T> const& m);
	mat2<T>& operator=(mat3<T> const& m);

	///////////////////////////////////////////////////////////////////////////////
	// indexing operators
	///////////////////////////////////////////////////////////////////////////////
	T* data();
	T const* data() const;

	T& operator()(uint8_t column, uint8_t row);
	T const& operator()(uint8_t column, uint8_t row) const;

	///////////////////////////////////////////////////////////////////////////////
	// scalar operators
	///////////////////////////////////////////////////////////////////////////////

	mat2<T> operator*(mat2<T> const& other) const;
	mat2<T> operator+(mat2<T> const& other) const;
	mat2<T> operator-(mat2<T> const& other) const;
	mat2<T> operator*(T scalar) const;
	mat2<T> operator+(T scalar) const;
	mat2<T> operator-(T scalar) const;
	mat2<T>& operator*=(mat2<T> const& other);
	mat2<T>& operator+=(mat2<T> const& other);
	mat2<T>& operator-=(mat2<T> const& other);
	mat2<T>& operator*=(T scalar);
	mat2<T>& operator+=(T scalar);
	mat2<T>& operator-=(T scalar);

	///////////////////////////////////////////////////////////////////////////////
	//members
	///////////////////////////////////////////////////////////////////////////////

	T m[elementCount];
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

template <typename T>
typename mat2<T>::ZUninitialized mat2<T>::uninitialized;
template <typename T>
typename mat2<T>::ZRotation mat2<T>::rotation;

///////////////////////////////////////////////////////////////////////////////

}//namespace math
