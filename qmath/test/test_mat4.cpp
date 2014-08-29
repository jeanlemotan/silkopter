#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

template<typename T>
void testMat4()
{
	mat4<T> m0(
		vec4<T>(0.6f, 0.2f, 0.3f, 0.4f), 
		vec4<T>(0.2f, 0.7f, 0.5f, 0.3f), 
		vec4<T>(0.3f, 0.5f, 0.7f, 0.2f), 
		vec4<T>(0.4f, 0.3f, 0.2f, 0.6f));

	{
		BOOST_CHECK(is_identity(mat4<T>::identity, epsilon<T>()));
		BOOST_CHECK(is_one(mat4<T>::one, epsilon<T>()));
		BOOST_CHECK(is_zero(mat4<T>::zero, epsilon<T>()));
		mat4<T> inv0 = inverse(mat4<T>::identity);
		BOOST_CHECK(is_identity(inv0, epsilon<T>()));
		mat4<T> trans0 = transposed(mat4<T>::identity);
		BOOST_CHECK(is_identity(trans0, epsilon<T>()));
	}

	{
		mat4<T> inv0 = inverse(m0);
		mat4<T> res0 = m0 * inv0;
		BOOST_CHECK(is_identity(res0, T(0.0001)));
	}

	{
		mat4<T> m0(0.72f);
		BOOST_CHECK(m0.get_column(0) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_column(1) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_column(2) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_column(3) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_row(0) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_row(1) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_row(2) == vec4<T>(0.72f));
		BOOST_CHECK(m0.get_row(3) == vec4<T>(0.72f));
	}
	{
		mat4<T> m0(0.72f);
		mat4<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_column(1) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_column(2) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_column(3) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_row(0) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_row(1) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_row(2) == vec4<T>(0.72f));
		BOOST_CHECK(m1.get_row(3) == vec4<T>(0.72f));
	}
	{
		mat4<int> m0(12);
		mat4<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec4<T>(12));
		BOOST_CHECK(m1.get_column(1) == vec4<T>(12));
		BOOST_CHECK(m1.get_column(2) == vec4<T>(12));
		BOOST_CHECK(m1.get_column(3) == vec4<T>(12));
		BOOST_CHECK(m1.get_row(0) == vec4<T>(12));
		BOOST_CHECK(m1.get_row(1) == vec4<T>(12));
		BOOST_CHECK(m1.get_row(2) == vec4<T>(12));
		BOOST_CHECK(m1.get_row(3) == vec4<T>(12));
	}
	{
		mat3<T> m0(0.72f);
		mat4<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec4<T>(0.72f, 0.72f, 0.72f, 0.f));
		BOOST_CHECK(m1.get_column(1) == vec4<T>(0.72f, 0.72f, 0.72f, 0.f));
		BOOST_CHECK(m1.get_column(2) == vec4<T>(0.72f, 0.72f, 0.72f, 0.f));
		BOOST_CHECK(m1.get_column(3) == vec4<T>(0.f, 0.f, 0.f, 1.f));
		BOOST_CHECK(m1.get_row(0) == vec4<T>(0.72f, 0.72f, 0.72f, 0));
		BOOST_CHECK(m1.get_row(1) == vec4<T>(0.72f, 0.72f, 0.72f, 0));
		BOOST_CHECK(m1.get_row(2) == vec4<T>(0.72f, 0.72f, 0.72f, 0));
		BOOST_CHECK(m1.get_row(3) == vec4<T>(0, 0, 0, 1));
	}
	{
		mat2<T> m0(0.72f);
		mat4<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec4<T>(0.72f, 0.72f, 0.f, 0.f));
		BOOST_CHECK(m1.get_column(1) == vec4<T>(0.72f, 0.72f, 0.f, 0.f));
		BOOST_CHECK(m1.get_column(2) == vec4<T>(0.f, 0.f, 1.f, 0.f));
		BOOST_CHECK(m1.get_column(3) == vec4<T>(0.f, 0.f, 0.f, 1.f));
		BOOST_CHECK(m1.get_row(0) == vec4<T>(0.72f, 0.72f, 0, 0));
		BOOST_CHECK(m1.get_row(1) == vec4<T>(0.72f, 0.72f, 0, 0));
		BOOST_CHECK(m1.get_row(2) == vec4<T>(0, 0, 1, 0));
		BOOST_CHECK(m1.get_row(3) == vec4<T>(0, 0, 0, 1));
	}

	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		BOOST_CHECK(m0.get_column(0) == vec4<T>(0, 1, 2, 3));
		BOOST_CHECK(m0.get_column(1) == vec4<T>(4, 5, 6, 7));
		BOOST_CHECK(m0.get_column(2) == vec4<T>(8, 9, 10, 11));
		BOOST_CHECK(m0.get_column(3) == vec4<T>(12, 13, 14, 15));
	}
	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		BOOST_CHECK(m0.get_row(0) == vec4<T>(0, 4, 8, 12));
		BOOST_CHECK(m0.get_row(1) == vec4<T>(1, 5, 9, 13));
		BOOST_CHECK(m0.get_row(2) == vec4<T>(2, 6, 10, 14));
		BOOST_CHECK(m0.get_row(3) == vec4<T>(3, 7, 11, 15));
	}
	{
		mat4<T> m0(
			vec4<T>(0, 1, 2, 3),
			vec4<T>(4, 5, 6, 7),
			vec4<T>(8, 9, 10, 11),
			vec4<T>(12, 13, 14, 15));
		BOOST_CHECK(m0.get_column(0) == vec4<T>(0, 1, 2, 3));
		BOOST_CHECK(m0.get_column(1) == vec4<T>(4, 5, 6, 7));
		BOOST_CHECK(m0.get_column(2) == vec4<T>(8, 9, 10, 11));
		BOOST_CHECK(m0.get_column(3) == vec4<T>(12, 13, 14, 15));
	}
	{
		mat4<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m1(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
		mat4<T> m2(202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600, 538, 612, 686, 760);
		mat4<T> res = m0 * m1;
		BOOST_CHECK(res == m2);
		m0 *= m1;
		BOOST_CHECK(m0 == m2);

#ifdef TEST_AGAINST_EIGEN
		Eigen::Matrix<T, 4, 4> _m0; 
		_m0 <<	1, 5, 9, 13,
				2, 6, 10, 14,
				3, 7, 11, 15,
				4, 8, 12, 16;
		Eigen::Matrix<T, 4, 4> _m1; 
		_m1 <<	5, 9,  13, 17,
				6, 10, 14, 18,
				7, 11, 15, 19,
				8, 12, 16, 20;
		Eigen::Matrix<T, 4, 4> _m2 = _m0 * _m1;
		BOOST_CHECK(memcmp(m2.m, _m2.data(), mat4<T>::element_count * sizeof(float)) == 0);
#endif
	}
	{
		mat4<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m2(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);
		BOOST_CHECK(m0 + m1 == m2);
		m0 += m1;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat4<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m2(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		BOOST_CHECK(m0 - m1 == m2);
		m0 -= m1;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat4<T> m0(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16);
		mat4<T> m1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m2(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		BOOST_CHECK(m2 - m1 == m0);
		m2 -= m1;
		BOOST_CHECK(m2 == m0);
	}

	{
		mat4<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m2(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);
		BOOST_CHECK(m0 * 2 == m2);
		m0 *= 2;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat4<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m2(11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26);
		BOOST_CHECK(m0 + T(10) == m2);
		m0 += 10;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat4<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		mat4<T> m2(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		BOOST_CHECK(m0 - T(1) == m2);
		m0 -= 1;
		BOOST_CHECK(m0 == m2);
	}

	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		mat4<T> m1;
		T x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		m1.set(x);
		BOOST_CHECK(m0 == m1);
	}

	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		m0.set_identity();
		BOOST_CHECK(math::is_identity(m0));
	}
	{
		mat4<T> m0;
		m0.invert();
		BOOST_CHECK(math::is_identity(m0));
	}
	{
		mat4<T> m0(12, 0, 0, 0, 
			0, 9, 0, 0, 
			0, 0, 10, 0, 
			12, 13, 14, 1);
		mat4<T> m1 = inverse(m0);
		BOOST_CHECK(!equals(m1, m0, T(0.0001)));
		BOOST_CHECK(equals(inverse(m1), m0, T(0.0001)));
	}
	{
		mat4<T> m0(12, 0, 0, 0, 
			0, 9, 0, 0, 
			0, 0, 10, 0, 
			12, 13, 14, 1);
		mat4<T> m1 = transposed(m0);
		BOOST_CHECK(!equals(m1, m0));
		BOOST_CHECK(equals(transposed(m1), m0));
		mat4<T> m2(12, 0, 0, 12, 
					0, 9, 0, 13, 
					0, 0, 10, 14, 
					0, 0, 0, 1);
		BOOST_CHECK(equals(m1, m2));
	}

	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		BOOST_CHECK(m0.get_axis_x() == vec4<T>(0, 1, 2, 3));
		m0.set_axis_x(vec4<T>(9, 8, 7, 6));
		BOOST_CHECK(m0.get_axis_x() == vec4<T>(9, 8, 7, 6));

		BOOST_CHECK(m0.get_axis_y() == vec4<T>(4, 5, 6, 7));
		m0.set_axis_y(vec4<T>(9, 8, 7, 6));
		BOOST_CHECK(m0.get_axis_y() == vec4<T>(9, 8, 7, 6));

		BOOST_CHECK(m0.get_axis_z() == vec4<T>(8, 9, 10, 11));
		m0.set_axis_z(vec4<T>(9, 8, 7, 6));
		BOOST_CHECK(m0.get_axis_z() == vec4<T>(9, 8, 7, 6));

		BOOST_CHECK(m0.get_axis_w() == vec4<T>(12, 13, 14, 15));
		m0.set_axis_w(vec4<T>(9, 8, 7, 6));
		BOOST_CHECK(m0.get_axis_w() == vec4<T>(9, 8, 7, 6));
	}

	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		mat4<T> m1(23);
		BOOST_CHECK(m0 != m1);
		BOOST_CHECK(m0 == mat4<T>(m0));
	}
	{
		mat4<T> m0(60, 61, 62, 0, 63, 64, 65, 0, 66, 67, 68, 0, 0, 0, 0, 1);
		mat3<T> m1(60, 61, 62, 63, 64, 65, 66, 67, 68);
		mat4<T> m2(m1);
		mat4<T> m3;
		m3 = m1;
		BOOST_CHECK(m2 == m0);
		BOOST_CHECK(m3 == m0);
	}
	{
		mat4<T> m0(60, 61, 0, 0, 62, 63, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		mat2<T> m1(60, 61, 62, 63);
		mat4<T> m2(m1);
		mat4<T> m3;
		m3 = m1;
		BOOST_CHECK(m2 == m0);
		BOOST_CHECK(m3 == m0);
	}
	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		T* m = m0.data();
		for (uint32_t i = 0; i < 16; i++)
		{
			BOOST_CHECK(m[i] == T(i));
			m[i] = 0;
		}
		BOOST_CHECK(m0 == mat4<T>::zero);
	}
	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		T const* m = m0.data();
		for (uint32_t i = 0; i < 16; i++)
		{
			BOOST_CHECK(m[i] == T(i));
		}
		BOOST_CHECK(m0 == mat4<T>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
	}

	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		BOOST_CHECK(m0(0, 0) == 0);
		BOOST_CHECK(m0(0, 1) == 1);
		BOOST_CHECK(m0(0, 2) == 2);
		BOOST_CHECK(m0(0, 3) == 3);
		BOOST_CHECK(m0(1, 0) == 4);
		BOOST_CHECK(m0(1, 1) == 5);
		BOOST_CHECK(m0(1, 2) == 6);
		BOOST_CHECK(m0(1, 3) == 7);
		BOOST_CHECK(m0(2, 0) == 8);
		BOOST_CHECK(m0(2, 1) == 9);
		BOOST_CHECK(m0(2, 2) == 10);
		BOOST_CHECK(m0(2, 3) == 11);
		BOOST_CHECK(m0(3, 0) == 12);
		BOOST_CHECK(m0(3, 1) == 13);
		BOOST_CHECK(m0(3, 2) == 14);
		BOOST_CHECK(m0(3, 3) == 15);
	}
	{
		mat4<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		for (uint32_t r = 0; r < 4; r++)
		{
			for (uint32_t c = 0; c < 4; c++)
			{
				m0(c, r) = T(r * 10 + c);
			}
		}
		BOOST_CHECK(m0 == mat4<T>(0, 10, 20, 30, 1, 11, 21, 31, 2, 12, 22, 32, 3, 13, 23, 33));
	}


	int a = 0;
}


BOOST_AUTO_TEST_CASE(Mat4)
{
	testMat4<float>();
	testMat4<double>();
}
