#include "qbase.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <ctime>

using namespace q;

//____________________________________________________________________________//

static void benchmark(u8* dst, u8 const* src, size_t size)
{
	return;

	u32 elementSizes[] = {1, 4, 5, 8, 12, 16, 30, 32};
	u32 elementSizesCount = sizeof(elementSizes) / sizeof(u32);

	u32 stride = 32;

	size = size / stride;


	//warm the cache
	for (u32 x = 0; x < 512; x++)
	{
		q::util::memcpy(dst, stride, src, stride, 4, size / 4);
	}

	QLOG_INFO("BENCH", "\nStride {0}, Size {1}", stride, size);
	for (u32 ali = 0; ali < 5; ali++)
	{
		QLOG_INFO("BENCH", "\nAlignment {0}", ali);
		for (u32 es = 0; es < elementSizesCount; es++)
		{
			u32 k = elementSizes[es];
			u32 count = (size - stride - ali) / k - 1;

			QLOG_INFO("BENCH", "\n\tElement Size {0}, Count {1}", k, count);

			std::clock_t start = std::clock();

			for (u32 x = 0; x < 128; x++)
			{
				//reference
				u8* dst0 = dst + ali;
				u8 const* src0 = src + ali;
				for (u32 i = 0; i < count; i++)
				{
					::memcpy(dst0, src0, k);
					dst0 += stride;
					src0 += stride;
				}
			}
			QLOG_INFO("BENCH", "\n\t\t\tReference:\t{0}ms", (int)(std::clock() - start));
			

			start = std::clock();
			for (u32 x = 0; x < 128; x++)
			{
				q::util::memcpy(dst + ali, stride, src + ali, stride, k, count);
			}
			QLOG_INFO("BENCH", "\n\t\t\tmemcpy   :\t{0}ms", (int)(std::clock() - start));
		}
	}
}

BOOST_AUTO_TEST_CASE(test_memset)  
{
	size_t size = 1024*1024*8;
	std::vector<u8> src(size);
	std::vector<u8> dst(size);

	std::fill(src.begin(), src.end(), 0);

	benchmark(src.data(), dst.data(), size);
}

//____________________________________________________________________________//

// EOF