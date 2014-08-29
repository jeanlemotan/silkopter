#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(RANGE)
{
   {
		float a = 1.0f;
		float b = 2.0f;
		float c = 3.0f;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
	}

	{
		double a = 1.0;
		double b = 2.0;
		double c = 3.0;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		int8_t a = 1;
		int8_t b = 2;
		int8_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		uint8_t a = 1;
		uint8_t b = 2;
		uint8_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		int16_t a = 1;
		int16_t b = 2;
	    int16_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		uint16_t a = 1;
		uint16_t b = 2;
		uint16_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}
	
	{
		int32_t a = 1;
		int32_t b = 2;
		int32_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		uint32_t a = 1;
		uint32_t b = 2;
		uint32_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}
	{
		int64_t a = 1;
		int64_t b = 2;
		int64_t c = 3;

		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}
	{
		uint64_t a = 1;
		uint64_t b = 2;
		uint64_t c = 3;
		
		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		vec2f a(1.f,1.f);
		vec2f b(2.f,2.f);
		vec2f c(3.f,3.f);
		
		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		vec3f a(1.f, 1.f, 1.f);
		vec3f b(2.f, 2.f, 2.f);
		vec3f c(3.f, 3.f, 3.f);
		
		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}
	{
		vec4f a(1.f, 1.f, 1.f, 1.f);
		vec4f b(2.f, 2.f, 2.f, 2.f);
		vec4f c(3.f, 3.f, 3.f, 3.f);
		
		BOOST_CHECK(min(a,b) == a);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(max(a,b) == b);
		BOOST_CHECK(clamp(a,b,c) == b);
		BOOST_CHECK(clamp(b,a,c) == b);
		BOOST_CHECK(clamp(c,a,b) == b);
	}

	{
		float a = 1.2f;
		float b = 1.0f;
		float c = 1.8f;
		float d = 2.0f;

		BOOST_CHECK(math::floor(a) == b);
		BOOST_CHECK(math::ceil(a) == d);
		BOOST_CHECK(math::round(a) == b);		
		
		BOOST_CHECK(math::floor(c) == b);
		BOOST_CHECK(math::ceil(c) == d);
		BOOST_CHECK(math::round(c) == d);		
	}

	{
		double a = 1.2;
		double b = 1.0;
		double c = 1.8;
	    double d = 2.0;

		BOOST_CHECK(floor(a) == b);
		BOOST_CHECK(ceil(a) == d);
		BOOST_CHECK(math::round(a) == b);

		BOOST_CHECK(floor(c) == b);
		BOOST_CHECK(ceil(c) == d);
		BOOST_CHECK(math::round(c) == d);		
	}
}

}
