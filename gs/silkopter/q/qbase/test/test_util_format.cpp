#include "qbase.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <ctime>

template<class String>
void test_util_format()
{
	String s = util::format<String>("bubu mimi {0} {1} ", 1, "caca");
	BOOST_CHECK(s == "bubu mimi 1 caca ");

	s = util::format<String>("bubu mimi {0} ", -992361);
	BOOST_CHECK(s == "bubu mimi -992361 ");

	s = util::format<String>("bubu mimi {0} ", 9994423);
	BOOST_CHECK(s == "bubu mimi 9994423 ");

	s = util::format<String>("bubu mimi {0} ", 7.2f);
	BOOST_CHECK(s == "bubu mimi 7.2 "); 

	s = util::format<String>("bubu mimi {0} ", -7.2f);
	BOOST_CHECK(s == "bubu mimi -7.2 "); 

	s = util::format<String>("bubu mimi {0} ", "str");
	BOOST_CHECK(s == "bubu mimi str ");

	s = util::format<String>("bubu mimi {0} {1} {2} nn {3} t {4} yy{5:x} ", 1, -6674, 1.23f, -4.32, "str", 32);
	BOOST_CHECK(s == "bubu mimi 1 -6674 1.23 nn -4.32 t str yy20 ");

	//test double precision / range
	s = util::format<String>("bubu mimi {0} ", 1000); BOOST_CHECK(s == "bubu mimi 1000 ");
	s = util::format<String>("bubu mimi {0} ", -1000); BOOST_CHECK(s == "bubu mimi -1000 ");
	s = util::format<String>("bubu mimi {0} ", 1000002); BOOST_CHECK(s == "bubu mimi 1000002 ");
	s = util::format<String>("bubu mimi {0} ", -1000002); BOOST_CHECK(s == "bubu mimi -1000002 ");
	s = util::format<String>("bubu mimi {0} ", 1000000002); BOOST_CHECK(s == "bubu mimi 1000000002 ");
	s = util::format<String>("bubu mimi {0} ", -1000000002); BOOST_CHECK(s == "bubu mimi -1000000002 ");
	s = util::format<String>("bubu mimi {0} ", 1000.0); BOOST_CHECK(s == "bubu mimi 1000 ");
	s = util::format<String>("bubu mimi {0} ", -1000.0); BOOST_CHECK(s == "bubu mimi -1000 ");
	s = util::format<String>("bubu mimi {0} ", 1000002.0); BOOST_CHECK(s == "bubu mimi 1000002 ");
	s = util::format<String>("bubu mimi {0} ", -1000002.0); BOOST_CHECK(s == "bubu mimi -1000002 ");
	s = util::format<String>("bubu mimi {0} ", 1000000002.0); BOOST_CHECK(s == "bubu mimi 1000000002 ");
	s = util::format<String>("bubu mimi {0} ", -1000000002.0); BOOST_CHECK(s == "bubu mimi -1000000002 ");
	s = util::format<String>("bubu mimi {0} ", 1002002002002.0); BOOST_CHECK(s == "bubu mimi 1002002002002 ");
	s = util::format<String>("bubu mimi {0} ", -1002002002002.0); BOOST_CHECK(s == "bubu mimi -1002002002002 ");
	s = util::format<String>("bubu mimi {0} ", 1000.01); BOOST_CHECK(s == "bubu mimi 1000.01 ");
	s = util::format<String>("bubu mimi {0} ", -1000.0123); BOOST_CHECK(s == "bubu mimi -1000.0123 ");
	s = util::format<String>("bubu mimi {0} ", 1000002.01); BOOST_CHECK(s == "bubu mimi 1000002.01 ");
	s = util::format<String>("bubu mimi {0} ", -1000002.0123); BOOST_CHECK(s == "bubu mimi -1000002.0123 ");
	s = util::format<String>("bubu mimi {0} ", 1000000002.01); BOOST_CHECK(s == "bubu mimi 1000000002.01 ");
	s = util::format<String>("bubu mimi {0} ", -1000000002.0123); BOOST_CHECK(s == "bubu mimi -1000000002.0123 ");
	s = util::format<String>("bubu mimi {0} ", 1002003002.01); BOOST_CHECK(s == "bubu mimi 1002003002.01 ");
	s = util::format<String>("bubu mimi {0} ", -1002003002.0123); BOOST_CHECK(s == "bubu mimi -1002003002.0123 ");
	s = util::format<String>("bubu mimi {0} ", 1000.01001); BOOST_CHECK(s == "bubu mimi 1000.01001 ");
	s = util::format<String>("bubu mimi {0} ", -1000.01001); BOOST_CHECK(s == "bubu mimi -1000.01001 ");
	s = util::format<String>("bubu mimi {0} ", 1000002.01001); BOOST_CHECK(s == "bubu mimi 1000002.01001 ");
	s = util::format<String>("bubu mimi {0} ", -1000002.01001); BOOST_CHECK(s == "bubu mimi -1000002.01001 ");
	s = util::format<String>("bubu mimi {0} ", 1000000002.01001); BOOST_CHECK(s == "bubu mimi 1000000002.01001 ");
	s = util::format<String>("bubu mimi {0} ", -1000000002.01001); BOOST_CHECK(s == "bubu mimi -1000000002.01001 ");
	s = util::format<String>("bubu mimi {0} ", 1002003004002.01001); BOOST_CHECK(s == "bubu mimi 1002003004002.01001 ");
	s = util::format<String>("bubu mimi {0} ", -1002003004002.01001); BOOST_CHECK(s == "bubu mimi -1002003004002.01001 ");
	s = util::format<String>("bubu mimi {0} ", 1000.123456); BOOST_CHECK(s == "bubu mimi 1000.12346 ");
	s = util::format<String>("bubu mimi {0} ", -1000.987654); BOOST_CHECK(s == "bubu mimi -1000.98765 ");
	s = util::format<String>("bubu mimi {0} ", 1000002.2345678); BOOST_CHECK(s == "bubu mimi 1000002.23457 ");
	s = util::format<String>("bubu mimi {0} ", -1000002.345678); BOOST_CHECK(s == "bubu mimi -1000002.34568 ");
	s = util::format<String>("bubu mimi {0} ", 1000000002.456789); BOOST_CHECK(s == "bubu mimi 1000000002.45679 ");
	s = util::format<String>("bubu mimi {0} ", -1000000002.56789123); BOOST_CHECK(s == "bubu mimi -1000000002.56789 ");
	s = util::format<String>("bubu mimi {0} ", 1002003002.6754321); BOOST_CHECK(s == "bubu mimi 1002003002.67543 ");
	s = util::format<String>("bubu mimi {0} ", -1002003002.0122345); BOOST_CHECK(s == "bubu mimi -1002003002.01223 ");

	//test hex
	s = util::format<String>("bubu mimi {0:x} ", 1); BOOST_CHECK(s == "bubu mimi 1 ");
	s = util::format<String>("bubu mimi {0:x} ", 3); BOOST_CHECK(s == "bubu mimi 3 ");
	s = util::format<String>("bubu mimi {0:x} ", 9); BOOST_CHECK(s == "bubu mimi 9 ");
	s = util::format<String>("bubu mimi {0:x} ", 11); BOOST_CHECK(s == "bubu mimi b ");
	s = util::format<String>("bubu mimi {0:X} ", 11); BOOST_CHECK(s == "bubu mimi B ");
	s = util::format<String>("bubu mimi {0:X} ", 2235432); BOOST_CHECK(s == "bubu mimi 221C28 ");
	s = util::format<String>("bubu mimi {0:x} ", 2235432); BOOST_CHECK(s == "bubu mimi 221c28 ");

	//std pair
	s = util::format<String>("bubu mimi {0} ", std::make_pair(32, 76.3)); BOOST_CHECK(s == "bubu mimi <32, 76.3> ");
	s = util::format<String>("bubu mimi {0} ", std::make_pair("str", 76.3)); BOOST_CHECK(s == "bubu mimi <str, 76.3> ");
	s = util::format<String>("bubu mimi {0} ", std::make_pair(std::make_pair(32, 76.3), 76.3)); BOOST_CHECK(s == "bubu mimi <<32, 76.3>, 76.3> ");


	{
		using namespace std::chrono;
		s = util::format<String>("bubu mimi {0} ", hours(3)); BOOST_CHECK(s == "bubu mimi 03:00:00.0 ");
		s = util::format<String>("bubu mimi {0} ", minutes(27)); BOOST_CHECK(s == "bubu mimi 00:27:00.0 ");
		s = util::format<String>("bubu mimi {0} ", seconds(112)); BOOST_CHECK(s == "bubu mimi 00:01:52.0 ");
		s = util::format<String>("bubu mimi {0} ", milliseconds(1234)); BOOST_CHECK(s == "bubu mimi 00:00:01.234 ");
		s = util::format<String>("bubu mimi {0} ", hours(3) + minutes(27) + seconds(112)); BOOST_CHECK(s == "bubu mimi 03:28:52.0 ");
		s = s;
	}
};

using namespace q;


BOOST_AUTO_TEST_CASE(TEST_UTIL_FORMAT)
{
	test_util_format<std::string>();
	test_util_format<q::String>();
}