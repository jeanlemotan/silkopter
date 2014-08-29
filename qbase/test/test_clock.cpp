#include "qbase.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

using namespace q;

BOOST_AUTO_TEST_CASE(test_clock)
{
	auto start = Clock::now();

	std::this_thread::sleep_for(std::chrono::milliseconds(32));

	auto d = Clock::now() - start;

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);

	QLOG_INFO("BENCH", "32ms sleep lasted {0}", ms);
}