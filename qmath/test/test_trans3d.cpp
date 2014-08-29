#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

template<typename T>
void testTrans3d()
{
	trans3d<T> m0(
		vec3<T>(0.6f, 0.2f, 0.3f), 
		quat<T>(0.2f, 0.7f, 0.5f, 0.3f), 
		vec3<T>(0.3f, 0.5f, 0.7f));

	{
		BOOST_CHECK(is_identity(trans3d<T>::identity, epsilon<T>()));
		trans3d<T> inv0 = inverse(trans3d<T>::identity);
		BOOST_CHECK(is_identity(inv0, epsilon<T>()));
	}

	{
		trans3d<T> inv0 = inverse(m0);
		trans3d<T> res0 = m0 * inv0;
		BOOST_CHECK(is_identity(res0, 0.0001f));
	}
	{
		trans3d<T> m0;
		BOOST_CHECK(is_identity(m0));
	}
	{
		mat4<T> m0(mat4<T>::one);
		//this should leave m0 unchanged
		auto* t = new (&m0) trans3d<T>(trans3d<T>::uninitialized);

		BOOST_CHECK(m0 == mat4<T>::one);
	}
	{
		quat<T> rot;
		rot.set_from_euler_xyz(vec3<T>(1, 2, 3));
		mat3<T> rotm = rot.get_as_mat3();
		trans3d<T> t0(vec3<T>(23, 24, 25), rotm, vec3<T>(1, 1, 1));
		trans3d<T> t1(vec3<T>(23, 24, 25), rot, vec3<T>(1, 1, 1));

		BOOST_CHECK(t0 == t1);
		BOOST_CHECK(t0 != m0);
		BOOST_CHECK(t1 != m0);

		BOOST_CHECK(t0.get_translation() == vec3<T>(23, 24, 25));
		BOOST_CHECK(equals(t0.get_scale(), vec3<T>(1, 1, 1), T(0.001)));
		t0.post_scale(T(1) / t0.get_scale());
		BOOST_CHECK(equals(t0.get_rotation(), rotm, T(0.001)));
	}

	{
		quat<T> rot;
		rot.set_from_euler_xyz(vec3<T>(1, 2, 3));
		trans3d<T> t0(vec3<T>(23, 24, 25), rot, vec3<T>(1, 1, 1));
		trans3d<T> t1(vec3<T>(25, 26, 27), rot, vec3<T>(1, 1, 1));

		auto res0 = t0 * t1;
		trans3d<T> res1;
		res1.mat = t0.mat * t1.mat;
		res1.repair();

		BOOST_CHECK(res0 == res1);
		t0 *= t1;
		BOOST_CHECK(t0 == res1);
	}
	{
		quat<T> rot;
		rot.set_from_euler_xyz(vec3<T>(1, 2, 3));
		trans3d<T> t0(vec3<T>(23, 24, 25), rot, vec3<T>(1, 1, 1));
		t0.set_identity();
		BOOST_CHECK(t0 == trans3d<T>::identity);
	}
	{
		quat<T> rot;
		rot.set_from_euler_xyz(vec3<T>(1, 2, 3));
		trans3d<T> t0(vec3<T>(23, 24, 25), rot, vec3<T>(1, 1, 1));
		t0.set_rotation_identity();
		BOOST_CHECK(t0.get_rotation() == mat3<T>::identity);
		BOOST_CHECK(t0.get_translation() == vec3<T>(23, 24, 25));
	}
	{
		mat3<T> m0(0.72f);
		trans3d<T> m1(vec3<T>(1, 2, 3), m0, vec3<T>(1, 1, 1));
		BOOST_CHECK(m1.get_axis_x() == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_axis_y() == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_axis_z() == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_translation() == vec3<T>(1.f, 2.f, 3.f));
	}

	int a = 0;
}


BOOST_AUTO_TEST_CASE(Trans3d)
{
	testTrans3d<float>();
	//testTrans3d<double>();
}
