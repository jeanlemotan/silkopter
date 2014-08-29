#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>


// These are sample tests that show the different features of the framework

using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(angle)
{
    anglef a(3 * anglef::_2pi);

	auto b = math::normalized(a);

   // BOOST_CHECK(equals(b, 0.0f));
	
}

}
