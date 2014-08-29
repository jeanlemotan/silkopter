#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(LERP)
{
	{
		int8_t a = 10;
		int8_t b = 20;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		uint8_t a = 10;
		uint8_t b = 20;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		int16_t a = 10;
		int16_t b = 20;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		uint16_t a = 10;
		uint16_t b = 20;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}
	
	{
		int32_t a = 10;
		int32_t b = 20;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		uint32_t a = 10;
		uint32_t b = 20;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		float a = 10.f;
		float b = 20.f;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		double a = 10.f;
		double b = 20.f;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		vec2f a(10.f);
		vec2f b(20.f);

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		vec3f a(10.f);
		vec3f b(20.f);

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		vec4f a(10.f);
		vec4f b(20.f);

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == (a + b) / 2 );
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		mat3f a(10.f);
		mat3f b(20.f);

		mat3f c = (a + b) * 0.5f;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == c);
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}
    
	{
		mat4f a(10.f);
		mat4f b(20.f);

		mat4f c = (a + b) * 0.5f;

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == c);
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		quatf a(0.f, 0.f, 0.f, 1.0f);
		quatf b(anglef::_2pi, anglef::_2pi, anglef::_2pi, 1.0f);

		quatf c(anglef::pi, anglef::pi, anglef::pi, 1.f);

		BOOST_CHECK(lerp(a, b, 0.f) == a);
		BOOST_CHECK(lerp(a, b, 0.5f) == c);
		BOOST_CHECK(lerp(a, b, 1.f) == b);
	}

	{
		quatf a(0.f, 0.f, 0.f, 1.0f);
		quatf b(anglef::pi, anglef::pi, anglef::pi, 1.0f);

		quatf c(0.54190516f, 0.54190516f, 0.54190516f, 0.34498754f);
		quatf d(0.56784087f, 0.56784087f, 0.56784087f, 0.18074936f);

		BOOST_CHECK(equals(nlerp(a, b, 0.f), a));
		BOOST_CHECK(equals(nlerp(a, b, 0.5f), c));
		BOOST_CHECK(equals(nlerp(a, b, 1.f), d));
	}
}

}
