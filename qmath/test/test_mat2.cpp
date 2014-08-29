#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

namespace 
{

using namespace math;

template<typename T>
void testMat2()
{
	mat2<T> m0(
		vec2<T>(0.6f, 0.2f), 
		vec2<T>(0.2f, 0.7f));


	{
		BOOST_CHECK(is_identity(mat2<T>::identity, epsilon<T>()));
		BOOST_CHECK(is_one(mat2<T>::one, epsilon<T>()));
		BOOST_CHECK(is_zero(mat2<T>::zero, epsilon<T>()));
		mat2<T> inv0 = inverse(mat2<T>::identity);
		BOOST_CHECK(is_identity(inv0, epsilon<T>()));
		mat2<T> trans0 = transposed(mat2<T>::identity);
		BOOST_CHECK(is_identity(trans0, epsilon<T>()));
	}

	{
		mat2<T> inv0 = inverse(m0);
		mat2<T> res0 = m0 * inv0;
		BOOST_CHECK(is_identity(res0, T(0.0001)));
	}

	{
		mat2<T> m(mat2<T>::rotation, radians(90.f));
		vec2<T> v(1, 0);
		vec2<T> vt = transform(m, v);
		BOOST_CHECK(equals(vt, vec2<T>(0, 1), T(0.00001)));
		vt = transform(m, vt);
		BOOST_CHECK(equals(vt, vec2<T>(-1, 0), T(0.00001)));
		vt = transform(m, vt);
		BOOST_CHECK(equals(vt, vec2<T>(0, -1), T(0.00001)));

		for (uint32_t i = 0; i < 360; i++)
		{
			T angle = math::radians((T)i);
			mat2<T> m(mat2<T>::rotation, angle);

			vec2<T> v(1, 0);
			vec2<T> vt = transform(m, v);

			vec2<T> ref(v.x*math::cos(angle) - v.y*math::sin(angle), v.y*math::cos(angle) + v.x*math::sin(angle));

			BOOST_CHECK(equals(vt, ref, T(0.00001)));
		}
	}

	{
		mat2<T> m0(0.72f);
		BOOST_CHECK(m0.get_column(0) == vec2<T>(0.72f));
		BOOST_CHECK(m0.get_column(1) == vec2<T>(0.72f));
		BOOST_CHECK(m0.get_row(0) == vec2<T>(0.72f));
		BOOST_CHECK(m0.get_row(1) == vec2<T>(0.72f));
	}
	{
		mat2<T> m0(0.72f);
		mat2<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec2<T>(0.72f));
		BOOST_CHECK(m1.get_column(1) == vec2<T>(0.72f));
		BOOST_CHECK(m1.get_row(0) == vec2<T>(0.72f));
		BOOST_CHECK(m1.get_row(1) == vec2<T>(0.72f));
	}
	{
		mat2<int> m0(12);
		mat2<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec2<T>(12));
		BOOST_CHECK(m1.get_column(1) == vec2<T>(12));
		BOOST_CHECK(m1.get_row(0) == vec2<T>(12));
		BOOST_CHECK(m1.get_row(1) == vec2<T>(12));
	}
	{
		mat4<T> m0(0.72f);
		mat2<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec2<T>(0.72f, 0.72f));
		BOOST_CHECK(m1.get_column(1) == vec2<T>(0.72f, 0.72f));
		BOOST_CHECK(m1.get_row(0) == vec2<T>(0.72f, 0.72f));
		BOOST_CHECK(m1.get_row(1) == vec2<T>(0.72f, 0.72f));
	}
	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		mat2<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec2<T>(0, 1));
		BOOST_CHECK(m1.get_column(1) == vec2<T>(3, 4));
		BOOST_CHECK(m1.get_row(0) == vec2<T>(0, 3));
		BOOST_CHECK(m1.get_row(1) == vec2<T>(1, 4));
	}

	{
		mat2<T> m0(0, 1, 2, 3);
		BOOST_CHECK(m0.get_column(0) == vec2<T>(0, 1));
		BOOST_CHECK(m0.get_column(1) == vec2<T>(2, 3));
	}
	{
		mat2<T> m0(0, 1, 2, 3);
		BOOST_CHECK(m0.get_row(0) == vec2<T>(0, 2));
		BOOST_CHECK(m0.get_row(1) == vec2<T>(1, 3));
	}
	{
		mat2<T> m0(
			vec2<T>(0, 1),
			vec2<T>(2, 3));
		BOOST_CHECK(m0.get_column(0) == vec2<T>(0, 1));
		BOOST_CHECK(m0.get_column(1) == vec2<T>(2, 3));
	}
	{
		mat2<T> m0(1, 2, 3, 4);
		mat2<T> m1(5, 6, 7, 8);
		mat2<T> m2(23, 34, 31, 46);
		mat2<T> res = m0 * m1;
		BOOST_CHECK(res == m2);
		m0 *= m1;
		BOOST_CHECK(m0 == m2);

#ifdef TEST_AGAINST_EIGEN
		Eigen::Matrix<T, 2, 2> _m0; 
		_m0 <<	1, 3, 
			2, 4;
		Eigen::Matrix<T, 2, 2> _m1; 
		_m1 <<	5, 7, 
			6, 8;
		Eigen::Matrix<T, 2, 2> _m2 = _m0 * _m1;
		BOOST_CHECK(memcmp(m2.m, _m2.data(), mat2<T>::element_count * sizeof(float)) == 0);
#endif
	}

	{
		mat2<T> m0(mat2<T>::rotation, radians(90.f));
		mat2<T> m1(mat2<T>::rotation, radians(-45.f));
		mat2<T> m2(mat2<T>::rotation, radians(45.f));

		auto m01 = m0 * m1;
		BOOST_CHECK(equals(m01, m2, T(0.0001)));
	}

	{
		mat2<T> m0(1, 2, 3, 4);
		mat2<T> m1(1, 2, 3, 4);
		mat2<T> m2(2, 4, 6, 8);
		BOOST_CHECK(m0 + m1 == m2);
		m0 += m1;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat2<T> m0(1, 2, 3, 4);
		mat2<T> m1(1, 2, 3, 4);
		mat2<T> m2(0, 0, 0, 0);
		BOOST_CHECK(m0 - m1 == m2);
		m0 -= m1;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat2<T> m0(-1, -2, -3, -4);
		mat2<T> m1(1, 2, 3, 4);
		mat2<T> m2(0, 0, 0, 0);
		BOOST_CHECK(m2 - m1 == m0);
		m2 -= m1;
		BOOST_CHECK(m2 == m0);
	}

	{
		mat2<T> m0(1, 2, 3, 4);
		mat2<T> m2(2, 4, 6, 8);
		BOOST_CHECK(m0 * 2 == m2);
		m0 *= 2;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat2<T> m0(1, 2, 3, 4);
		mat2<T> m2(11, 12, 13, 14);
		BOOST_CHECK(m0 + T(10) == m2);
		m0 += 10;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat2<T> m0(1, 2, 3, 4);
		mat2<T> m2(0, 1, 2, 3);
		BOOST_CHECK(m0 - T(1) == m2);
		m0 -= 1;
		BOOST_CHECK(m0 == m2);
	}

	{
		mat2<T> m0(0, 1, 2, 3);
		mat2<T> m1;
		T x[] = {0, 1, 2, 3};
		m1.set(x);
		BOOST_CHECK(m0 == m1);
	}

	{
		mat2<T> m0(0, 1, 2, 3);
		m0.set_identity();
		BOOST_CHECK(math::is_identity(m0));
	}
	{
		mat2<T> m0;
		m0.invert();
		BOOST_CHECK(math::is_identity(m0));
	}
	{
		mat2<T> m0(12, 0,
					0, 9);
		mat2<T> m1 = inverse(m0);
		BOOST_CHECK(!equals(m1, m0, T(0.0001)));
		BOOST_CHECK(equals(inverse(m1), m0, T(0.0001)));
	}
	{
		mat2<T> m0(12, 1,
					0, 9);
		mat2<T> m1 = transposed(m0);
		BOOST_CHECK(!equals(m1, m0));
		BOOST_CHECK(equals(transposed(m1), m0));
		mat2<T> m2(12, 0,
					1, 9);
		BOOST_CHECK(equals(m1, m2));
	}

	{
		mat2<T> m0(0, 1, 2, 3);
		BOOST_CHECK(m0.get_axis_x() == vec2<T>(0, 1));
		m0.set_axis_x(vec2<T>(9, 8));
		BOOST_CHECK(m0.get_axis_x() == vec2<T>(9, 8));

		BOOST_CHECK(m0.get_axis_y() == vec2<T>(2, 3));
		m0.set_axis_y(vec2<T>(9, 8));
		BOOST_CHECK(m0.get_axis_y() == vec2<T>(9, 8));
	}

	{
		mat2<T> m0(0, 1, 2, 3);
		mat2<T> m1(23);
		BOOST_CHECK(m0 != m1);
		BOOST_CHECK(m0 == mat2<T>(m0));
	}
	{
		mat4<T> m0(60, 61, 62, 0, 63, 64, 65, 0, 66, 67, 68, 0, 0, 0, 0, 1);
		mat2<T> m1(60, 61, 63, 64);
		mat2<T> m2(m0);
		mat2<T> m3;
		m3 = m0;
		BOOST_CHECK(m2 == m1);
		BOOST_CHECK(m3 == m1);
	}
	{
		mat3<T> m0(60, 61, 0, 62, 63, 0, 0, 0, 1);
		mat2<T> m1(60, 61, 62, 63);
		mat2<T> m2(m0);
		mat2<T> m3;
		m3 = m1;
		BOOST_CHECK(m2 == m1);
		BOOST_CHECK(m3 == m1);
	}
	{
		mat2<T> m0(0, 1, 2, 3);
		T* m = m0.data();
		for (uint32_t i = 0; i < mat2<T>::element_count; i++)
		{
			BOOST_CHECK(m[i] == T(i));
			m[i] = 0;
		}
		BOOST_CHECK(m0 == mat2<T>::zero);
	}
	{
		mat2<T> m0(0, 1, 2, 3);
		T const* m = m0.data();
		for (uint32_t i = 0; i < mat2<T>::element_count; i++)
		{
			BOOST_CHECK(m[i] == T(i));
		}
		BOOST_CHECK(m0 == mat2<T>(0, 1, 2, 3));
	}

	{
		mat2<T> m0(0, 1, 2, 3);
		BOOST_CHECK(m0(0, 0) == 0);
		BOOST_CHECK(m0(0, 1) == 1);
		BOOST_CHECK(m0(1, 0) == 2);
		BOOST_CHECK(m0(1, 1) == 3);
	}
	{
		mat2<T> m0(0, 1, 2, 3);
		for (uint32_t r = 0; r < mat2<T>::row_count; r++)
		{
			for (uint32_t c = 0; c < mat2<T>::column_count; c++)
			{
				m0(c, r) = T(r * 10 + c);
			}
		}
		BOOST_CHECK(m0 == mat2<T>(0, 10, 1, 11));
	}


	int a = 0;
}


BOOST_AUTO_TEST_CASE(Mat2)
{
	testMat2<float>();
	testMat2<double>();
}


}
