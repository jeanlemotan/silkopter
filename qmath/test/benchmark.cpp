#include "stdafx.h"
#include "qmath.h"
#include "config.h"

#include <vector>
#include <ctime>
#include <sstream>
#include <iostream>
#include <boost/test/unit_test.hpp>


namespace boost
{
	void throw_exception(class std::exception const &e)
	{
		throw e;
	}
}

using namespace math;

#ifdef NDEBUG
static const int TIMES = 1000;
#else
static const int TIMES = 10;
#endif

static const int COUNT = 1024;

// #define BOOST_AUTO_TEST_CASE(X) void X()
// #define BOOST_FAIL(X) 
// #define BOOST_CHECK(X) 

	std::vector<vec4f> vec4_src(COUNT);
	std::vector<vec4f> vec4_ref(COUNT);
	std::vector<vec4f> vec4_dst1(COUNT);
	std::vector<vec4f> vec4_dst2(COUNT);

	std::vector<vec3f> vec3_src(COUNT);
	std::vector<vec3f> vec3_ref(COUNT);
	std::vector<vec3f> vec3_dst1(COUNT);
	std::vector<vec3f> vec3_dst2(COUNT);

	std::vector<vec3f> mat4_vec3_src(COUNT);
	std::vector<vec3f> mat4_vec3_ref(COUNT);
	std::vector<vec3f> mat4_vec3_dst1(COUNT);
	std::vector<vec3f> mat4_vec3_dst2(COUNT);

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_MAT4_VEC4_1)
	{
#ifdef MATH_USE_SIMD
		std::cout << "SIMD" << std::endl;
#else
		std::cout << "SCALAR" << std::endl;
#endif
		std::cout << COUNT << "x" << TIMES << std::endl;

// 		size_t al = std::alignment_of<vec4<float>>();
// 		al = std::alignment_of<vec4f>();
// 		al = __alignof(vec4<float>);
// 		al = __alignof(vec4f);
// 		al = __alignof(std::max_align_t);

		for (uint32_t i = 0; i < COUNT; i++)
		{
			vec4_src[i] = vec4f(vec4s32(i % 7, i % 23 - 100, i / 5, i / 32));
		}
		for (uint32_t i = 0; i < COUNT; i++)
		{
			vec3_src[i] = vec3f(vec3s32(i % 7, i % 23 - 100, i / 5));
		}

		mat4f mat(quat_axis_z(1.27f).get_as_mat3());

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					auto const& vect = vec4_src[i];
					vec4_ref[i].set(
						vect.x*mat.m[0] + vect.y*mat.m[4] + vect.z*mat.m[ 8] + vect.w*mat.m[12],
						vect.x*mat.m[1] + vect.y*mat.m[5] + vect.z*mat.m[ 9] + vect.w*mat.m[13],
						vect.x*mat.m[2] + vect.y*mat.m[6] + vect.z*mat.m[10] + vect.w*mat.m[14],
						vect.x*mat.m[3] + vect.y*mat.m[7] + vect.z*mat.m[11] + vect.w*mat.m[15]
					);
				}
			}
			std::cout << "scalar mat4*vec4 " << clock() - start << std::endl;
		}

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4_dst2[i] = math::transform(mat, vec4_src[i]);
				}
			}
			std::cout << "math::transform mat4*vec4 " << clock() - start << std::endl;
		}

		BOOST_CHECK(vec4_ref == vec4_dst2);
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_MAT4_VEC4_2)
	{
		mat4f mat;
		mat = quat_axis_z(1.27f).get_as_mat3();

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				math::batch::transform(mat, vec4_dst2.data(), sizeof(vec4f), vec4_src.data(), sizeof(vec4f), COUNT);
			}
			std::cout << "math::batch::transform vec4 " << clock() - start << std::endl;
		}

		BOOST_CHECK(vec4_ref == vec4_dst2);
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_MAT4_VEC3)
	{
		for (uint32_t i = 0; i < COUNT; i++)
		{
			mat4_vec3_src[i] = vec3f(vec3s32(i % 7, i % 23 - 100, i / 5));
		}

		mat4f mat;
		mat = quat_axis_z(1.27f).get_as_mat3();


		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					mat4_vec3_dst1[i] = math::transform(mat, mat4_vec3_src[i]);
				}
			}
			std::cout << "math::transform vec3 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				math::batch::transform(mat, mat4_vec3_dst2.data(), sizeof(vec3f), mat4_vec3_src.data(), sizeof(vec3f), COUNT);
			}
			std::cout << "math::batch::transform vec3 " << clock() - start << std::endl;
		}

		for (uint32_t i = 0; i < mat4_vec3_dst1.size(); i++)
		{
			if (mat4_vec3_dst1[i] != mat4_vec3_dst2[i])
			{
				BOOST_FAIL("Different transform product");
				break;
			}
		}
		BOOST_CHECK(mat4_vec3_dst1 == mat4_vec3_dst2);
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_MAT4_MAT4)
	{
		std::vector<mat4f> src(COUNT);
		std::vector<mat4f> dst1(COUNT);
		std::vector<mat4f> dst2(COUNT);

		for (uint32_t i = 0; i < COUNT; i++)
		{
			auto mat = quat_axis_x(1.27f).get_as_mat3();
			src[i] = mat;
		}

		mat4f mat;
		mat = quat_axis_z(1.27f).get_as_mat3();

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES/10; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					const float* m1 = mat.m;
					const float* m2 = src[i].m;
					float* m3 = dst2[i].m;

					// using this code only on release and when on the iphone
					m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
					m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
					m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
					m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

					m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
					m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
					m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
					m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

					m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
					m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
					m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
					m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

					m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
					m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
					m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
					m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
				}
			}
			std::cout << "scalar mat4f * mat4f " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES/10; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					multiply(dst1[i], mat, src[i]);
				}
			}
			std::cout << "math::multiply mat4f " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES/10; x++)
			{
				math::batch::multiply(mat, dst1.data(), sizeof(math::mat4f), src.data(), sizeof(math::mat4f), COUNT);
			}
			std::cout << "math::batch::multiply mat4f " << clock() - start << std::endl;
		}
		BOOST_CHECK(dst1 == dst2);
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC4_VEC4)
	{
		std::vector<vec4f> src(COUNT);
		std::vector<vec4f> dst1(COUNT);
		std::vector<vec4f> dst2(COUNT);

		for (uint32_t i = 0; i < COUNT; i++)
		{
			src[i] = vec4f(vec4s32(i % 7, i % 23 - 100, i / 5, i / 32));
		}

		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					dst2[i] = vec4f(x * xxx.x, y * xxx.y, z * xxx.z, w * xxx.w);
				}
			}
			std::cout << "scalar vec4*vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					dst1[i] = src[i] * xxx;
				}
			}
			std::cout << "math vec4*vec4 " << clock() - start << std::endl;
		}

		BOOST_CHECK(dst1 == dst2);
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC4_LERP_VEC4)
	{
		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					float t = 0.72f;
					vec4f const& v = vec4_src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					vec4_dst2[i] = vec4f(x + (xxx.x - x)*t, y + (xxx.y - y)*t, z + (xxx.z - z)*t, w + (xxx.w - w)*t);
				}
			}
			std::cout << "scalar vec4 lerp vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4_dst1[i] = math::lerp(vec4_src[i], xxx, 0.72f);
				}
			}
			std::cout << "math vec4 lerp vec4 " << clock() - start << std::endl;
		}

		for (uint32_t i = 0; i < COUNT; i++)
		{
			BOOST_CHECK(equals(vec4_dst1[i], vec4_dst2[i], 0.001f));
		}
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_CLAMP_VEC4)
	{
		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = vec4_src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					vec4_dst2[i] = vec4f(math::clamp(x, -3.f, 2.23f), math::clamp(y, -3.f, 2.23f), math::clamp(z, -3.f, 2.23f), math::clamp(w, -3.f, 2.23f));
				}
			}
			std::cout << "scalar clamp vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				math::vec4f min(-3.f);
				math::vec4f max(2.23f);
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4_dst1[i] = math::clamp(vec4_src[i], min, max);
				}
			}
			std::cout << "math clamp vec4 " << clock() - start << std::endl;
		}

		for (uint32_t i = 0; i < COUNT; i++)
		{
			if (vec4_dst1[i] != vec4_dst2[i])
			{
				int a = 0;
			}
			BOOST_CHECK(vec4_dst1[i] == vec4_dst2[i]);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_ABS_VEC4)
	{
		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = vec4_src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					vec4_dst2[i] = vec4f(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
				}
			}
			std::cout << "scalar abs vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4_dst1[i] = math::abs(vec4_src[i]);
				}
			}
			std::cout << "math abs vec4 " << clock() - start << std::endl;
		}

		BOOST_CHECK(vec4_dst1 == vec4_dst2);
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC4_DOT)
	{
		std::vector<vec4f> src(COUNT);
		std::vector<float> dst1(COUNT);
		std::vector<float> dst2(COUNT);

		for (uint32_t i = 0; i < COUNT; i++)
		{
			src[i] = vec4f(vec4s32(i % 7, i % 23 - 100, i / 5, i / 32));
		}

		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					dst2[i] = xxx.x*x + xxx.y*y + xxx.z*z + xxx.w*w;
				}
			}
			std::cout << "scalar dot vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					dst1[i] = math::dot(src[i], xxx);
				}
			}
			std::cout << "math dot vec4 " << clock() - start << std::endl;
		}

		for (uint32_t i = 0; i < dst1.size(); i++)
		{
			if (!math::equals(dst1[i], dst2[i], 0.0001f))
			{
				BOOST_FAIL("Different dot product");
				break;
			}
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				math::batch::dot(dst1.data(), sizeof(float), src.data(), sizeof(math::vec4f), &xxx, 0, COUNT);
			}
			std::cout << "math::batch dot vec4 " << clock() - start << std::endl;
		}

		for (uint32_t i = 0; i < dst1.size(); i++)
		{
			if (!math::equals(dst1[i], dst2[i], 0.0001f))
			{
				BOOST_FAIL("Different dot product");
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC4_DISTANCE)
	{
		std::vector<vec4f> src(COUNT);
		std::vector<float> dst1(COUNT);
		std::vector<float> dst2(COUNT);

		for (uint32_t i = 0; i < COUNT; i++)
		{
			src[i] = vec4f(vec4s32(i % 7, i % 23 - 100, i / 5, i / 32));
		}

		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = src[i];
					float x = v.x-xxx.x;
					float y = v.y-xxx.y;
					float z = v.z-xxx.z;
					float w = v.w-xxx.w;
					dst2[i] = math::sqrt(x*x + y*y + z*z + w*w);
				}
			}
			std::cout << "scalar distance vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					dst1[i] = math::distance(src[i], xxx);
				}
			}
			std::cout << "math distance vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					dst1[i] = math::distance<float, math::fast>(src[i], xxx);
				}
			}
			std::cout << "math fast distance vec4 " << clock() - start << std::endl;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC4_LENGTH)
	{
		std::vector<vec4f> src(COUNT);
		std::vector<float> dst1(COUNT);
		std::vector<float> dst2(COUNT);

		for (uint32_t i = 0; i < COUNT; i++)
		{
			src[i] = vec4f(vec4s32(i % 7, i % 23 - 100, i / 5, i / 32));
		}

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					dst2[i] = math::sqrt(x*x + y*y + z*z + w*w);
				}
			}
			std::cout << "scalar length vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					dst1[i] = math::length(src[i]);
				}
			}
			std::cout << "math length vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					dst1[i] = math::length<float, math::fast>(src[i]);
				}
			}
			std::cout << "math fast length vec4 " << clock() - start << std::endl;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC4_NORMALIZE)
	{
		vec4f xxx(12.f, 4.f, 7.f, 1.3f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4f const& v = vec4_src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float w = v.w;
					float d = 1.f / math::sqrt(x*x + y*y + z*z + w*w);
					vec4_dst2[i].set(x*d, y*d, z*d, w*d);
				}
			}
			std::cout << "scalar normalize vec4 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4_dst1[i] = math::normalized(vec4_src[i]);
				}
			}
			std::cout << "math normalize vec4 " << clock() - start << std::endl;
		}
		for (uint32_t i = 0; i < vec4_dst1.size(); i++)
		{
			if (!math::equals(vec4_dst1[i], vec4_dst2[i], 0.0001f))
			{
				BOOST_FAIL("Different normalize");
				break;
			}
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec4_dst1[i] = math::normalized<float, math::fast>(vec4_src[i]);
				}
			}
			std::cout << "math fast normalize vec4 " << clock() - start << std::endl;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(BENCHMARK_VEC3_NORMALIZE)
	{
		vec3f xxx(12.f, 4.f, 7.f);

		std::cout << std::endl;

		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec3f const& v = vec3_src[i];
					float x = v.x;
					float y = v.y;
					float z = v.z;
					float d = 1.f / math::sqrt(x*x + y*y + z*z);
					vec3_dst2[i].set(x*d, y*d, z*d);
				}
			}
			std::cout << "scalar normalize vec3 " << clock() - start << std::endl;
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec3_dst1[i] = math::normalized(vec3_src[i]);
				}
			}
			std::cout << "math normalize vec3 " << clock() - start << std::endl;
		}
		for (uint32_t i = 0; i < vec3_dst1.size(); i++)
		{
			if (!math::equals(vec3_dst1[i], vec3_dst2[i], 0.0001f))
			{
				BOOST_FAIL("Different normalize");
				break;
			}
		}
		{
			std::clock_t start = std::clock();
			for (uint32_t x = 0; x < TIMES; x++)
			{
				for (uint32_t i = 0; i < COUNT; i++)
				{
					vec3_dst1[i] = math::normalized<float, math::fast>(vec3_src[i]);
				}
			}
			std::cout << "math fast normalize vec3 " << clock() - start << std::endl;
		}
	}
