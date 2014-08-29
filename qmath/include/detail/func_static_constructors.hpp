namespace math
{


///! Creates a world space lookat matrix (front axis is -yaxis, up axis is zaxis)
/// This lookAt method computes a look at matrix in jet coordinate system (3dmax biped). 
/// This means that when you send a front of (0,1,0) and an up of (0,0,1) the resulting matrix is identity.
template<class T, class Policy>
inline mat3<T> look_at(vec3<T> const& front, vec3<T> const& up)
{
	vec3<T> axisY = normalized<T, Policy>(front);
	vec3<T> axisX = cross(axisY, normalized<T, Policy>(up));
	axisX.template normalize<Policy>(); //this normalize is mandatory because axisY and up may be unitary but they hardly are orthogonal
	vec3<T> axisZ = cross(axisX, axisY);

	mat3<T> mat;
	T* m = mat.m;
	m[0] = axisX.x;
	m[1] = axisX.y;
	m[2] = axisX.z;

	m[3] = axisY.x;
	m[4] = axisY.y;
	m[5] = axisY.z;

	m[6] = axisZ.x;
	m[7] = axisZ.y;
	m[8] = axisZ.z;

	return mat;
}

template<class T, class Policy>
inline quat<T> quat_look_at(vec3<T> const& front, vec3<T> const& up)
{
	mat3<T> mat = look_at<T, Policy>(front, up);
	quat<T> q;
	q.set_from_mat3(mat);
	return q;
}

template<typename T, class Policy>
inline quat<T> quat_axis_x(T angle)
{
	quat<T> q(quat<T>::uninitialized);
	q.template set_from_euler_xyz<Policy>(math::vec3<T>(angle, 0, 0));
	return q;
}

template<typename T, class Policy>
inline quat<T> quat_axis_y(T angle)
{
	quat<T> q(quat<T>::uninitialized);
	q.template set_from_euler_xyz<Policy>(math::vec3<T>(0, angle, 0));
	return q;
}

template<typename T, class Policy>
inline quat<T> quat_axis_z(T angle)
{
	quat<T> q(quat<T>::uninitialized);
	q.template set_from_euler_xyz<Policy>(math::vec3<T>(0, 0, angle));
	return q;
}

template <typename T, class Policy>
quat<T> quat_rotation_from_to(vec3<T> const& a, vec3<T> const& b)
{
	vec3<T> axis = cross(a, b);
	T dot = math::dot(a, b);
	if (dot < (T)(-1.0) + std::numeric_limits<T>::epsilon())// vectors are parallel and facing in the opposite direction
	{
		// Try crossing with x axis.
		vec3<T> t = cross(a,vec3<T>(1, 0, 0));
		// If not ok, cross with y axis.
		if(is_zero(math::length<T, Policy>(t)))
		{
			t = cross(a,vec3<T>(0, 1, 0));
		}

		t.template normalize<Policy>();
		return quat<T>(t[0], t[1], t[2], 0);
	}

	// if vectors are parallel and are facing in the same direction
	//	the axis is zero and quaternion is the identity
	quat<T> result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (dot + 1.0f) * 0.5f);
	result.template normalize<Policy>();
	return result;
}



}