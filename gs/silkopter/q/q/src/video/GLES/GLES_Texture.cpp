#include "QStdAfx.h"
#include "video/GLES/GLES_Texture.h"
#include "video/GLES/GLES_Interface.h"

using namespace q;
using namespace video;
using namespace data;
using namespace thread;

GLES_Texture::GLES_Texture(Path const& path)
	: Texture(path)
	, m_old_filtering(-1)
	, m_old_use_mipmapping(-1)
	, m_old_wrapping_u(-1)
	, m_old_wrapping_v(-1)
	, m_format(Format::RGBA_8)
	, m_is_npot(true)
	, m_has_mipmaps(false)
{
}

//////////////////////////////////////////////////////////////////////////

void GLES_Texture::unload()
{
	destroy_gl_object();
	//set_is_valid(false);
}
bool GLES_Texture::is_valid() const
{
	return m_gl_id.get() != 0;
}

//////////////////////////////////////////////////////////////////////////

bool GLES_Texture::supports_format(Format format) const
{
	Capabilities caps = System::inst().get_renderer()->get_capabilities();
	switch (format)
	{
	case Format::RGBA_4:
	case Format::RGBA_5551:
	case Format::RGBA_8:
	case Format::RGB_565:
	case Format::RGB_8:
	case Format::I_8:
	case Format::A_8:
	case Format::AI_8:
		{
			return true;
		}
	case Format::RGBA_16:
	case Format::RGB_16:
	case Format::A_16:
	case Format::AI_16:
	case Format::I_16:
		{
			return caps.half_float_texture_support;
		}
	case Format::RGBA_32:
	case Format::RGB_32:
	case Format::A_32:
	case Format::AI_32:
	case Format::I_32:
		{
			return caps.float_texture_support;
		}
	case Format::DEPTH_HALF:
	case Format::DEPTH_FULL:
		{
			return caps.depth_texture_support;
		}
	case Format::RGBA_PVRTC2:
	case Format::RGB_PVRTC2:
	case Format::RGBA_PVRTC4:
	case Format::RGB_PVRTC4:
		{
			return caps.pvrtc_compression_support;
		}
	case Format::RGB_ETC:
		{
			return caps.etc1_compression_support;
		}
	}
	return false;
}

void GLES_Texture::Texture_Deleter::operator()(Handle p)
{
	gles::Interface().iglDeleteTextures(1, &p.value);
}

uint32_t GLES_Texture::create_gl_object()
{
	//is valid after we allocate
	//set_is_valid(false);

	uint32_t id;
	gles::Interface().iglGenTextures(1, &id);
	m_gl_id.reset(Handle(id));
	return id;
}
void GLES_Texture::destroy_gl_object()
{
    m_gl_id.reset();
}

bool GLES_Texture::allocate(Format format, math::vec2u32 const& size)
{
	unload();

	if (!supports_format(format))
	{
		return false;
	}

	m_format = format;
	m_size = size;
	m_is_npot = !math::is_pot(size.x) || !math::is_pot(size.y);
	m_has_mipmaps = false;

	create_gl_object();

	if (get_type() != Type::COMPRESSED)
	{
		upload_data(0, 0);
	}

	//set_is_valid(true);

	return true;
}
void GLES_Texture::upload_data(uint32_t mipmap, uint8_t const* data)
{
	gles::Interface interf;

	gles::iGLint oldTex;
	interf.iglGetIntegerv(gles::iGL_TEXTURE_BINDING_2D, &oldTex);

	interf.iglBindTexture(gles::iGL_TEXTURE_2D, get_gl_id());

	//m_is_npot = !math::isPow2(getSize().x) || !math::isPow2(getSize().y);
	if (m_is_npot)
	{
		if (mipmap > 0)
		{
			QLOG_ERR("Q", "Mipmap {} specified for npot texture '{}'. Ignoring.", mipmap, get_path());
			return;
		}
	}

	m_has_mipmaps = m_has_mipmaps | (mipmap > 0);

	interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_WRAP_S, gles::iGL_CLAMP_TO_EDGE);
	interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_WRAP_T, gles::iGL_CLAMP_TO_EDGE);
	m_old_wrapping_u = m_old_wrapping_v = (uint32_t)Sampler::Wrap::CLAMP;

	if (get_type() == Type::HDR)
	{
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_MIN_FILTER, gles::iGL_NEAREST);
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_MAG_FILTER, gles::iGL_NEAREST);
	}
	else
	{
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_MIN_FILTER, gles::iGL_LINEAR);
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_MAG_FILTER, gles::iGL_LINEAR);
	}
	m_old_filtering = (uint32_t)999999;


	bool isCompressed   = false;
	uint32_t  blockSizeX     = 1;
	uint32_t  blockSizeY     = 1;
	uint32_t  minBlocks      = 1;
	int  internalFormat = -1;
	int  pixelFormat    = -1;
	int  pixelType      = gles::iGL_UNSIGNED_BYTE;
	int	 bitsPerPixel	= 0;

	Format f = get_format();
	switch (f)
	{
	case Format::RGBA_4:
		{
			internalFormat = pixelFormat = gles::iGL_RGBA;
			pixelType = gles::iGL_UNSIGNED_SHORT_4_4_4_4;
			bitsPerPixel = 16;
		}
		break;
	case Format::RGBA_5551:
		{
			internalFormat = pixelFormat = gles::iGL_RGBA;
			pixelType = gles::iGL_UNSIGNED_SHORT_5_5_5_1;
			bitsPerPixel = 16;
		}
		break;
	case Format::RGBA_8:
		{
			internalFormat = pixelFormat = gles::iGL_RGBA;
			bitsPerPixel = 32;
		}
		break;
	case Format::RGB_565:
		{
			internalFormat = pixelFormat = gles::iGL_RGB;
			pixelType = gles::iGL_UNSIGNED_SHORT_5_6_5;
			bitsPerPixel = 16;
		}
		break;
	case Format::RGB_8:
		{
			internalFormat = pixelFormat = gles::iGL_RGB;
			bitsPerPixel = 24;
		}
		break;
	case Format::I_8:
		{
			internalFormat = pixelFormat = gles::iGL_LUMINANCE;
			bitsPerPixel = 8;
		}
		break;
	case Format::A_8:
		{
			internalFormat = pixelFormat = gles::iGL_ALPHA;
			bitsPerPixel = 8;
		}
		break;
	case Format::AI_8:
		{
			internalFormat = pixelFormat = gles::iGL_LUMINANCE_ALPHA;
			bitsPerPixel = 16;
		}
		break;
	case Format::RGBA_16:
		{
			internalFormat = gles::iGL_RGBA16F;
			pixelFormat = gles::iGL_RGBA;
			pixelType = gles::iGL_HALF_FLOAT;
			bitsPerPixel = 64;
		}
		break;
	case Format::RGB_16:
		{
			internalFormat = gles::iGL_RGB16F;
			pixelFormat = gles::iGL_RGB;
			pixelType = gles::iGL_HALF_FLOAT;
			bitsPerPixel = 48;
		}
		break;
	case Format::I_16:
		{
			internalFormat = gles::iGL_LUMINANCE16F;
			pixelFormat = gles::iGL_LUMINANCE;
			pixelType = gles::iGL_HALF_FLOAT;
			bitsPerPixel = 16;
		}
		break;
	case Format::A_16:
		{
			internalFormat = gles::iGL_ALPHA16F;
			pixelFormat = gles::iGL_ALPHA;
			pixelType = gles::iGL_HALF_FLOAT;
			bitsPerPixel = 16;
		}
		break;
	case Format::AI_16:
		{
			internalFormat = gles::iGL_LUMINANCE_ALPHA16F;
			pixelFormat = gles::iGL_LUMINANCE_ALPHA;
			pixelType = gles::iGL_HALF_FLOAT;
			bitsPerPixel = 32;
		}
		break;
	case Format::RGBA_32:
		{
			internalFormat = gles::iGL_RGBA32F;
			pixelFormat = gles::iGL_RGBA;
			pixelType = gles::iGL_FLOAT;
			bitsPerPixel = 128;
		}
		break;
	case Format::RGB_32:
		{
			internalFormat = gles::iGL_RGB32F;
			pixelFormat = gles::iGL_RGB;
			pixelType = gles::iGL_FLOAT;
			bitsPerPixel = 96;
		}
		break;
	case Format::I_32:
		{
			internalFormat = gles::iGL_LUMINANCE32F;
			pixelFormat = gles::iGL_LUMINANCE;
			pixelType = gles::iGL_FLOAT;
			bitsPerPixel = 32;
		}
		break;
	case Format::A_32:
		{
			internalFormat = gles::iGL_ALPHA32F;
			pixelFormat = gles::iGL_ALPHA;
			pixelType = gles::iGL_FLOAT;
			bitsPerPixel = 32;
		}
		break;
	case Format::AI_32:
		{
			internalFormat = gles::iGL_LUMINANCE_ALPHA32F;
			pixelFormat = gles::iGL_LUMINANCE_ALPHA;
			pixelType = gles::iGL_FLOAT;
			bitsPerPixel = 64;
		}
		break;
	case Format::DEPTH_HALF:
		{
			internalFormat = pixelFormat = gles::iGL_DEPTH_COMPONENT;
			pixelType = gles::iGL_UNSIGNED_SHORT;
			bitsPerPixel = 16;
		}
		break;
	case Format::DEPTH_FULL:
		{
			internalFormat = pixelFormat = gles::iGL_DEPTH_COMPONENT;
			pixelType = gles::iGL_UNSIGNED_INT;
			bitsPerPixel = 32;
		}
		break;

#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03
	case Format::RGBA_PVRTC2:
	case Format::RGB_PVRTC2:
		{
			internalFormat = pixelFormat = ((f == Format::RGBA_PVRTC2) ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG);
			blockSizeX = 8;
			blockSizeY = 4;
			minBlocks = 2;
			bitsPerPixel = 2;
			isCompressed = true;
		}
		break;

	case Format::RGBA_PVRTC4:
	case Format::RGB_PVRTC4:
		{
			internalFormat = pixelFormat = ((f == Format::RGBA_PVRTC4) ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG);
			blockSizeX = blockSizeY = 4;
			minBlocks = 2;
			bitsPerPixel = 4;
			isCompressed = true;
		}
		break;
	case Format::RGB_ETC:
		{
			internalFormat = 0x8D64; // GL_ETC1_RGB8_OES
			bitsPerPixel = 4;
			isCompressed = true;
		}
		break;

	default :
		{
			QASSERT(0);
			return;
		}
		break;
	}

	uint32_t width = (mipmap > 0) ? (get_size().x >> mipmap) : get_size().x;
	uint32_t height = (mipmap > 0) ? (get_size().y >> mipmap) : get_size().y;
	uint32_t dataSize = math::max(width / blockSizeX, minBlocks) * math::max(height / blockSizeY, minBlocks) * blockSizeX * blockSizeY * bitsPerPixel / 8;

	if (isCompressed)
	{
		interf.iglCompressedTexImage2D(gles::iGL_TEXTURE_2D, mipmap, internalFormat, width, height, 0, dataSize, data);
	}
	else
	{
		interf.iglTexImage2D(gles::iGL_TEXTURE_2D, mipmap, internalFormat, width, height, 0, pixelFormat, pixelType, data);
	}

	interf.iglBindTexture(gles::iGL_TEXTURE_2D, oldTex);
	//set_is_valid(true);
}

void GLES_Texture::bind(Sampler const& sampler, uint32_t slotIdx) const
{
	QASSERT(is_valid());
	if (!is_valid())
	{
		return;
	}
	gles::Interface interf;
	interf.iglActiveTexture(gles::iGL_TEXTURE0 + slotIdx);
	interf.iglBindTexture(gles::iGL_TEXTURE_2D, get_gl_id());

	bool resetFilters = false;
	int mm = sampler.get_mipmapping() ? 1 : 0;
	if (m_old_use_mipmapping != mm)
	{
		m_old_use_mipmapping = mm;
		resetFilters = true;
	}

	int fil = (int)sampler.get_filtering();

	//filtering might not be supported
	if (get_type() == Type::HDR)
	{
		fil = (uint32_t)Sampler::Filter::NEAREST;
	}

	if (m_old_filtering != fil)
	{
		m_old_filtering = fil;
		resetFilters = true;
	}

	static const gles::iGLuint kWrap[] =
	{
		gles::iGL_REPEAT,
		gles::iGL_CLAMP_TO_EDGE,
	};

	int wu = static_cast<int>(m_is_npot ? Sampler::Wrap::CLAMP : sampler.get_wrapping_u());
	if (m_old_wrapping_u != wu)
	{
		m_old_wrapping_u = wu;
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_WRAP_S, kWrap[wu]);
	}
	int wv = static_cast<int>(m_is_npot ? Sampler::Wrap::CLAMP : sampler.get_wrapping_v());
	if (m_old_wrapping_v != wv)
	{
		m_old_wrapping_v = wv;
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_WRAP_T, kWrap[wv]);
	}

	if (resetFilters)
	{
		gles::iGLuint magFilter = (fil == (uint32_t)Sampler::Filter::NEAREST) ? gles::iGL_NEAREST : gles::iGL_LINEAR;
		gles::iGLuint minFilter;

		if (sampler.get_mipmapping() && has_mipmaps() && !m_is_npot)
		{
			minFilter = (fil == (uint32_t)Sampler::Filter::TRILINEAR)
				? gles::iGL_LINEAR_MIPMAP_LINEAR
				: (fil == (uint32_t)Sampler::Filter::BILINEAR)
					? gles::iGL_LINEAR_MIPMAP_NEAREST
					: gles::iGL_NEAREST_MIPMAP_NEAREST;
		}
		else
		{
			minFilter = (fil == (uint32_t)Sampler::Filter::NEAREST) ? gles::iGL_NEAREST : gles::iGL_LINEAR;
		}

		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_MIN_FILTER, minFilter);
		interf.iglTexParameteri(gles::iGL_TEXTURE_2D, gles::iGL_TEXTURE_MAG_FILTER, magFilter);
	}
}

void GLES_Texture::generate_mipmaps()
{
	//QASSERT_MSG(0, "Not supported");
}

math::vec2u32 const&	GLES_Texture::get_size() const
{
	return m_size;
}
bool GLES_Texture::is_npot() const
{
	return m_is_npot;
}

Texture::Format GLES_Texture::get_format() const
{
	return m_format;
}
bool GLES_Texture::has_mipmaps() const
{
	return m_has_mipmaps;
}

Texture::Type GLES_Texture::get_type() const
{
	return (Type)(((int)m_format) & (~0xFF));
}
