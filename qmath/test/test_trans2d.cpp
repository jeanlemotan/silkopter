#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

template<typename T>
void testTrans2d()
{
	trans2d<T> m0(
		vec2<T>(0.6f, 0.2f), 
		0.7f, 
		vec2<T>(0.3f, 0.5f));

	{
		BOOST_CHECK(is_identity(trans2d<T>::identity, epsilon<T>()));
		trans2d<T> inv0 = inverse(trans2d<T>::identity);
		BOOST_CHECK(is_identity(inv0, epsilon<T>()));
	}

	{
		trans2d<T> inv0 = inverse(m0);
		trans2d<T> res0 = m0 * inv0;
		BOOST_CHECK(is_identity(res0, 0.0001f));
	}
	{
		trans2d<T> m0;
		BOOST_CHECK(is_identity(m0));
	}
	{
		mat4<T> m0(mat4<T>::one);
		//this should leave m0 unchanged
		auto* t = new (&m0) trans2d<T>(trans2d<T>::uninitialized);

		BOOST_CHECK(m0 == mat4<T>::one);
	}
	{
		T rot = T(2.7);
		mat2<T> rotm(mat2<T>::rotation, rot);
		trans2d<T> t0(vec2<T>(23, 24), rotm, vec2<T>(1, 1));
		trans2d<T> t1(vec2<T>(23, 24), rot, vec2<T>(1, 1));

		BOOST_CHECK(t0 == t1);
		BOOST_CHECK(t0 != m0);
		BOOST_CHECK(t1 != m0);

		BOOST_CHECK(t0.get_translation() == vec2<T>(23, 24));
		BOOST_CHECK(equals(t0.get_scale(), vec2<T>(1, 1), T(0.001)));
		t0.post_scale(T(1) / t0.get_scale());
		BOOST_CHECK(equals(t0.get_rotation(), rotm, T(0.001)));
	}

	{
		T rot = T(2.7);
		mat2<T> rotm(mat2<T>::rotation, rot);
		trans2d<T> t0(vec2<T>(23, 24), rotm, vec2<T>(1, 1));
		trans2d<T> t1(vec2<T>(23, 24), rot, vec2<T>(1, 1));

		auto res0 = t0 * t1;
		trans2d<T> res1;
		res1.mat = t0.mat * t1.mat;
		res1.repair();

		BOOST_CHECK(res0 == res1);
		t0 *= t1;
		BOOST_CHECK(t0 == res1);
	}
	{
		T rot = T(2.7);
		mat2<T> rotm(mat2<T>::rotation, rot);
		trans2d<T> t0(vec2<T>(23, 24), rotm, vec2<T>(1, 1));
		t0.set_identity();
		BOOST_CHECK(t0 == trans2d<T>::identity);
	}
	{
		T rot = T(2.7);
		mat2<T> rotm(mat2<T>::rotation, rot);
		trans2d<T> t0(vec2<T>(23, 24), rotm, vec2<T>(1, 1));
		t0.set_rotation_identity();
		BOOST_CHECK(t0.get_rotation() == mat2<T>::identity);
		BOOST_CHECK(t0.get_translation() == vec2<T>(23, 24));
	}
	{
		mat2<T> m0(0.72f);
		trans2d<T> m1(vec2<T>(1, 2), m0, vec2<T>(1, 1));
		BOOST_CHECK(m1.get_axis_x() == vec2<T>(0.72f, 0.72f));
		BOOST_CHECK(m1.get_axis_y() == vec2<T>(0.72f, 0.72f));
		BOOST_CHECK(m1.get_translation() == vec2<T>(1, 2));
	}

	int a = 0;
}


BOOST_AUTO_TEST_CASE(Trans2d)
{
	testTrans2d<float>();
//	testTrans2d<double>();
}
