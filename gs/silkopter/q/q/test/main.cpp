#include "q.h"

#include <iostream>
#include <cstdio>

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

//____________________________________________________________________________//

void benchmark()
{
	const int count = 10000;
	std::chrono::steady_clock::time_point c0, c1;
	double dt0, dt1;

	std::cout << std::endl;

	std::cout << "sprintf_s" << std::endl;
	char buf[100];
	c0 = std::chrono::steady_clock::now();
	std::vector<std::string> res0(count);
	for(int i=0; i<count; ++i)
	{
		sprintf(buf, "P1:%d P2:%d P3:%s P4:%s", i, i, "test", "mimi");
		res0[i] = buf;
	}
	c1 = std::chrono::steady_clock::now();
	dt0 = std::chrono::duration<double, std::ratio<1, 1>>(c1 - c0).count();
	std::cout << dt0 << "Seconds" << std::endl;

	std::cout << std::endl;

	std::cout << "util::format(to std::string)" << std::endl;
	c0 = std::chrono::steady_clock::now();
	std::string fmt("P1:{0} P2:{1} P3:{2} P4:{3}");
	for(int i=0; i<count; ++i)
	{
		std::string res = q::util::format<std::string>(fmt, i, i, "test", "mimi");
#ifdef _DEBUG
		if (res != res0[i])
		{
			printf("\n error at %d", i);
		}
#endif
	}
	c1 = std::chrono::steady_clock::now();
	dt1 = std::chrono::duration<double, std::ratio<1, 1>>(c1 - c0).count();
	std::cout << dt1 << "Seconds" << std::endl;

	std::cout << std::endl << "Speed-up: " << dt0/dt1 << std::endl;

	std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(test_String)
{
	using namespace q;

	System::create();

	//String f = util::format<String>("bubu {0} {2} {1} {{ ", 5, 7, 32);
	//f = util::format<String>("bubu {0} {2} {1} {{ ", 5, -7, f);

	benchmark();

	char const* cc = "textexample";
	char const* cu = "TEXTEXAMPLE";
	size_t length = strlen(cc);
	String a(cc);
	BOOST_CHECK(a == cc);
	BOOST_CHECK(a.size() == length);
	for (size_t i = 0; i < a.size(); i++)
	{
		BOOST_CHECK(a[i] == cc[i]);
	}

	BOOST_CHECK(String(String(cc)) == cc);
	BOOST_CHECK(String(cc, cc + length) == cc);
	BOOST_CHECK(String(std::string(cc)) == cc);
	BOOST_CHECK(String(String(cc)) == String(cc));
	BOOST_CHECK(!(String(String(cc)) != cc));
	BOOST_CHECK(!(String(cc, cc + length) != cc));
	BOOST_CHECK(!(String(std::string(cc)) != cc));
	BOOST_CHECK(!(String(String(cc)) != String(cc)));

	BOOST_CHECK(String(String(cc)) == cu);
	BOOST_CHECK(String(cc, cc + length) == cu);
	BOOST_CHECK(String(std::string(cc)) == cu);
	BOOST_CHECK(String(String(cc)) == String(cu));
	BOOST_CHECK(!(String(String(cc)) != cu));
	BOOST_CHECK(!(String(cc, cc + length) != cu));
	BOOST_CHECK(!(String(std::string(cc)) != cu));
	BOOST_CHECK(!(String(String(cc)) != String(cu)));

	for (size_t i = 0; i < a.size(); i++)
	{
		bool t = (a[i] != cu[i]);
		BOOST_CHECK(t);
	}

	System::destroy();
}

//____________________________________________________________________________//

// each test file may contain any number of test cases; each test case has to have unique name
BOOST_AUTO_TEST_CASE( test2 )
{

}

//____________________________________________________________________________//

// EOF
