#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(BINARY)
{
  	//returns true if the argument is a power of 2.
	BOOST_CHECK( is_pot(2) == true );
	BOOST_CHECK( is_pot(3) == false );
	BOOST_CHECK( is_pot(16) == true );
	
	BOOST_CHECK( get_min_pot(127) == 128 );
	BOOST_CHECK( get_min_pot(128) == 128 );
	BOOST_CHECK( get_min_pot(129) == 256 );
	
	//returns the log2 of the argument
	BOOST_CHECK( get_pot(4) == 2 );
	BOOST_CHECK( get_pot(5) == 2 );
	BOOST_CHECK( get_pot(7) == 2 );
	BOOST_CHECK( get_pot(8) == 3 );
	
	//returns the number of 1 bits in the argument
	BOOST_CHECK( get_on_bits_count(4) == 1 );
	BOOST_CHECK( get_on_bits_count(5) == 2 );
	BOOST_CHECK( get_on_bits_count(7) == 3 );	
}

}
