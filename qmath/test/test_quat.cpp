#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

#include <vector>

// These are sample tests that show the different features of the framework

using namespace math;

namespace 
{

template<typename T>
void testQuat()
{
	{
		quat<T> a;
		BOOST_CHECK(a.x == 0.f && a.y == 0.f && a.z == 0.f && a.w == 1.f);
	}

	{
		quat<T> a(1, 2, 3, 4);
		BOOST_CHECK(a.x == 1 && a.y == 2 && a.z == 3 && a.w == 4);
	}

	{
		quat<T> a(quat<T>(1, 2, 3, 4));
		BOOST_CHECK(a.x == 1 && a.y == 2 && a.z == 3 && a.w == 4);
	}

	{
		for (uint32_t i = 0; i < 360; i++)
		{
			quat<T> a;
			a.set_from_angle_axis(radians((T)i), vec3<T>(1, 0, 0));
			quat<T> b = quat_axis_x(radians((T)i));
			BOOST_CHECK(equals(a, b));
#ifdef TEST_AGAINST_EIGEN

			Eigen::AngleAxis<T> _aa(radians((T)i), Eigen::Matrix<T, 3, 1>(1, 0, 0));
			Eigen::Quaternion<T> _a(_aa);
			BOOST_CHECK(_a.x() == a.x && _a.y() == a.y && _a.z() == a.z && _a.w() == a.w);

#endif
		}
	}
	{
		for (uint32_t i = 0; i < 360; i++)
		{
			quat<T> a;
			a.set_from_angle_axis(radians((T)i), vec3<T>(0, 1, 0));
			quat<T> b = quat_axis_y(radians((T)i));
			BOOST_CHECK(equals(a, b));
#ifdef TEST_AGAINST_EIGEN

			Eigen::AngleAxis<T> _aa(radians((T)i), Eigen::Matrix<T, 3, 1>(0, 1, 0));
			Eigen::Quaternion<T> _a(_aa);
			BOOST_CHECK(_a.x() == a.x && _a.y() == a.y && _a.z() == a.z && _a.w() == a.w);

#endif
		}
	}
	{
		for (uint32_t i = 0; i < 360; i++)
		{
			quat<T> a;
			a.set_from_angle_axis(radians((T)i), vec3<T>(0, 0, 1));
			quat<T> b = quat_axis_z(radians((T)i));
			BOOST_CHECK(equals(a, b));
#ifdef TEST_AGAINST_EIGEN

			Eigen::AngleAxis<T> _aa(radians((T)i), Eigen::Matrix<T, 3, 1>(0, 0, 1));
			Eigen::Quaternion<T> _a(_aa);
			BOOST_CHECK(_a.x() == a.x && _a.y() == a.y && _a.z() == a.z && _a.w() == a.w);

#endif
		}
	}
	{
		vec3<T> right(1, 0, 0);
		quat<T> a = quat_axis_z(radians(T(90.0)));
		mat3<T> m = a.get_as_mat3();
		vec3<T> up = rotate(a, right);
		BOOST_CHECK(equals(up, vec3<T>(0, 1, 0), T(0.001)));

#ifdef TEST_AGAINST_EIGEN

		Eigen::Matrix<T, 3, 1> _right(1, 0, 0);
		Eigen::AngleAxis<T> _aa(radians(90.f), Eigen::Matrix<T, 3, 1>(0, 0, 1));
		Eigen::Quaternion<T> _q(_aa);
		Eigen::Matrix<T, 3, 3> _m(_q);
		Eigen::Matrix<T, 3, 1> _up = _q * _right;
		BOOST_CHECK(_up.isApprox(Eigen::Matrix<T, 3, 1>(0, 1, 0), T(0.001)));

#endif
	}


	{
		//test Euler constructors
		vec3<T> tv = normalized(vec3<T>(1, 1, 1));

		// ms temporarily disable these long tests 
		//return;

		std::vector<quat<T>> quatAxisX(360);
		std::vector<quat<T>> quatAxisY(360);
		std::vector<quat<T>> quatAxisZ(360);
		for (int i = 0; i < 360; i+=5)
		{
			T angle = radians((T)i);
			quatAxisX[i].set_from_angle_axis(angle, vec3<T>(1, 0, 0));
			quatAxisY[i].set_from_angle_axis(angle, vec3<T>(0, 1, 0));
			quatAxisZ[i].set_from_angle_axis(angle, vec3<T>(0, 0, 1));
		}

		//ZXY = Y * X * Z

		quat<T> aX = quatAxisX[32];
		quat<T> aY = quatAxisY[32];
		quat<T> aZ = quatAxisZ[32];

		// 	quat<T> aXYZ = aZ * aY * aX;

		//test Euler setters getters
		for (int anglex = 0; anglex < 360; anglex += 15)
		{
			vec3<T> angles;
			angles.x = radians((T)anglex);
			quat<T> aX = quatAxisX[anglex];

			printf("\ntesting X = %d", anglex);
			for (int angley = 0; angley < 360; angley += 15)
			{
				angles.y = radians((T)angley);
				quat<T> aY = quatAxisY[angley];

				quat<T> aYX = aX * aY;
				quat<T> aXY = aY * aX;

				for (int anglez = 0; anglez < 360; anglez += 15)
				{
					angles.z = radians((T)anglez);
					//printf("\ntesting (%d, %d, %d)", anglex, angley, anglez);
					quat<T> aZ = quatAxisZ[anglez];

					quat<T> aXYZ = aYX*aZ;
					quat<T> aXZY = aX * aZ * aY;
					quat<T> aYXZ = aXY * aZ;
					quat<T> aYZX = aY * aZ * aX;
					quat<T> aZXY = aZ * aYX;
					quat<T> aZYX = aZ * aXY;

					{
						quat<T> a;
						a.set_from_euler_xyz(angles);
						if (!equals(aXYZ, a))
						{
							BOOST_CHECK(is_one(length(a), T(0.00001)));
							vec3<T> v1 = rotate(aXYZ, tv);
							vec3<T> v2 = rotate(a, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_xzy(angles);
						if (!equals(aXZY, a))
						{
							BOOST_CHECK(is_one(length(a), T(0.00001)));
							vec3<T> v1 = rotate(aXZY, tv);
							vec3<T> v2 = rotate(a, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_yxz(angles);
						if (!equals(aYXZ, a))
						{
							BOOST_CHECK(is_one(length(a), T(0.00001)));
							vec3<T> v1 = rotate(aYXZ, tv);
							vec3<T> v2 = rotate(a, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_yzx(angles);
						if (!equals(aYZX, a))
						{
							BOOST_CHECK(is_one(length(a), T(0.00001)));
							vec3<T> v1 = rotate(aYZX, tv);
							vec3<T> v2 = rotate(a, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_zxy(angles);
						if (!equals(aZXY, a))
						{
							BOOST_CHECK(is_one(length(a), T(0.00001)));
							vec3<T> v1 = rotate(aZXY, tv);
							vec3<T> v2 = rotate(a, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_zyx(angles);
						if (!equals(aZYX, a))
						{
							BOOST_CHECK(is_one(length(a), T(0.00001)));
							vec3<T> v1 = rotate(aZYX, tv);
							vec3<T> v2 = rotate(a, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)));
						}
					}
				}
			}
		}

		//test Euler setters getters
		for (int anglex = 0; anglex < 360; anglex += 15)
		{
			vec3<T> angles;
			angles.x = radians((T)anglex);
			quat<T> aX = quatAxisX[anglex];

			printf("\ntesting X = %d", anglex);
			for (int angley = 0; angley < 360; angley += 15)
			{
				angles.y = radians((T)angley);
				quat<T> aY = quatAxisY[angley];

				quat<T> aYX = aX * aY;
				quat<T> aXY = aY * aX;

				for (int anglez = 0; anglez < 360; anglez += 15)
				{
					angles.z = radians((T)anglez);
					//printf("\ntesting (%d, %d, %d)", anglex, angley, anglez);
					quat<T> aZ = quatAxisZ[anglez];

					{
						quat<T> a;
						a.set_from_euler_xyz(angles);
						vec3<T> angles2;
						a.get_as_euler_xyz(angles2);
						if (!equals(angles, angles2))
						{
							quat<T> a2;
							a2.set_from_euler_xyz(angles2);
							vec3<T> v1 = rotate(aYX * aZ, tv);
							vec3<T> v2 = rotate(a, tv);
							vec3<T> v3 = rotate(a2, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)) && equals(v2, v3, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_xzy(angles);
						vec3<T> angles2;
						a.get_as_euler_xzy(angles2);
						if (!equals(angles, angles2))
						{
							quat<T> a2;
							a2.set_from_euler_xzy(angles2);
							vec3<T> v1 = rotate(aX * aZ * aY, tv);
							vec3<T> v2 = rotate(a, tv);
							vec3<T> v3 = rotate(a2, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)) && equals(v2, v3, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_yxz(angles);
						vec3<T> angles2;
						a.get_as_euler_yxz(angles2);
						if (!equals(angles, angles2))
						{
							quat<T> a2;
							a2.set_from_euler_yxz(angles2);
							vec3<T> v1 = rotate(aXY * aZ, tv);
							vec3<T> v2 = rotate(a, tv);
							vec3<T> v3 = rotate(a2, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)) && equals(v2, v3, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_yzx(angles);
						vec3<T> angles2;
						a.get_as_euler_yzx(angles2);
						if (!equals(angles, angles2))
						{
							quat<T> a2;
							a2.set_from_euler_yzx(angles2);
							vec3<T> v1 = rotate(aY * aZ * aX, tv);
							vec3<T> v2 = rotate(a, tv);
							vec3<T> v3 = rotate(a2, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)) && equals(v2, v3, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_zxy(angles);
						vec3<T> angles2;
						a.get_as_euler_zxy(angles2);
						if (!equals(angles, angles2))
						{
							quat<T> a2;
							a2.set_from_euler_zxy(angles2);
							vec3<T> v1 = rotate(aZ * aYX, tv);
							vec3<T> v2 = rotate(a, tv);
							vec3<T> v3 = rotate(a2, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)) && equals(v2, v3, T(0.001)));
						}
					}

					{
						quat<T> a;
						a.set_from_euler_zyx(angles);
						vec3<T> angles2;
						a.get_as_euler_zyx(angles2);
						if (!equals(angles, angles2))
						{
							quat<T> a2;
							a2.set_from_euler_zyx(angles2);
							vec3<T> v1 = rotate(aZ * aXY, tv);
							vec3<T> v2 = rotate(a, tv);
							vec3<T> v3 = rotate(a2, tv);
							BOOST_CHECK(equals(v1, v2, T(0.001)) && equals(v2, v3, T(0.001)));
						}
					}
				}
			}
		}

	}

}

BOOST_AUTO_TEST_CASE(TESTQUAT)
{
	testQuat<float>();
	testQuat<double>();
}

}
