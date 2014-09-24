namespace math
{


///! Creates a RH orthographic projection matrix
template<typename T>
inline mat4<T> ortho(T left, T right, T bottom, T top, T z_near, T z_far)
{
	mat4<T> mat;
	T* M = mat.m;
	M[0] = 2 / (right - left);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = 2 / (top - bottom);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(-2) / (z_far - z_near);
	M[11] = 0;

	M[12] = - (right + left) / (right - left);
	M[13] = - (top + bottom) / (top - bottom);
	M[14] = - (z_far + z_near) / (z_far - z_near);
	M[15] = 1;
	return mat;
}

///! Creates a RH perspective projection matrix
template<typename T>
inline mat4<T> perspective(T fov, T aspect_ratio, T z_near, T z_far)
{
	T const y_fac = tan(fov / 2);
	T const x_fac = y_fac*aspect_ratio;
	mat4<T> mat;
	T* M = mat.m;

	M[0] = 1/x_fac;
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = 1/y_fac;
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = -(z_far+z_near)/(z_far-z_near);
	M[11] = -1;

	M[12] = 0;
	M[13] = 0;
	M[14] = -(2*z_far*z_near)/(z_far-z_near);
	M[15] = 0;
	return mat;
}

///! Creates a RH asymmetric frustum projection matrix
template<typename T>
inline mat4<T> perspective(T left, T right, T bottom, T top, T z_near, T z_far)
{
	// glFrustum call (column order)
	//
	// |    2n/(r-l)       0         (r+l)/(r-l)     0       |
	// |      0          2n/(t-b)    (t+b)/(t-b)     0       |
	// |      0            0         (n+f)/(n-f) (2nf)/(n-f) |
	// |      0            0              -1         0       |
	//
	T diff_x = 1/(right-left);
	T diff_y = 1/(top-bottom);
	T diff_z = 1/(z_near-z_far);

	mat4<T> mat;
	T* m = mat.m;
	m[ 0] = 2*z_near*diff_x;
	m[ 1] = 0;
	m[ 2] = 0;
	m[ 3] = 0;

	m[ 4] = 0;
	m[ 5] = 2*z_near*diff_y;
	m[ 6] = 0;
	m[ 7] = 0;

	m[ 8] = (right+left)*diff_x;
	m[ 9] = (top+bottom)*diff_y;
	m[10] = (z_far+z_near)*diff_z;
	m[11] = -1;

	m[12] = 0;
	m[13] = 0;
	m[14] = 2*z_near*z_far*diff_z;
	m[15] = 0;

	return mat;
}
 

}
