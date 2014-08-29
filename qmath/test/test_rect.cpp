#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(Vec3ToVec2Cast)
{
    vec3f a(1.0,2.0,3.0);
	vec2f b(a);

    BOOST_CHECK(b.x == 1.0);
	BOOST_CHECK(b.y == 2.0);
}

}
