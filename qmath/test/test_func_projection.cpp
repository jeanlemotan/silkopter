#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(PROJECTION)
{
	{
		mat4f a = math::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.5f, 10.0f);
	
		BOOST_CHECK(equals(a.get_column(0), vec4f(0.2f ,0.0f ,0.0f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(1), vec4f(0.0f ,0.2f ,0.0f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(2), vec4f(0.0f ,0.0f ,-0.21052632f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(3), vec4f(0.0f ,0.0f ,-1.1052631f ,1.0f)));
	}															
	{
		mat4f a = perspective(radians(90.f), 1.0f, 5.f, 10.f);
	
		BOOST_CHECK(equals(a.get_column(0), vec4f(1.0f ,0.0f ,0.0f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(1), vec4f(0.0f ,1.0f ,0.0f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(2), vec4f(0.0f ,0.0f ,-3.0f ,-1.0f)));
		BOOST_CHECK(equals(a.get_column(3), vec4f(0.0f ,0.0f ,-20.0f ,0.0f)));
	}
	{
		mat4f a = perspective(0.f, 20.f, 20.f, 0.f, 5.f, 10.f);
	
		BOOST_CHECK(equals(a.get_column(0), vec4f(0.5f ,0.0f ,0.0f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(1), vec4f(0.0f ,-0.5f ,0.0f ,0.0f)));
		BOOST_CHECK(equals(a.get_column(2), vec4f(1.0f ,-1.0f , -3.0f ,-1.0f)));
		BOOST_CHECK(equals(a.get_column(3), vec4f(0.0f ,0.0f ,-20.0f ,0.0f)));
	}
}

}
