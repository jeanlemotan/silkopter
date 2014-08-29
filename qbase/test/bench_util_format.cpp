#include "qbase.h"

#include <iostream>

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

//____________________________________________________________________________//

void benchmark()
{
#ifdef _DEBUG
	const int count = 1000;
#else
	const int count = 10000000;
#endif
	std::chrono::steady_clock::time_point c0, c1;
	double dt0, dt1;

	std::cout << std::endl;

	std::cout << "sprintf_s" << std::endl;
	char buf[256];
	c0 = std::chrono::steady_clock::now();
	std::vector<std::string> res0(count);
	for(int i = 0; i < count; ++i)
	{
		sprintf_s(buf, 100, "P1:%d P2:%d P3:%s P4:%f P5:%f", i%4, i%3, "test", 1432123.2341f, 1432123.2341f);
		res0[i] = buf;
	}
	c1 = std::chrono::steady_clock::now();
	dt0 = std::chrono::duration<double, std::ratio<1, 1>>(c1 - c0).count(); 
	std::cout << dt0 << "seconds" << std::endl;

	std::cout << std::endl;

	{
		std::cout << "util::format(to std::string)" << std::endl;
		c0 = std::chrono::steady_clock::now();
		for(int i = 0; i < count; ++i)
		{
			std::string res = q::util::format<std::string>("P1:{0} P2:{1} P3:{2} P4:{3} P5:{4}", i%4, i%3, "test", 1432123.2341f, 1432123.2341f);
		}
		c1 = std::chrono::steady_clock::now();
		dt1 = std::chrono::duration<double, std::ratio<1, 1>>(c1 - c0).count(); 
		std::cout << dt1 << "seconds" << std::endl;
		std::cout << "Speed-up: " << dt0/dt1 << std::endl << std::endl;
	}

	{
		std::cout << "util::format(to q::String)" << std::endl;
		c0 = std::chrono::steady_clock::now();
		for(int i = 0; i < count; ++i)
		{
			q::String res = q::util::format<q::String>("P1:{0} P2:{1} P3:{2} P4:{3} P5:{4}", i%4, i%3, "test", 1432123.2341f, 1432123.2341f);
		}
		c1 = std::chrono::steady_clock::now();
		dt1 = std::chrono::duration<double, std::ratio<1, 1>>(c1 - c0).count(); 
		std::cout << dt1 << "seconds" << std::endl;
		std::cout << "Speed-up: " << dt0/dt1 << std::endl << std::endl;
	}

	for(int i = 0; i < count; ++i)
	{
		if (i == res0[0][0])
		{
			printf("%s", res0[0].c_str());
		}
	}

	std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(test_String)							    
{
	using namespace q;

	//String f = util::format<String>("bubu {0} {2} {1} {{ ", 5, 7, 32);
	//f = util::format<String>("bubu {0} {2} {1} {{ ", 5, -7, f);


	char const* cc = "textexample";
	char const* cu = "TEXTEXAMPLE";
	size_t length = strlen(cc);
	String a(cc);
	BOOST_CHECK(a == cc);
	BOOST_CHECK(a.size() == length);
	for (u32 i = 0; i < a.size(); i++)
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

	for (u32 i = 0; i < a.size(); i++)
	{
		bool t = (a[i] != cu[i]);
		BOOST_CHECK(t);
	}

	benchmark();
}

//____________________________________________________________________________//

// each test file may contain any number of test cases; each test case has to have unique name
BOOST_AUTO_TEST_CASE( test2 )
{

}

//____________________________________________________________________________//

// EOF