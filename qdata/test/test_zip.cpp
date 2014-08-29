#include "QData.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <chrono>
#include <ctime>

namespace boost
{
void throw_exception(std::exception const& e)
{
}
}

using namespace q;
using namespace data;

BOOST_AUTO_TEST_CASE(test_data_ZipDataPack)
{
	std::vector<uint8_t> comp_file;
	std::vector<uint8_t> uncomp_file;
	{
		Zip_Data_Pack pack(File_System_ptr(), Path("../test_data/uncompressed.zip"));
		pack.get_count();
		int idx = pack.find_idx_by_path(Path("xcode4/q.xcodeproj/project.pbxproj"));
		BOOST_REQUIRE(idx >= 0);
		auto source = pack.open(Path("xcode4/q.xcodeproj/project.pbxproj"));
		BOOST_REQUIRE(source);
		BOOST_CHECK(source->get_size() == 67550);
		comp_file = read_whole_source(*source);
		BOOST_CHECK(comp_file.size() == 67550);
	}
	{
		Zip_Data_Pack pack(File_System_ptr(), Path("../test_data/compressed.zip"));
		pack.get_count();
		int idx = pack.find_idx_by_path(Path("xcode4/q.xcodeproj/project.pbxproj"));
		BOOST_REQUIRE(idx >= 0);
		auto source = pack.open(Path("xcode4/q.xcodeproj/project.pbxproj"));
		BOOST_REQUIRE(source);
		BOOST_CHECK(source->get_size() == 67550);
		uncomp_file = read_whole_source(*source);
		BOOST_CHECK(uncomp_file.size() == 67550);
	}

	BOOST_CHECK(comp_file == uncomp_file);
}
