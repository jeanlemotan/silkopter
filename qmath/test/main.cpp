#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace math;

// 	extern void BENCHMARK_MAT4_VEC4_1();
// 	extern void BENCHMARK_MAT4_VEC4_2();
// 	extern void BENCHMARK_MAT4_VEC3();
// 	extern void BENCHMARK_MAT4_MAT4();
// 	extern void BENCHMARK_VEC4_VEC4();
// 	extern void BENCHMARK_VEC4_LERP_VEC4();
// 	extern void BENCHMARK_CLAMP_VEC4();
// 	extern void BENCHMARK_ABS_VEC4();
// 	extern void BENCHMARK_VEC4_DOT();
// 	extern void BENCHMARK_VEC4_DISTANCE();
// 	extern void BENCHMARK_VEC4_NORMALIZE();
// 	extern void BENCHMARK_VEC3_NORMALIZE();
// 
// 	void main( int argc, char* argv[] )
// 	{
// 		BENCHMARK_MAT4_VEC4_1();
// 		BENCHMARK_MAT4_VEC4_2();
// 		BENCHMARK_MAT4_VEC3();
// 		BENCHMARK_MAT4_MAT4();
// 		BENCHMARK_VEC4_VEC4();
// 		BENCHMARK_VEC4_LERP_VEC4();
// 		BENCHMARK_CLAMP_VEC4();
// 		BENCHMARK_ABS_VEC4();
// 		BENCHMARK_VEC4_DOT();
// 		BENCHMARK_VEC4_DISTANCE();
// 		BENCHMARK_VEC4_NORMALIZE();
// 		BENCHMARK_VEC3_NORMALIZE();
// 	}

	BOOST_AUTO_TEST_CASE(SIZES)
	{
		BOOST_CHECK(sizeof(aabb2f) == sizeof(vec2f) * 2);
		BOOST_CHECK(sizeof(aabb3f) == sizeof(vec3f) * 2);
		BOOST_CHECK(sizeof(line2f) == sizeof(vec2f) * 2);
		BOOST_CHECK(sizeof(line3f) == sizeof(vec3f) * 2);
		BOOST_CHECK(sizeof(mat2f) == sizeof(float) * 4);
		BOOST_CHECK(sizeof(mat3f) == sizeof(float) * 9);
		BOOST_CHECK(sizeof(mat4f) == sizeof(float) * 16);
		BOOST_CHECK(sizeof(vec2f) == sizeof(float) * 2);
		BOOST_CHECK(sizeof(vec3f) == sizeof(float) * 3);
		BOOST_CHECK(sizeof(vec4f) == sizeof(float) * 4);
		BOOST_CHECK(sizeof(vec2s16) == sizeof(int16_t) * 2);
		BOOST_CHECK(sizeof(vec3s16) == sizeof(int16_t) * 3);
		BOOST_CHECK(sizeof(vec4s16) == sizeof(int16_t) * 4);
	}


