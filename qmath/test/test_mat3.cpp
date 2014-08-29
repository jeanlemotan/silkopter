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

	BOOST_AUTO_TEST_CASE(Mat3QuatConversion)
	{
		for (int i = 0; i < 360; i++)
		{
			for (int j = -88; j < 88; j++)
			{
				float fangle = math::radians((float)i);
				float uangle = math::radians((float)j);

				quatf qzx = quat_axis_z(fangle) * quat_axis_x(uangle);
				vec3f front = math::rotate(qzx, vec3f(0, -1, 0));
				mat3f mlookat = look_at(front, vec3f(0, 0, 1));
				quatf qlookat;
				qlookat.set_from_mat3(mlookat);

				mat3f mq = qlookat.get_as_mat3();
				BOOST_CHECK(equals(mlookat, mq, 0.001f));

#ifdef TEST_AGAINST_EIGEN
				using namespace Eigen;
				Quaternionf _qzx = Quaternionf(AngleAxisf(fangle, Vector3f(0, 0, 1))) * Quaternionf(AngleAxisf(uangle, Vector3f(1, 0, 0)));
				Vector3f _front = _qzx * Vector3f(0, -1, 0);
				Matrix3f _mlookat(mlookat.m);
				Quaternionf _qlookat(_mlookat);
				Matrix3f _mq(_qlookat);
				mat3f _m3; _m3.set(_mq.data());
				BOOST_CHECK(equals(mq, _m3, 0.001f));
#endif
			}
		}
	}

BOOST_AUTO_TEST_CASE(Mat3Lookat)
{
	for (int i = 0; i < 360; i++)
	{
		for (int j = -88; j < 88; j++)
		{
			float fangle = math::radians((float)i);
			float uangle = math::radians((float)j);

			{
				vec3f front = math::rotate(quat_axis_z(fangle) * quat_axis_x(uangle), vec3f(0, 1, 0));
				mat3f mlookat = look_at(front, vec3f(0, 0, 1));
				quatf qlookat;
				qlookat.set_from_mat3(mlookat);
				{
					mat3f mm = qlookat.get_as_mat3();
					BOOST_CHECK(equals(mm, mlookat, 0.001f));
				}
				vec3f vm = math::transform(mlookat, vec3f(0, 1, 0));
				vec3f vq = math::rotate(qlookat, vec3f(0, 1, 0));
				BOOST_CHECK(equals(vm, front, 0.001f));
				BOOST_CHECK(equals(vq, front, 0.001f));

				break; //xxx
			}
		}
	}
}


using namespace math;

template<typename T>
void testMat3()
{
	mat3<T> m0(
		vec3<T>(0.6f, 0.2f, 0.3f), 
		vec3<T>(0.2f, 0.7f, 0.5f), 
		vec3<T>(0.3f, 0.5f, 0.7f));

	{
		BOOST_CHECK(is_identity(mat3<T>::identity, epsilon<T>()));
		BOOST_CHECK(is_one(mat3<T>::one, epsilon<T>()));
		BOOST_CHECK(is_zero(mat3<T>::zero, epsilon<T>()));
		mat3<T> inv0 = inverse(mat3<T>::identity);
		BOOST_CHECK(is_identity(inv0, epsilon<T>()));
		mat3<T> trans0 = transposed(mat3<T>::identity);
		BOOST_CHECK(is_identity(trans0, epsilon<T>()));
	}

	{
		mat3<T> inv0 = inverse(m0);
		mat3<T> res0 = m0 * inv0;
		BOOST_CHECK(is_identity(res0, T(0.0001)));
	}

	{
		mat3<T> m0(0.72f);
		BOOST_CHECK(m0.get_column(0) == vec3<T>(0.72f));
		BOOST_CHECK(m0.get_column(1) == vec3<T>(0.72f));
		BOOST_CHECK(m0.get_column(2) == vec3<T>(0.72f));
		BOOST_CHECK(m0.get_row(0) == vec3<T>(0.72f));
		BOOST_CHECK(m0.get_row(1) == vec3<T>(0.72f));
		BOOST_CHECK(m0.get_row(2) == vec3<T>(0.72f));
	}
	{
		mat3<T> m0(0.72f);
		mat3<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec3<T>(0.72f));
		BOOST_CHECK(m1.get_column(1) == vec3<T>(0.72f));
		BOOST_CHECK(m1.get_column(2) == vec3<T>(0.72f));
		BOOST_CHECK(m1.get_row(0) == vec3<T>(0.72f));
		BOOST_CHECK(m1.get_row(1) == vec3<T>(0.72f));
		BOOST_CHECK(m1.get_row(2) == vec3<T>(0.72f));
	}
	{
		mat3<int> m0(12);
		mat3<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec3<T>(12));
		BOOST_CHECK(m1.get_column(1) == vec3<T>(12));
		BOOST_CHECK(m1.get_column(2) == vec3<T>(12));
		BOOST_CHECK(m1.get_row(0) == vec3<T>(12));
		BOOST_CHECK(m1.get_row(1) == vec3<T>(12));
		BOOST_CHECK(m1.get_row(2) == vec3<T>(12));
	}
	{
		mat4<T> m0(0.72f);
		mat3<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_column(1) == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_column(2) == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_row(0) == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_row(1) == vec3<T>(0.72f, 0.72f, 0.72f));
		BOOST_CHECK(m1.get_row(2) == vec3<T>(0.72f, 0.72f, 0.72f));
	}
	{
		mat2<T> m0(0.72f);
		mat3<T> m1(m0);
		BOOST_CHECK(m1.get_column(0) == vec3<T>(0.72f, 0.72f, 0.f));
		BOOST_CHECK(m1.get_column(1) == vec3<T>(0.72f, 0.72f, 0.f));
		BOOST_CHECK(m1.get_column(2) == vec3<T>(0.f, 0.f, 1.f));
		BOOST_CHECK(m1.get_row(0) == vec3<T>(0.72f, 0.72f, 0));
		BOOST_CHECK(m1.get_row(1) == vec3<T>(0.72f, 0.72f, 0));
		BOOST_CHECK(m1.get_row(2) == vec3<T>(0, 0, 1));
	}

	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		BOOST_CHECK(m0.get_column(0) == vec3<T>(0, 1, 2));
		BOOST_CHECK(m0.get_column(1) == vec3<T>(3, 4, 5));
		BOOST_CHECK(m0.get_column(2) == vec3<T>(6, 7, 8));
	}
	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		BOOST_CHECK(m0.get_row(0) == vec3<T>(0, 3, 6));
		BOOST_CHECK(m0.get_row(1) == vec3<T>(1, 4, 7));
		BOOST_CHECK(m0.get_row(2) == vec3<T>(2, 5, 8));
	}
	{
		mat3<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m1(5, 6, 7, 8, 9, 10, 11, 12, 13);
		mat3<T> m2(78, 96, 114, 114, 141, 168, 150, 186, 222);
		mat3<T> res = m0 * m1;
		BOOST_CHECK(res == m2);
		m0 *= m1;
		BOOST_CHECK(m0 == m2);

#ifdef TEST_AGAINST_EIGEN
		Eigen::Matrix<T, 3, 3> _m0; 
		_m0 <<	1, 4, 7,
			2, 5, 8,
			3, 6, 9;
		Eigen::Matrix<T, 3, 3> _m1; 
		_m1 <<	5, 8, 11,
				6, 9, 12,
				7, 10, 13;
		Eigen::Matrix<T, 3, 3> _m2 = _m0 * _m1;
		BOOST_CHECK(memcmp(m2.m, _m2.data(), mat3<T>::element_count * sizeof(float)) == 0);
#endif
	}
	{
		mat3<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m1(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m2(2, 4, 6, 8, 10, 12, 14, 16, 18);
		BOOST_CHECK(m0 + m1 == m2);
		m0 += m1;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat3<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m1(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m2(0, 0, 0, 0, 0, 0, 0, 0, 0);
		BOOST_CHECK(m0 - m1 == m2);
		m0 -= m1;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat3<T> m0(-1, -2, -3, -4, -5, -6, -7, -8, -9);
		mat3<T> m1(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m2(0, 0, 0, 0, 0, 0, 0, 0, 0);
		BOOST_CHECK(m2 - m1 == m0);
		m2 -= m1;
		BOOST_CHECK(m2 == m0);
	}

	{
		mat3<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m2(2, 4, 6, 8, 10, 12, 14, 16, 18);
		BOOST_CHECK(m0 * 2 == m2);
		m0 *= 2;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat3<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m2(11, 12, 13, 14, 15, 16, 17, 18, 19);
		BOOST_CHECK(m0 + T(10) == m2);
		m0 += 10;
		BOOST_CHECK(m0 == m2);
	}
	{
		mat3<T> m0(1, 2, 3, 4, 5, 6, 7, 8, 9);
		mat3<T> m2(0, 1, 2, 3, 4, 5, 6, 7, 8);
		BOOST_CHECK(m0 - T(1) == m2);
		m0 -= 1;
		BOOST_CHECK(m0 == m2);
	}

	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		mat3<T> m1;
		T x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		m1.set(x);
		BOOST_CHECK(m0 == m1);
	}

	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		m0.set_identity();
		BOOST_CHECK(math::is_identity(m0));
	}
	{
		mat3<T> m0;
		m0.invert();
		BOOST_CHECK(math::is_identity(m0));
	}
	{
		mat3<T> m0(12, 0, 0,
					0, 9, 0,
					12, 13, 1);
		mat3<T> m1 = inverse(m0);
		BOOST_CHECK(!equals(m1, m0, T(0.0001)));
		BOOST_CHECK(equals(inverse(m1), m0, T(0.0001)));
	}
	{
		mat3<T> m0(12, 0, 0,
					0, 9, 0,
					12, 13, 1);
		mat3<T> m1 = transposed(m0);
		BOOST_CHECK(!equals(m1, m0));
		BOOST_CHECK(equals(transposed(m1), m0));
		mat3<T> m2(12, 0, 12,
					0, 9, 13,
					0, 0, 1);
		BOOST_CHECK(equals(m1, m2));
	}

	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		BOOST_CHECK(m0.get_axis_x() == vec3<T>(0, 1, 2));
		m0.set_axis_x(vec3<T>(9, 8, 7));
		BOOST_CHECK(m0.get_axis_x() == vec3<T>(9, 8, 7));

		BOOST_CHECK(m0.get_axis_y() == vec3<T>(3, 4, 5));
		m0.set_axis_y(vec3<T>(9, 8, 7));
		BOOST_CHECK(m0.get_axis_y() == vec3<T>(9, 8, 7));

		BOOST_CHECK(m0.get_axis_z() == vec3<T>(6, 7, 8));
		m0.set_axis_z(vec3<T>(9, 8, 7));
		BOOST_CHECK(m0.get_axis_z() == vec3<T>(9, 8, 7));
	}

	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		mat3<T> m1(23);
		BOOST_CHECK(m0 != m1);
		BOOST_CHECK(m0 == mat3<T>(m0));
	}
	{
		mat4<T> m0(60, 61, 62, 0, 63, 64, 65, 0, 66, 67, 68, 0, 0, 0, 0, 1);
		mat3<T> m1(60, 61, 62, 63, 64, 65, 66, 67, 68);
		mat3<T> m2(m0);
		mat3<T> m3;
		m3 = m0;
		BOOST_CHECK(m2 == m1);
		BOOST_CHECK(m3 == m1);
	}
	{
		mat3<T> m0(60, 61, 0, 62, 63, 0, 0, 0, 1);
		mat2<T> m1(60, 61, 62, 63);
		mat3<T> m2(m1);
		mat3<T> m3;
		m3 = m1;
		BOOST_CHECK(m2 == m0);
		BOOST_CHECK(m3 == m0);
	}
	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		T* m = m0.data();
		for (uint32_t i = 0; i < mat3<T>::element_count; i++)
		{
			BOOST_CHECK(m[i] == T(i));
			m[i] = 0;
		}
		BOOST_CHECK(m0 == mat3<T>::zero);
	}
	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		T const* m = m0.data();
		for (uint32_t i = 0; i < mat3<T>::element_count; i++)
		{
			BOOST_CHECK(m[i] == T(i));
		}
		BOOST_CHECK(m0 == mat3<T>(0, 1, 2, 3, 4, 5, 6, 7, 8));
	}

	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		BOOST_CHECK(m0(0, 0) == 0);
		BOOST_CHECK(m0(0, 1) == 1);
		BOOST_CHECK(m0(0, 2) == 2);
		BOOST_CHECK(m0(1, 0) == 3);
		BOOST_CHECK(m0(1, 1) == 4);
		BOOST_CHECK(m0(1, 2) == 5);
		BOOST_CHECK(m0(2, 0) == 6);
		BOOST_CHECK(m0(2, 1) == 7);
		BOOST_CHECK(m0(2, 2) == 8);
	}
	{
		mat3<T> m0(0, 1, 2, 3, 4, 5, 6, 7, 8);
		for (uint32_t r = 0; r < mat3<T>::row_count; r++)
		{
			for (uint32_t c = 0; c < mat3<T>::column_count; c++)
			{
				m0(c, r) = T(r * 10 + c);
			}
		}
		BOOST_CHECK(m0 == mat3<T>(0, 10, 20, 1, 11, 21, 2, 12, 22));
	}


	int a = 0;
}


BOOST_AUTO_TEST_CASE(Mat3)
{
	testMat3<float>();
	testMat3<double>();
}


}
