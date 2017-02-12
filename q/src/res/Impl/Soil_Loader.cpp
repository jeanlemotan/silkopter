#include "QStdAfx.h"
#include "Soil_Loader.h"

#include "soil/SOIL.h"

// backend is intendedly hidden
#define STBI_NO_HDR
#include "soil/stb_image_aug.h"

using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace video;

extern int stbi_jpeg_test_memory(stbi_uc const *buffer, int len);
extern int stbi_tga_test_memory(stbi_uc const *buffer, int len);
extern int stbi_bmp_test_memory(stbi_uc const *buffer, int len);
extern int stbi_png_test_memory(stbi_uc const *buffer, int len);
extern int stbi_dds_test_memory(stbi_uc const *buffer, int len);
extern int stbi_psd_test_memory(stbi_uc const *buffer, int len);

//////////////////////////////////////////////////////////////////////////

static bool test_jpg(const uint8_t* data, size_t size)
{
	const stbi_uc* buffer = (const stbi_uc*)data;
	return stbi_jpeg_test_memory(buffer, size) == 1;
}

//////////////////////////////////////////////////////////////////////////

static bool test_tga(const uint8_t* data, size_t size)
{
	const stbi_uc* buffer = (const stbi_uc*)data;
	return stbi_tga_test_memory(buffer, size) == 1;
}

//////////////////////////////////////////////////////////////////////////

static bool test_bmp(const uint8_t* data, size_t size)
{
	const stbi_uc* buffer = (const stbi_uc*)data;
	return stbi_bmp_test_memory(buffer, size) == 1;
}

//////////////////////////////////////////////////////////////////////////

static bool test_png(const uint8_t* data, size_t size)
{
	const stbi_uc* buffer = (const stbi_uc*)data;
	return stbi_png_test_memory(buffer, size) == 1;
}

//////////////////////////////////////////////////////////////////////////

static bool test_dds(const uint8_t* data, size_t size)
{
	const stbi_uc* buffer = (const stbi_uc*)data;
	return stbi_dds_test_memory(buffer, size) == 1;
}

//////////////////////////////////////////////////////////////////////////

// static bool test_psd(const uint8_t* data, size_t size)
// {
// 	const stbi_uc* buffer = (const stbi_uc*)data;
// 	return stbi_psd_test_memory(buffer, size) == 1;
// }

//////////////////////////////////////////////////////////////////////////

static bool test_soil(data::Source& source)
{
	source.seek(0);

	uint8_t buf[128];
	size_t size = math::min(source.get_size(), sizeof(buf));
	size = source.read(buf, size);

	return test_tga(buf, size) || test_png(buf, size) || test_bmp(buf, size) || test_jpg(buf, size) || test_dds(buf, size);
}

//////////////////////////////////////////////////////////////////////////

static void load_soil(Path const& /*path*/, data::Source& source, Resource& r)
{
	r.unload();

    auto* texture = dynamic_cast<Texture*>(&r);
    auto* image = dynamic_cast<Dynamic_Image*>(&r);
	if (!texture && !image)
	{
		QASSERT(0);
		return;
	}
	source.seek(0);

	std::vector<uint8_t> data(source.get_size());
	source.read(data.data(), source.get_size());

	int width, height, channels;
	std::shared_ptr<uint8_t> img(SOIL_load_image_from_memory(data.data(), source.get_size(), &width, &height, &channels, 0), SOIL_free_image_data);

	if (img)
	{
		if (texture)
		{
			//soil bug for 16bpp
			// 			if (channels == 2)
			// 			{
			// 				texture->allocate(Texture::Format::RGB_565, math::vec2u32(width, height));
			// 			}
			if (channels == 3)
			{
				texture->allocate(Texture::Format::RGB_8, math::vec2u32(width, height));
			}
			else if (channels == 4)
			{
				texture->allocate(Texture::Format::RGBA_8, math::vec2u32(width, height));
			}
			if (texture->is_valid())
			{
				texture->upload_data(0, img.get());
				texture->generate_mipmaps();
			}
		}
		else
		{
			//soil bug for 16bpp
			// 			if (channels == 2)
			// 			{
			// 				image->allocate(Image::Format::RGB_565, math::vec2u32(width, height), img.get());
			// 			}
			if (channels == 3)
			{
				image->allocate(Dynamic_Image::Format::RGB_8, math::vec2u32(width, height), img.get());
			}
			else if (channels == 4)
			{
				image->allocate(Dynamic_Image::Format::RGBA_8, math::vec2u32(width, height), img.get());
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

bool Soil_Texture_Loader::can_load_from_source(data::Source& source) const
{
	return test_soil(source);
}

//////////////////////////////////////////////////////////////////////////

void Soil_Texture_Loader::load(Path const& path, data::Source& source, video::Texture& texture) const
{
	texture.unload();
	load_soil(path, source, texture);
}

//////////////////////////////////////////////////////////////////////////

bool Soil_Dynamic_Image_Loader::can_load_from_source(data::Source& source) const
{
	return test_soil(source);
}

//////////////////////////////////////////////////////////////////////////

void Soil_Dynamic_Image_Loader::load(Path const& path, data::Source& source, video::Dynamic_Image& image) const
{
	image.unload();
	load_soil(path, source, image);
}
