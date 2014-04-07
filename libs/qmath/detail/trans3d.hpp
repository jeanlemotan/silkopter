namespace math
{

///////////////////////////////////////////////////////////////////////////////
//constructors
///////////////////////////////////////////////////////////////////////////////

template<typename T> inline trans3d<T>::trans3d()
{
	set_identity();
	assert(is_valid());
}
template<typename T> inline trans3d<T>::trans3d(ZUninitialized)
	: mat(mat4<T>::uninitialized)
{
}
template<typename T> inline trans3d<T>::trans3d(trans3d<T> const& other)
	: mat(other.mat)
{
	assert(is_valid());
}
template<typename T> template<typename U> inline trans3d<T>::trans3d(trans3d<U> const& other)
	: mat(other.mat)
{
	assert(is_valid());
}
template<typename T> inline trans3d<T>::trans3d(vec3<T> const& translation, quat<T> const& rotation, vec3<T> const& scale)
{
	set_translation(translation);
	set_rotation(rotation.get_as_mat3());
	post_scale(scale);
}
template<typename T> inline trans3d<T>::trans3d(vec3<T> const& translation, mat3<T> const& rotation, vec3<T> const& scale)
{
	set_translation(translation);
	set_rotation(rotation);
	post_scale(scale);
}


///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T> inline trans3d<T>& trans3d<T>::set_identity()
{
	mat.set_identity();
	assert(is_valid());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::set_rotation_identity()
{
	mat.m[ 0] = 1;
	mat.m[ 1] = 0;
	mat.m[ 2] = 0;

	mat.m[ 4] = 0;
	mat.m[ 5] = 1;
	mat.m[ 6] = 0;

	mat.m[ 8] = 0;
	mat.m[ 9] = 0;
	mat.m[10] = 1;

	assert(is_valid());
	return *this;
}
template<typename T> inline bool trans3d<T>::invert()
{
	mat.invert();
	repair();
	return true;
}
template<typename T> inline vec3<T> const& trans3d<T>::get_axis_x() const
{
	return (vec3<T> const&)(mat.get_axis_x());
}
template<typename T> inline vec3<T> const& trans3d<T>::get_axis_y() const
{
	return (vec3<T> const&)(mat.get_axis_y());
}
template<typename T> inline vec3<T> const& trans3d<T>::get_axis_z() const
{
	return (vec3<T> const&)(mat.get_axis_z());
}
template<typename T> inline vec3<T> const& trans3d<T>::get_translation() const
{
	return (vec3<T> const&)(mat.get_axis_w());
}
template<typename T> inline vec3<T> trans3d<T>::get_scale() const
{
	return vec3<T>(length(get_axis_x()), length(get_axis_y()), length(get_axis_z()));
}
template<typename T> inline trans3d<T>& trans3d<T>::set_axis_x(vec3<T> const& axis)
{
	mat.set_axis_x(vec4<T>(axis, 0));
	assert(is_valid());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::set_axis_y(vec3<T> const& axis)
{
	mat.set_axis_y(vec4<T>(axis, 0));
	assert(is_valid());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::set_axis_z(vec3<T> const& axis)
{
	mat.set_axis_z(vec4<T>(axis, 0));
	assert(is_valid());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::set_translation(vec3<T> const& p)
{
	mat.set_axis_w(vec4<T>(p, T(1)));
	assert(is_valid());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::set_scale(vec3<T> const& s)
{
	mat.m[ 0] = s.x;
	mat.m[ 5] = s.y;
	mat.m[10] = s.z;
	assert(is_valid());
	return *this;
}
template<typename T> inline mat3<T> trans3d<T>::get_rotation() const
{
	return mat3<T>(mat);
}
template<typename T> inline trans3d<T>& trans3d<T>::set_rotation(mat3<T> const& rotation)
{
	mat.column0 = vec4<T>(rotation.m[0], rotation.m[1], rotation.m[2], 0);
	mat.column1 = vec4<T>(rotation.m[3], rotation.m[4], rotation.m[5], 0);
	mat.column2 = vec4<T>(rotation.m[6], rotation.m[7], rotation.m[8], 0);
	assert(is_valid());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::set_rotation(quat<T> const& rotation)
{
	set_rotation(rotation.get_as_mat3());
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::post_scale(vec3<T> const& scale)
{
	vec4<T> s4(scale, T(0));

	mat.column0 = (vec4<T>(mat.column0) * s4);
	mat.column1 = (vec4<T>(mat.column1) * s4);
	mat.column2 = (vec4<T>(mat.column2) * s4);

// 	mat.m[ 0] *= scale.x;
// 	mat.m[ 1] *= scale.x;
// 	mat.m[ 2] *= scale.x;
// 
// 	mat.m[ 4] *= scale.y;
// 	mat.m[ 5] *= scale.y;
// 	mat.m[ 6] *= scale.y;
// 
// 	mat.m[ 8] *= scale.z;
// 	mat.m[ 9] *= scale.z;
// 	mat.m[10] *= scale.z;
	return *this;
}
template<typename T> inline trans3d<T>& trans3d<T>::post_translate(vec3<T> const& translation)
{
	vec4<T> t4(translation, T(0));
	mat.column3 = vec4<T>(vec3<T>(mat.column0)*t4 + vec3<T>(mat.column1)*t4 + vec3<T>(mat.column2)*t4, T(1));
	assert(is_valid());

// 	mat.m[12] += mat.m[0]*translation.x + mat.m[4]*translation.y + mat.m[ 8]*translation.z;
// 	mat.m[13] += mat.m[1]*translation.x + mat.m[5]*translation.y + mat.m[ 9]*translation.z;
// 	mat.m[14] += mat.m[2]*translation.x + mat.m[6]*translation.y + mat.m[10]*translation.z;

	return *this;
}

template<typename T> inline void trans3d<T>::repair()
{
	mat.m[3] = mat.m[7] = mat.m[11] = T(0);
	mat.m[15] = T(1);
}
template<typename T> inline bool trans3d<T>::is_valid() const
{
	return mat.m[3] == 0 && mat.m[7] == 0 && mat.m[11] == 0 && mat.m[15] == T(1);
}

//////////////////////////////////////////////////////////////////////////

template<typename T> inline bool trans3d<T>::operator==(trans3d<T> const& m) const
{
	return mat == m.mat;
}
template<typename T> inline bool trans3d<T>::operator!=(trans3d<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template<typename T> inline trans3d<T> trans3d<T>::operator*(trans3d<T> const& other) const
{
	trans3d<T> ret;
	multiply(ret, *this, other);
	assert(ret.is_valid());
	return ret;
}
template<typename T> inline trans3d<T>& trans3d<T>::operator*=(trans3d<T> const& other)
{
	trans3d<T> a(*this);
	multiply(*this, a, other);
	assert(is_valid());
	return *this;
}

}