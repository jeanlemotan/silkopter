#include "QStdAfx.h"
#include "PVR_Loader.h"

using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace video;

#include "pvrtc/PVRTDecompress.h"
#include "pvrtc/PVRTTexture.h"


//////////////////////////////////////////////////////////////////////////

static bool test_pvr(data::Source& source)
{
	source.seek(0);

	uint8_t buf[sizeof(PVRTextureHeaderV3)];
	size_t size = math::min(source.get_size(), sizeof(buf));
	size = source.read(buf, size);

	size_t headerSize = sizeof(PVRTextureHeaderV3);
	if (size >= headerSize)
	{
		const PVRTextureHeaderV3* header = (const PVRTextureHeaderV3*)buf;
		if (header && header->u32Version == PVRTEX3_IDENT)
		{
			return true;
		}
	}

	return false;
}

static void load_pvr(Path const& path, data::Source& source, Resource& r)
{
	auto* texture = rtti::cast_to<Texture>(&r);
	auto* image = rtti::cast_to<Dynamic_Image>(&r);
	if (!texture && !image)
	{
		QASSERT(0);
		return;
	}

	std::vector<uint8_t> data(source.get_size());
	source.read(data.data(), source.get_size());

	size_t const headerSize = sizeof(PVRTextureHeaderV3);
	PVRTextureHeaderV3 const* header = reinterpret_cast<PVRTextureHeaderV3 const*>(data.data());

	//verification
	if (header->u32Version != PVRTEX3_IDENT)
	{
		QLOG_ERR("Q", "Invalid header, '{}' is not a PVR", path);
		return;
	}

	//bool hasMipMaps = (header->u32MIPMapCount > 1);
	bool isCubeMap = header->u32NumFaces > 1;
	QASSERT(!isCubeMap);

	int width  = header->u32Width;
	int height = header->u32Height;

	QASSERT(width > 0  &&  height > 0);

	//default values
	//bool isCompressed   = false;
	uint32_t  blockSizeX     = 1;
	uint32_t  blockSizeY     = 1;
	uint32_t  minBlocks      = 1;
	uint32_t bitsPerPixel	= 0;
	bool supported		= true;
	bool etcFormat		= false;
	bool pvr2Format		= false;
	bool pvr4Format		= false;

	Dynamic_Image::Format imgFormat;
	Texture::Format texFormat;
	if (header->u64PixelFormat == PVRTGENPIXELID4('r','g','b','a',4,4,4,4))
	{
		imgFormat = Dynamic_Image::Format::RGBA_4;
		texFormat = Texture::Format::RGBA_4;
		bitsPerPixel = 16;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID4('r','g','b','a',5,5,5,1))
	{
		imgFormat = Dynamic_Image::Format::RGBA_5551;
		texFormat = Texture::Format::RGBA_5551;
		bitsPerPixel = 16;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID4('r','g','b','a',8,8,8,8))
	{
		imgFormat = Dynamic_Image::Format::RGBA_8;
		texFormat = Texture::Format::RGBA_8;
		bitsPerPixel = 32;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID3('r','g','b',5,6,5))
	{
		imgFormat = Dynamic_Image::Format::RGB_565;
		texFormat = Texture::Format::RGB_565;
		bitsPerPixel = 16;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID3('r','g','b',8,8,8))
	{
		imgFormat = Dynamic_Image::Format::RGB_8;
		texFormat = Texture::Format::RGB_8;
		bitsPerPixel = 24;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID1('a',8))
	{
		imgFormat = Dynamic_Image::Format::A_8;
		texFormat = Texture::Format::A_8;
		bitsPerPixel = 8;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID1('i',8))
	{
		imgFormat = Dynamic_Image::Format::I_8;
		texFormat = Texture::Format::I_8;
		bitsPerPixel = 8;
	}
	else if (header->u64PixelFormat == PVRTGENPIXELID2('a','i',8,8))
	{
		imgFormat = Dynamic_Image::Format::AI_8;
		texFormat = Texture::Format::AI_8;
		bitsPerPixel = 16;
	}
	else if (header->u64PixelFormat == ePVRTPF_PVRTCI_2bpp_RGBA || header->u64PixelFormat == ePVRTPF_PVRTCI_2bpp_RGB)
	{
		imgFormat = Dynamic_Image::Format::RGBA_8;
		texFormat = ((header->u64PixelFormat == ePVRTPF_PVRTCI_2bpp_RGBA) ? Texture::Format::RGBA_PVRTC2 : Texture::Format::RGB_PVRTC2);
		supported = texture && texture->supports_format(texFormat);
		if (!supported)
		{
			QLOG_WARNING("Q", "PVRTC2 texture format not supprted. Will be decompressed");
		}
		blockSizeX = 8;
		blockSizeY = 4;
		minBlocks = 2;
		bitsPerPixel = 2;
		pvr2Format = true;
	}
	else if (header->u64PixelFormat == ePVRTPF_PVRTCI_4bpp_RGBA || header->u64PixelFormat == ePVRTPF_PVRTCI_4bpp_RGB)
	{
		imgFormat = Dynamic_Image::Format::RGBA_8;
		texFormat = ((header->u64PixelFormat == ePVRTPF_PVRTCI_4bpp_RGBA) ? Texture::Format::RGBA_PVRTC4 : Texture::Format::RGB_PVRTC4);
		supported = texture && texture->supports_format(texFormat);
		if (!supported)
		{
			QLOG_WARNING("Q", "PVRTC4 texture format not supprted. Will be decompressed");
		}
		blockSizeX = blockSizeY = 4;
		minBlocks = 2;
		bitsPerPixel = 4;
		pvr4Format = true;
	}
	else if (header->u64PixelFormat == ePVRTPF_ETC1)
	{
		imgFormat = Dynamic_Image::Format::RGBA_8;
		texFormat = Texture::Format::RGB_ETC;
		supported = texture && texture->supports_format(texFormat);
		if (!supported)
		{
			QLOG_WARNING("Q", "ETC1 texture format not supprted. Will be decompressed");
		}
		minBlocks = 4;
		bitsPerPixel = 4;
		etcFormat = true;
	}
	else
	{
		QLOG_ERR("Q", "Unknown texture format");
		return;
	}

	math::vec2u32 size(width, height);

	if (supported)
	{
		if (texture && !texture->allocate(texFormat, size))
		{
			QLOG_ERR("Q", "Unknown texture format");
			return;
		}
	}
	else
	{
		if (texture && !texture->allocate(Texture::Format::RGBA_8, size))
		{
			QLOG_ERR("Q", "Unknown texture format");
			return;
		}
	}

	uint32_t* decompressedData = 0;
	if (!supported)
	{
		decompressedData = new uint32_t[width * height];
	}

	uint8_t const* dataPtr = data.data() + headerSize;
	int fullWidth = width;
	int fullHeight = height;
	for (uint32_t i = 0; i < header->u32NumFaces; i++)
	{
		width = fullWidth;
		height = fullHeight;

		for (uint32_t m = 0; m < header->u32MIPMapCount; ++m)
		{
			int textureLod = m;
			size_t data_size = (math::max(width / blockSizeX, minBlocks) * math::max(height / blockSizeY, minBlocks) * blockSizeX * blockSizeY * bitsPerPixel / 8);

			if (textureLod >= 0)
			{
				if (!supported)
				{
					if (etcFormat)
					{
						PVRTDecompressETC(dataPtr, width, height, (uint8_t*)decompressedData, 0);
					}
					else
					{
						PVRTDecompressPVRTC(dataPtr, pvr2Format ? 1 : 0, width, height, (uint8_t*)decompressedData);
					}

					if (texture)
					{
						texture->upload_data(textureLod, (const uint8_t*)decompressedData);
					}
					else
					{
						image->allocate(Dynamic_Image::Format::RGBA_8, size, (const uint8_t*)decompressedData);
						i = header->u32NumFaces;
						break;
					}
				}
				else
				{
					if (texture)
					{
						texture->upload_data(textureLod, dataPtr);
					}
					else
					{
						image->allocate(imgFormat, size, dataPtr);
						i = header->u32NumFaces;
						break;
					}
				}
			}

			dataPtr += data_size;
			width  = math::max(width >> 1, 1);
			height = math::max(height >> 1, 1);
		}
	}

	delete[] decompressedData;
}

//////////////////////////////////////////////////////////////////////////

bool PVR_Texture_Loader::can_load_from_source(data::Source& source) const
{
	return test_pvr(source);
}

//////////////////////////////////////////////////////////////////////////

void PVR_Texture_Loader::load(Path const& path, data::Source& source, Texture& texture) const
{
	texture.unload();
	load_pvr(path, source, texture);
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

bool PVR_Dynamic_Image_Loader::can_load_from_source(data::Source& source) const
{
	return test_pvr(source);
}

//////////////////////////////////////////////////////////////////////////

void PVR_Dynamic_Image_Loader::load(Path const& path, data::Source& source, Dynamic_Image& image) const
{
	image.unload();
	load_pvr(path, source, image);
}

//////////////////////////////////////////////////////////////////////////

