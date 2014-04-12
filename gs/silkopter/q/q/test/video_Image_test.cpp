#include "q.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <ctime>
#include <iostream>
#include <future>


using namespace q;
using namespace video;

template<class Image_T, class New_Color_T>
void save_converted_image(char const* prefix, Image_T& orig)
{
	{
		auto img = orig.template convert_to<New_Color_T>();
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
	{
		auto img = orig.template convert_to<New_Color_T>();
		img.downscale(true);
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}_ds.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
	{
		auto img = orig.template convert_to<New_Color_T>();
		img.downscale(false);
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}_dp.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
	{
		auto img = orig.template convert_to<New_Color_T>();
		img.flip_v();
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}_fv.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
	{
		auto img = orig.template convert_to<New_Color_T>();
		img.flip_h();
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}_fh.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
	{
		auto img = orig.template convert_to<New_Color_T>();
		img.rotate_90_cw();
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}_90cw.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
	{
		auto img = orig.template convert_to<New_Color_T>();
		img.rotate_90_ccw();
		data::File_Sink fs(Path(util::format<String>("{0}_{1}_{2}_90ccw.tga", prefix, Image_T::Color_Traits::get_name(), color::Traits<New_Color_T>::get_name())));
		img.save_as_tga(fs);
	}
}

template<class Image_T>
void test_image()
{
	std::cout << "Testing: " << Image_T::Color_Traits::get_name() << "..." << std::endl;
	auto image = System::inst().get_factory().load<Dynamic_Image>(Path("../test_data/lena.png"));
	BOOST_REQUIRE(image);

	Image<color::RGB_8> img2;
	img2.allocate(image->get_size(), reinterpret_cast<color::RGB_8 const*>(image->get_data()));

	Image_T orig = img2.convert_to<typename Image_T::Color_Type>();

	save_converted_image<Image_T, typename Image_T::Color_Type>("image", orig);
	save_converted_image<Image_T, color::A_8>("image", orig);
	save_converted_image<Image_T, color::I_8>("image", orig);
	save_converted_image<Image_T, color::AI_8>("image", orig);
	save_converted_image<Image_T, color::RGB_565>("image", orig);
	save_converted_image<Image_T, color::RGBA_5551>("image", orig);
	save_converted_image<Image_T, color::RGBA_4>("image", orig);
	save_converted_image<Image_T, color::RGB_8>("image", orig);
	save_converted_image<Image_T, color::RGBA_8>("image", orig);
	save_converted_image<Image_T, color::RGB_32>("image", orig);
	save_converted_image<Image_T, color::RGBA_32>("image", orig);
};
template<class Image_T>
void test_normalmap()
{
	std::cout << "Testing: " << Image_T::Color_Traits::get_name() << "..." << std::endl;
	auto image = System::inst().get_factory().load<Dynamic_Image>(Path("../test_data/normalmap.png"));
	BOOST_REQUIRE(image != nullptr);

	Image<color::RGB_8> img2;
	img2.allocate(image->get_size(), reinterpret_cast<color::RGB_8 const*>(image->get_data()));

	Image_T orig = img2.convert_to<typename Image_T::Color_Type>();

	auto* name = Image_T::Color_Traits::get_name();
	{
		data::File_Sink fs(Path(util::format<String>("normalmap_{0}_orig.tga", name)));
		orig.save_as_tga(fs);
	}
	save_converted_image<Image_T, typename Image_T::Color_Type>("normalmap", orig);
	save_converted_image<Image_T, color::RGB_565>("normalmap", orig);
	save_converted_image<Image_T, color::RGB_8>("normalmap", orig);
	save_converted_image<Image_T, color::RGB_32>("normalmap", orig);
};

BOOST_AUTO_TEST_CASE(video_Dynamic_Image_test)
{
	System::create();

	BOOST_CHECK(sizeof(color::A_8) == 1);
	BOOST_CHECK(sizeof(color::I_8) == 1);
	BOOST_CHECK(sizeof(color::AI_8) == 2);
	BOOST_CHECK(sizeof(color::RGB_565) == 2);
	BOOST_CHECK(sizeof(color::RGBA_5551) == 2);
	BOOST_CHECK(sizeof(color::RGBA_4) == 2);
	BOOST_CHECK(sizeof(color::RGB_8) == 3);
	BOOST_CHECK(sizeof(color::RGBA_8) == 4);
	BOOST_CHECK(sizeof(color::RGB_32) == 12);
	BOOST_CHECK(sizeof(color::RGBA_32) == 16);
	BOOST_CHECK(sizeof(color::I_32) == 4);
	BOOST_CHECK(sizeof(color::NORMAL_8) == 3);
	BOOST_CHECK(sizeof(color::NORMAL_32) == 12);

	{
		std::vector<std::future<void>> results;
		
		results.push_back(std::async([]()
		{ 
			test_normalmap<Image<color::RGB_8>>();
			test_normalmap<Image<color::NORMAL_8>>();
		}));
		results.push_back(std::async([]()
		{ 
			test_normalmap<Image<color::NORMAL_32>>();
			test_image<Image<color::I_8>>();
		}));
		results.push_back(std::async([]()
		{ 
			test_image<Image<color::A_8>>();
			test_image<Image<color::AI_8>>();
		}));
		results.push_back(std::async([]()
		{ 
			test_image<Image<color::RGBA_4>>();
			test_image<Image<color::RGB_565>>();
		}));
		results.push_back(std::async([]()
		{ 
			test_image<Image<color::RGBA_5551>>();
			test_image<Image<color::RGBA_8>>();
		}));
		results.push_back(std::async([]()
		{ 
			test_image<Image<color::RGB_8>>();
			test_image<Image<color::RGBA_32>>();
		}));
		results.push_back(std::async([]()
		{ 
			test_image<Image<color::RGB_32>>();
			test_image<Image<color::I_32>>();
		}));

		for (auto& f: results)
		{
			f.get();
		}
	}

}
