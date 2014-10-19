namespace math
{
	template<typename T> trans2d<T> const trans2d<T>::identity;																   \

///////////////////////////////////////////////////////////////////////////////
//constructors
///////////////////////////////////////////////////////////////////////////////

template<typename T> inline trans2d<T>::trans2d()
{
	set_identity();
    QASSERT(is_valid());
}
template<typename T> inline trans2d<T>::trans2d(ZUninitialized)
	: mat(mat3<T>::uninitialized)
{
}
template<typename T> inline trans2d<T>::trans2d(trans2d<T> const& other)
	: mat(other.mat)
{
    QASSERT(is_valid());
}
template<typename T> template<typename U> inline trans2d<T>::trans2d(trans2d<U> const& other)
	: mat(other.mat)
{
    QASSERT(is_valid());
}

template<typename T> inline trans2d<T>::trans2d(vec2<T> const& translation, angle<T> const& rotation, vec2<T> const& scale)
{
	set_translation(translation);
    mat2<T> rot(rotation);
	set_rotation(rot);
	post_scale(scale);
}
template<typename T> inline trans2d<T>::trans2d(vec2<T> const& translation, mat2<T> const& rotation, vec2<T> const& scale)
{
	set_translation(translation);
	set_rotation(rotation);
	post_scale(scale);
}

///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T> inline trans2d<T>& trans2d<T>::set_identity()
{
	mat.set_identity();
    QASSERT(is_valid());
	return *this;
}
template<typename T> inline trans2d<T>& trans2d<T>::set_rotation_identity()
{
	mat.m[0] = 1;
	mat.m[1] = 0;

	mat.m[3] = 0;
	mat.m[4] = 1;

    QASSERT(is_valid());
	return *this;
}
template<typename T> 
template<class Policy>
inline bool trans2d<T>::invert()
{
	mat.template invert<Policy>();
	repair();
	return true;
}
template<typename T> inline vec2<T> const& trans2d<T>::get_axis_x() const
{
	return (vec2<T> const&)(mat.m[0]);
}
template<typename T> inline vec2<T> const& trans2d<T>::get_axis_y() const
{
	return (vec2<T> const&)(mat.m[3]);
}
template<typename T> inline vec2<T> const& trans2d<T>::get_translation() const
{
	return (vec2<T> const&)(mat.m[6]);
}
template<typename T> inline vec2<T> trans2d<T>::get_scale() const
{
	return vec2<T>(length(get_axis_x()), length(get_axis_y()));
}
template<typename T> inline trans2d<T>& trans2d<T>::set_axis_x(vec2<T> const& axis)
{
	mat.set_axis_x(vec3<T>(axis, 0));
    QASSERT(is_valid());
	return *this;
}
template<typename T> inline trans2d<T>& trans2d<T>::set_axis_y(vec2<T> const& axis)
{
	mat.set_axis_y(vec3<T>(axis, 0));
    QASSERT(is_valid());
	return *this;
}
template<typename T> inline trans2d<T>& trans2d<T>::set_translation(vec2<T> const& p)
{
	mat.set_axis_z(vec3<T>(p, T(1)));
    QASSERT(is_valid());
	return *this;
}
template<typename T> inline trans2d<T>& trans2d<T>::set_scale(vec2<T> const& s)
{
	mat.m[0] = s.x;
	mat.m[4] = s.y;
	mat.m[8] = s.z;
    QASSERT(is_valid());
	return *this;
}
template<typename T> inline mat2<T> trans2d<T>::get_rotation() const
{
	return mat2<T>(mat);
}
template<typename T> inline trans2d<T>& trans2d<T>::set_rotation(mat2<T> const& rot)
{
	mat.m[0] = rot.m[0];
	mat.m[1] = rot.m[1];
	mat.m[3] = rot.m[2];
	mat.m[4] = rot.m[3];
    QASSERT(is_valid());
	return *this;
}
template<typename T> inline trans2d<T>& trans2d<T>::set_rotation(angle<T> const& rot)
{
    return set_rotation(mat2<T>(rot));
}
template<typename T> inline trans2d<T>& trans2d<T>::post_scale(const vec2<T>& scale)
{
	mat.m[ 0] *= scale.x;
	mat.m[ 1] *= scale.x;

	mat.m[ 3] *= scale.y;
	mat.m[ 4] *= scale.y;

	return *this;
}
template<typename T> inline trans2d<T>& trans2d<T>::post_translate(vec2<T> const& trans)
{
	mat.m[6] += mat.m[0]*trans.x + mat.m[3]*trans.y;
	mat.m[7] += mat.m[1]*trans.x + mat.m[4]*trans.y;
	return *this;
}

template<typename T> inline void trans2d<T>::repair()
{
	mat.m[2] = mat.m[5] = T(0);
	mat.m[8] = T(1);
}
template<typename T> inline bool trans2d<T>::is_valid() const
{
	return mat.m[2] == 0 && mat.m[5] == 0 && mat.m[8] == T(1);
}

//////////////////////////////////////////////////////////////////////////

template<typename T> inline bool trans2d<T>::operator==(trans2d<T> const& m) const
{
	return mat == m.mat;
}
template<typename T> inline bool trans2d<T>::operator!=(trans2d<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template<typename T> inline trans2d<T> trans2d<T>::operator*(trans2d<T> const& other) const
{
	trans2d<T> ret;
	multiply(ret, *this, other);
    QASSERT(ret.is_valid());
	return ret;
}
template<typename T> inline trans2d<T>& trans2d<T>::operator*=(const trans2d<T>& other)
{
	trans2d<T> a(*this);
	multiply(*this, a, other);
    QASSERT(is_valid());
	return *this;
}

}
