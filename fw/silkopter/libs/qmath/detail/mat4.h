#pragma once

namespace math
{

///////////////////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////////////////
template<typename T> struct vec3;
template<typename T> struct vec4;
template<typename T> struct mat3;

//column-major (OpenGL) layout

//	.--------------- column0 - x axis
//	|   .----------- column1 - y axis
//	|   |   .------- column2 - z axis
//  |	|	|	.--- column3 - w axis
//  0   4   8  12
//  1   5   9  13
//  2   6  10  14
//  3   7  11  15

template <typename T>
struct mat4
{
	typedef T value_t;

	static struct ZUninitialized {} uninitialized;
	static mat4<T> const zero;
	static mat4<T> const one;
	static mat4<T> const identity;
	static uint8_t const rowCount = 4;
	static uint8_t const columnCount = 4;
	static uint8_t const elementCount = 16;


	///////////////////////////////////////////////////////////////////////////////
	// constructors
	///////////////////////////////////////////////////////////////////////////////
	mat4();
	mat4(ZUninitialized);
	explicit mat4(T value);
	mat4(mat4<T> const& other);
	template<typename U> explicit mat4(mat4<U> const& v);

	//casting from mat3
	explicit mat4(mat3<T> const& v);
	explicit mat4(mat2<T> const& v);

	//setting from a T array
	mat4(T const v0, T const v1, T const v2, T const v3,
		T const v4, T const v5, T const v6, T const v7,
		T const v8, T const v9, T const v10, T const v11,
		T const v12, T const v13, T const v14, T const v15);
	mat4(vec4<T> const& column0, vec4<T> const& column1, vec4<T> const& column2, vec4<T> const& column3);

	///////////////////////////////////////////////////////////////////////////////
	// arithmetic operators
	///////////////////////////////////////////////////////////////////////////////

	mat4<T> operator*(mat4<T> const& other) const;
	mat4<T> operator+(mat4<T> const& other) const;
	mat4<T> operator-(mat4<T> const& other) const;
	mat4<T> operator*(T scalar) const;
	mat4<T> operator+(T scalar) const;
	mat4<T> operator-(T scalar) const;
	mat4<T>& operator*=(mat4<T> const& other);
	mat4<T>& operator+=(mat4<T> const& other);
	mat4<T>& operator-=(mat4<T> const& other);
	mat4<T>& operator*=(T scalar);
	mat4<T>& operator+=(T scalar);
	mat4<T>& operator-=(T scalar);


	///////////////////////////////////////////////////////////////////////////////
	// methods
	///////////////////////////////////////////////////////////////////////////////

	void set(T const values[16]);

	void	set_identity();
	template<class Polict = standard> bool	invert();
	void transpose();

public:
	//Rows are NOT linearly in memory. First row is m[0], m[4], m[8], m[12]
	vec4<T> get_row(uint8_t row) const;
	void	set_row(uint8_t row, vec4<T> const& v);

	//Columns are linearly in memory. First row is m[0], m[1], m[2], m[3]  and is the X axis of the matrix
	vec4<T> const& get_column(uint8_t column) const;
	void	set_column(uint8_t column, vec4<T> const& v);

	vec4<T> const& get_axis_x() const;
	void	set_axis_x(vec4<T> const& axis);

	vec4<T> const& get_axis_y() const;
	void	set_axis_y(vec4<T> const& axis);

	vec4<T> const& get_axis_z() const;
	void	set_axis_z(vec4<T> const& axis);

	vec4<T> const& get_axis_w() const;
	void	set_axis_w(vec4<T> const& axis);

	///////////////////////////////////////////////////////////////////////////////
	// operators

	bool operator==(mat4<T> const& v) const;
	bool operator!=(mat4<T> const& v) const;

	//casting from mat3
	mat4<T>& operator=(mat3<T> const& m);
	mat4<T>& operator=(mat2<T> const& m);

	///////////////////////////////////////////////////////////////////////////////
	// indexing operators
	///////////////////////////////////////////////////////////////////////////////
	T* data();
	T const* data() const;

	T& operator()(uint8_t column, uint8_t row);
	T const& operator()(uint8_t column, uint8_t row) const;

	///////////////////////////////////////////////////////////////////////////////
	//members
	///////////////////////////////////////////////////////////////////////////////
	union
	{
		T m[elementCount];
		struct 
		{
			vec4<T> column0;
			vec4<T> column1;
			vec4<T> column2;
			vec4<T> column3;
		};
	};
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

template <typename T>
typename mat4<T>::ZUninitialized mat4<T>::uninitialized;

///////////////////////////////////////////////////////////////////////////////

}//namespace math
