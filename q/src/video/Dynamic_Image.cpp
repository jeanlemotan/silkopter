#include "QStdAfx.h"

using namespace q;
using namespace video;

Dynamic_Image::Dynamic_Image(Path const& path)
	: Resource(path)
	, m_format(Format::RGBA_8)
	, m_owns_data(true)
	, m_data_ptr(0)
	, m_pitch(0)
{

}

void Dynamic_Image::allocate(Format format, math::vec2u32 const& size, uint8_t const* data)
{
	deallocate();

	QASSERT(size.x > 0 && size.y > 0);
	m_size = math::max(size, math::vec2u32(1, 1));
	m_format = format;

	size_t data_size = get_data_size();

	m_data.resize(data_size);
	m_data_ptr = m_data.data();
	m_owns_data = true;
	m_pitch = get_line_size();
	if (data)
	{
		memcpy(m_data_ptr, data, data_size);
	}
	else
	{
		memset(m_data_ptr, 0, data_size);
	}
}

void Dynamic_Image::adopt(Format format, math::vec2u32 const& size, size_t pitch, uint8_t* data)
{
	QASSERT(data);
	QASSERT(pitch > 0 && size.x > 0 && size.y > 0);
	deallocate();

	m_size = math::max(size, math::vec2u32(1, 1));
	m_format = format;

	m_data_ptr = data;
	m_owns_data = false;
	m_pitch = pitch;
}

Dynamic_Image::~Dynamic_Image()
{
	deallocate();
}

void Dynamic_Image::deallocate()
{
	std::vector<uint8_t> t;
	std::swap(t, m_data);

	m_data_ptr = 0;
}

Dynamic_Image_uptr	Dynamic_Image::clone() const
{
	auto mi = std::unique_ptr<Dynamic_Image>(new Dynamic_Image);
	if (is_compact())
	{
		mi->allocate(get_format(), get_size(), get_data());
	}
	else
	{
		mi->allocate(get_format(), get_size(), nullptr);
		for (uint32_t i = 0; i < m_size.y; i++)
		{
			memcpy(mi->get_mutable_line_data(i), get_line_data(i), get_line_size());
		}
	}
	return mi;
}

void Dynamic_Image::save_as_tga(data::Sink& sink) const
{
	auto const* src = this;

	Dynamic_Image_uptr tempImg;
	if (m_format != Format::RGBA_8 && m_format != Format::RGB_8)
	{
		tempImg = clone();
		tempImg->convert_to_rgba8();
		src = tempImg.get();
	}

	save_as_tga(sink, src->m_format == Format::RGBA_8, src->m_size, src->m_pitch, src->m_data_ptr);
}

void Dynamic_Image::save_as_tga(data::Sink& sink, bool has_alpha, math::vec2u32 const& size, size_t pitch, uint8_t const* data)
{
	QASSERT(data);
	QASSERT(size.x > 0 && size.y > 0);
	QASSERT(size.x < 65536 && size.y < 65536);

	sink << (uint8_t)0; //idlength
	sink << (uint8_t)0; //colourmaptype
	sink << (uint8_t)2; //datatypecode
	sink << (uint16_t)0; //colourmaporigin
	sink << (uint16_t)0; //colourmaplength
	sink << (uint8_t)0; //colourmapdepth
	sink << (uint16_t)0; //x_origin
	sink << (uint16_t)0; //y_origin
	sink << (uint16_t)size.x; //width
	sink << (uint16_t)size.y; //height
	sink << (uint8_t)(has_alpha ? 32 : 24); //bitsperpixel
	sink << (uint8_t)0; //descriptor

	size_t line_size = size.x * (has_alpha ? 4 : 3);
	boost::auto_buffer<uint8_t, boost::store_n_bytes<8192>> temp;
	temp.uninitialized_resize(line_size);

	if (has_alpha)
	{
		for (int i = size.y - 1; i >= 0; i--)
		{
			uint32_t const* src = (uint32_t const*)(data + pitch * i);
			uint32_t* dst = (uint32_t*)temp.data();
			for (uint32_t x = 0; x < size.x; x++)
			{
				uint32_t c = *src++;
				c = math::color::swap_rb(c);
				*dst++ = c;
			}
			sink.write(temp.data(), line_size);
		}
	}
	else
	{
		for (int i = size.y - 1; i >= 0; i--)
		{
			uint8_t const* src = data + pitch * i;
			uint8_t* dst = temp.data();
			for (uint32_t x = 0; x < size.x; x++)
			{
				dst[2] = *src++;
				dst[1] = *src++;
				dst[0] = *src++;
				dst += 3;
			}
			sink.write(temp.data(), line_size);
		}
	}
}

void Dynamic_Image::unload()
{
	deallocate();
}
bool Dynamic_Image::is_valid() const
{
	return m_data_ptr != nullptr;
}

Dynamic_Image& Dynamic_Image::clear()
{
	if (is_compact())
	{
		memset(m_data_ptr, 0, get_data_size());
	}
	else
	{
		size_t line_size = get_line_size();
		uint8_t* data = m_data_ptr;
		for (size_t i = 0; i < m_size.y; i++)
		{
			memset(data, 0, line_size);
			data += m_pitch;
		}
	}
	return *this;
}

static uint32_t mixColorsRgba8(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	uint32_t ab = 0;
	{
		uint32_t al = (a & 0x00ff00ff);
		uint32_t ah = (a & 0xff00ff00) >> 8;
		uint32_t bl = (b & 0x00ff00ff);
		uint32_t bh = (b & 0xff00ff00) >> 8;
		uint32_t ml = ((al << 7) + (bl << 7));
		uint32_t mh = ((ah << 7) + (bh << 7));
		ab = (mh & 0xff00ff00) | ((ml & 0xff00ff00) >> 8);
	}
	uint32_t cd = 0;
	{
		uint32_t al = (c & 0x00ff00ff);
		uint32_t ah = (c & 0xff00ff00) >> 8;
		uint32_t bl = (d & 0x00ff00ff);
		uint32_t bh = (d & 0xff00ff00) >> 8;
		uint32_t ml = ((al << 7) + (bl << 7));
		uint32_t mh = ((ah << 7) + (bh << 7));
		cd = (mh & 0xff00ff00) | ((ml & 0xff00ff00) >> 8);
	}

	uint32_t al = (ab & 0x00ff00ff);
	uint32_t ah = (ab & 0xff00ff00) >> 8;
	uint32_t bl = (cd & 0x00ff00ff);
	uint32_t bh = (cd & 0xff00ff00) >> 8;
	uint32_t ml = ((al << 7) + (bl << 7));
	uint32_t mh = ((ah << 7) + (bh << 7));
	uint32_t result = (mh & 0xff00ff00) | ((ml & 0xff00ff00) >> 8);
	return result;
}
static uint8_t mixColorsA8(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	uint8_t ab = (a + b) >> 1;
	uint8_t cd = (c + d) >> 1;
	uint8_t result = (ab + cd) >> 1;
	return result;
}
static uint16_t mixColorsAI8(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
	uint32_t ax = (a & 0xff) | ((a & 0xff00) << 8);
	uint32_t bx = (b & 0xff) | ((b & 0xff00) << 8);
	uint32_t abx = ((ax + bx) >> 1) & 0x00ff00ff;

	uint32_t cx = (c & 0xff) | ((c & 0xff00) << 8);
	uint32_t dx = (d & 0xff) | ((d & 0xff00) << 8);
	uint32_t cdx = ((cx + dx) >> 1) & 0x00ff00ff;

	uint32_t abcdx = ((abx + cdx) >> 1) & 0x00ff00ff;
	return static_cast<uint16_t>(abcdx | (abcdx >> 8));
}
static uint16_t mixColorsRgb565(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
	uint32_t ax = (a & 0xf81f) | ((a & 0x07e0) << 16);
	uint32_t bx = (b & 0xf81f) | ((b & 0x07e0) << 16);
	uint32_t abx = ((ax + bx) >> 1) & 0x07e0f81f;

	uint32_t cx = (c & 0xf81f) | ((c & 0x07e0) << 16);
	uint32_t dx = (d & 0xf81f) | ((d & 0x07e0) << 16);
	uint32_t cdx = ((cx + dx) >> 1) & 0x07e0f81f;

	uint32_t abcdx = ((abx + cdx) >> 1) & 0x07e0f81f;
	return static_cast<uint16_t>(abcdx | (abcdx >> 16));
}
static uint16_t mixColorsRgba4(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
	uint32_t ax = (a & 0x0f0f) | ((a & 0xf0f0) << 12);
	uint32_t bx = (b & 0x0f0f) | ((b & 0xf0f0) << 12);
	uint32_t abx = ((ax + bx) >> 1) & 0x0f0f0f0f;

	uint32_t cx = (c & 0x0f0f) | ((c & 0xf0f0) << 12);
	uint32_t dx = (d & 0x0f0f) | ((d & 0xf0f0) << 12);
	uint32_t cdx = ((cx + dx) >> 1) & 0x0f0f0f0f;

	uint32_t abcdx = ((abx + cdx) >> 1) & 0x0f0f0f0f;
	return static_cast<uint16_t>(abcdx | (abcdx >> 12));
}
static uint16_t mixColorsRgba5551(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
	uint32_t ax = (a & 0x7c1f) | ((a & 0x8e30) << 15);
	uint32_t bx = (b & 0x7c1f) | ((b & 0x8e30) << 15);
	uint32_t abx = ((ax + bx) >> 1) & 0x41f07c1f;

	uint32_t cx = (c & 0x7c1f) | ((c & 0x8e30) << 15);
	uint32_t dx = (d & 0x7c1f) | ((d & 0x8e30) << 15);
	uint32_t cdx = ((cx + dx) >> 1) & 0x41f07c1f;

	uint32_t abcdx = ((abx + cdx) >> 1) & 0x41f07c1f;
	return static_cast<uint16_t>(abcdx | (abcdx >> 15));
}
static math::vec4f mixColorsRgba32(math::vec4f const& a, math::vec4f const& b, math::vec4f const& c, math::vec4f const& d)
{
	return (a + b + c + d) * 0.25f;
}
static math::vec3f mixColorsRgb32(math::vec3f const& a, math::vec3f const& b, math::vec3f const& c, math::vec3f const& d)
{
	return (a + b + c + d) * 0.25f;
}

Dynamic_Image& Dynamic_Image::downscale(bool smooth)
{
	if (m_size == math::vec2u32(1, 1) || !is_compact())
	{
		return *this;
	}

	math::vec2u32 newSize(math::max(m_size.x >> 1, 1u), math::max(m_size.y >> 1, 1u));

	auto img = std::make_shared<Dynamic_Image>();
	img->allocate(m_format, newSize, nullptr);

	uint32_t factorX = m_size.x / newSize.x;
	uint32_t factorY = (m_size.y / newSize.y) * m_size.x;
	uint32_t deltaX = factorX - 1;
	uint32_t deltaY = factorY - m_size.x;
	//uint32_t count = newSize.x * newSize.y;

	if (smooth)
	{
		switch (m_format)
		{
		case Format::I_32:
			{
				float* dst = (float*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					float const* src0 = (float const*)m_data_ptr + y * factorY;
					float const* src1 = src0 + deltaX;
					float const* src2 = src0 + deltaY;
					float const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						float a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = (a + b + c + d) * 0.25f;
					}
				}
			}
			break;
		case Format::RGBA_8:
			{
				uint32_t* dst = (uint32_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint32_t const* src0 = (uint32_t const*)m_data_ptr + y * factorY;
					uint32_t const* src1 = src0 + deltaX;
					uint32_t const* src2 = src0 + deltaY;
					uint32_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						uint32_t a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsRgba8(a, b, c, d);
					}
				}
			}
			break;
		case Format::RGBA_32:
			{
				QASSERT(sizeof(math::vec4f) == 16);
				auto* dst = (math::vec4f*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					auto const* src0 = (math::vec4f const*)m_data_ptr + y * factorY;
					auto const* src1 = src0 + deltaX;
					auto const* src2 = src0 + deltaY;
					auto const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						auto a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsRgba32(a, b, c, d);
					}
				}
			}
			break;
		case Format::RGB_32:
			{
				auto* dst = (math::vec3f*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					auto const* src0 = (math::vec3f const*)m_data_ptr + y * factorY;
					auto const* src1 = src0 + deltaX;
					auto const* src2 = src0 + deltaY;
					auto const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						//do the math in vec3 to get some simd love
						math::vec3f a(*src0), b(*src1), c(*src2), d(*src3);
						*dst = mixColorsRgb32(a, b, c, d);
					}
				}
			}
			break;
		case Format::I_8:
		case Format::A_8:
			{
				uint8_t* dst = img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint8_t const* src0 = m_data_ptr + y * factorY;
					uint8_t const* src1 = src0 + deltaX;
					uint8_t const* src2 = src0 + deltaY;
					uint8_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						uint8_t a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsA8(a, b, c, d);
					}
				}
			}
			break;
		case Format::AI_8:
			{
				uint16_t* dst = (uint16_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint16_t const* src0 = (uint16_t const*)m_data_ptr + y * factorY;
					uint16_t const* src1 = src0 + deltaX;
					uint16_t const* src2 = src0 + deltaY;
					uint16_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						uint16_t a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsAI8(a, b, c, d);
					}
				}
			}
			break;
		case Format::RGB_565:
			{
				uint16_t* dst = (uint16_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint16_t const* src0 = (uint16_t const*)m_data_ptr + y * factorY;
					uint16_t const* src1 = src0 + deltaX;
					uint16_t const* src2 = src0 + deltaY;
					uint16_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						uint16_t a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsRgb565(a, b, c, d);
					}
				}
			}
			break;
		case Format::RGBA_4:
			{
				uint16_t* dst = (uint16_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint16_t const* src0 = (uint16_t const*)m_data_ptr + y * factorY;
					uint16_t const* src1 = src0 + deltaX;
					uint16_t const* src2 = src0 + deltaY;
					uint16_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						uint16_t a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsRgba4(a, b, c, d);
					}
				}
			}
			break;
		case Format::RGBA_5551:
			{
				uint16_t* dst = (uint16_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint16_t const* src0 = (uint16_t const*)m_data_ptr + y * factorY;
					uint16_t const* src1 = src0 + deltaX;
					uint16_t const* src2 = src0 + deltaY;
					uint16_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX, dst++)
					{
						uint16_t a = *src0, b = *src1, c = *src2, d = *src3;
						*dst = mixColorsRgba5551(a, b, c, d);
					}
				}
			}
			break;
		case Format::RGB_8:
			{
				factorX *= 3;
				factorY *= 3;
				deltaX *= 3;
				deltaY *= 3;
				uint8_t* dst = img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint8_t const* src0 = m_data_ptr + y * factorY;
					uint8_t const* src1 = src0 + deltaX;
					uint8_t const* src2 = src0 + deltaY;
					uint8_t const* src3 = src1 + deltaY;
					for (uint32_t i = 0; i < newSize.x; i++, src0 += factorX, src1 += factorX, src2 += factorX, src3 += factorX)
					{
						uint32_t a = (*(src0) << 16) | (*(src0 + 1) << 8) | *(src0 + 2);
						uint32_t b = (*(src1) << 16) | (*(src1 + 1) << 8) | *(src1 + 2);
						uint32_t c = (*(src2) << 16) | (*(src2 + 1) << 8) | *(src2 + 2);
						uint32_t d = (*(src3) << 16) | (*(src3 + 1) << 8) | *(src3 + 2);
						uint32_t x = mixColorsRgba8(a, b, c, d);
						*dst++ = (x >> 16) & 0xFF;
						*dst++ = (x >> 8) & 0xFF;
						*dst++ = x & 0xFF;
					}
				}
			}
			break;
		default: QASSERT(0); break;
		}
	}
	else
	{
		switch (m_format)
		{
		case Format::RGBA_32:
			{
				QASSERT(sizeof(math::vec4f) == 16);
				auto* dst = (math::vec4f*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					auto const* src = (math::vec4f const*)m_data_ptr + y * factorY;
					for (uint32_t i = 0; i < newSize.x; i++, src += factorX, dst++)
					{
						*dst = *src;
					}
				}
			}
			break;
		case Format::RGB_32:
			{
				auto* dst = (math::vec3f*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					auto const* src = (math::vec3f const*)m_data_ptr + y * factorY;
					for (uint32_t i = 0; i < newSize.x; i++, src += factorX, dst++)
					{
						*dst = *src;
					}
				}
			}
			break;
		case Format::I_32:
		case Format::RGBA_8:
			{
				QASSERT(sizeof(float) == sizeof(uint32_t));

				uint32_t* dst = (uint32_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint32_t const* src = (uint32_t const*)m_data_ptr + y * factorY;
					for (uint32_t i = 0; i < newSize.x; i++, src += factorX, dst++)
					{
						*dst = *src;
					}
				}
			}
			break;
		case Format::I_8:
		case Format::A_8:
			{
				uint8_t* dst = img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint8_t const* src = m_data_ptr + y * factorY;
					for (uint32_t i = 0; i < newSize.x; i++, src += factorX, dst++)
					{
						*dst = *src;
					}
				}
			}
			break;
		case Format::AI_8:
		case Format::RGB_565:
		case Format::RGBA_4:
		case Format::RGBA_5551:
			{
				uint16_t* dst = (uint16_t*)img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint16_t const* src = (uint16_t const*)m_data_ptr + y * factorY;
					for (uint32_t i = 0; i < newSize.x; i++, src += factorX, dst++)
					{
						*dst = *src;
					}
				}
			}
			break;
		case Format::RGB_8:
			{
				factorX *= 3;
				factorY *= 3;
				uint8_t* dst = img->m_data_ptr;
				for (uint32_t y = 0; y < newSize.y; y++)
				{
					uint8_t const* src = m_data_ptr + y * factorY;
					for (uint32_t i = 0; i < newSize.x; i++, src += factorX)
					{
						*dst++ = *src;
						*dst++ = *(src + 1);
						*dst++ = *(src + 2);
					}
				}
			}
			break;
		default: QASSERT(0); break;
		}
	}

	swap(*img);
	return *this;
}
Dynamic_Image& Dynamic_Image::flip_h()
{
	switch (m_format)
	{
	case Format::A_8:
	case Format::I_8:
		{
			for (uint32_t j = 0; j < m_size.y; j++)
			{
				auto* a = get_mutable_line_data(j);
				auto* b = a + m_size.x - 1;
				for (uint32_t i = 0, szi = m_size.x / 2; i < szi; i++)
				{
					std::swap(*a, *b);
					a++;
					b--;
				}
			}
		}
		break;
	case Format::AI_8:
	case Format::RGBA_4:
	case Format::RGBA_5551:
	case Format::RGB_565:
		{
			for (uint32_t j = 0; j < m_size.y; j++)
			{
				auto* a = (uint16_t*)get_mutable_line_data(j);
				auto* b = a + m_size.x - 1;
				for (uint32_t i = 0, szi = m_size.x / 2; i < szi; i++)
				{
					std::swap(*a, *b);
					a++;
					b--;
				}
			}
		}
		break;
	case Format::RGB_8:
		{
			uint32_t line_size = get_line_size();
			for (uint32_t j = 0; j < m_size.y; j++)
			{
				auto* a = get_mutable_line_data(j);
				auto* b = a + line_size - 3;
				for (uint32_t i = 0, szi = m_size.x / 2; i < szi; i++)
				{
					std::swap(a[0], b[0]);
					std::swap(a[1], b[1]);
					std::swap(a[2], b[2]);
					a += 3; 
					b -= 3;
				}
			}
		}
		break;
	case Format::I_32:
	case Format::RGBA_8:
		{
			QASSERT(sizeof(float) == sizeof(uint32_t));

			for (uint32_t j = 0; j < m_size.y; j++)
			{
				auto* a = (uint32_t*)get_mutable_line_data(j);
				auto* b = a + m_size.x - 1;
				for (uint32_t i = 0, szi = m_size.x / 2; i < szi; i++)
				{
					std::swap(*a, *b);
					a++;
					b--;
				}
			}
		}
		break;
	case Format::RGBA_32:
		{
			QASSERT(sizeof(math::vec4f) == 16);
			for (uint32_t j = 0; j < m_size.y; j++)
			{
				auto* a = (math::vec4f*)get_mutable_line_data(j);
				auto* b = a + m_size.x - 1;
				for (uint32_t i = 0, szi = m_size.x / 2; i < szi; i++)
				{
					std::swap(*a, *b);
					a++;
					b--;
				}
			}
		}
		break;
	case Format::RGB_32:
		{
			for (uint32_t j = 0; j < m_size.y; j++)
			{
				auto* a = (math::vec3f*)get_mutable_line_data(j);
				auto* b = a + m_size.x - 1;
				for (uint32_t i = 0, szi = m_size.x / 2; i < szi; i++)
				{
					std::swap(*a, *b);
					a++;
					b--;
				}
			}
		}
		break;
    default:
        QLOGE("Image format not handled: {}", static_cast<int>(m_format));
        break;

	}
	return *this;
}
Dynamic_Image& Dynamic_Image::flip_v()
{
	boost::auto_buffer<uint8_t, boost::store_n_bytes<8192>> buffer;

	size_t line_size = get_line_size();
	buffer.uninitialized_resize(line_size);

	uint8_t* a = get_mutable_line_data(0);
	uint8_t* b = get_mutable_line_data(m_size.y - 1);
	for (size_t i = 0, sz = m_size.y / 2; i < sz; i++)
	{
		memcpy(buffer.data(), a, line_size);
		memcpy(a, b, line_size);
		memcpy(b, buffer.data(), line_size);
		a += m_pitch;
		b -= m_pitch;
	}
	return *this;
}
Dynamic_Image& Dynamic_Image::flip(bool horizontal, bool vertical)
{
	if (horizontal & vertical)
	{
		flip_v();
		flip_h();
	}
	else if (horizontal)
	{
		flip_v();
	}
	else if (vertical)
	{
		flip_h();
	}
	return *this;
}
Dynamic_Image& Dynamic_Image::rotate_90_cw()
{
	QASSERT(m_owns_data);

	return *this;
}
Dynamic_Image& Dynamic_Image::rotate_90_ccw()
{
	QASSERT(m_owns_data);

	return *this;
}

Dynamic_Image& Dynamic_Image::convert_to_rgba8()
{
	if (!is_compact())
	{
		return *this;
	}
	if (m_format == Format::RGBA_8)
	{
		return *this;
	}

	uint32_t count = m_size.x * m_size.y;

	//since the float and uint32_t have the same storage, we can do this in place
	if (m_format == Format::I_32)
	{
		QASSERT(sizeof(float) == sizeof(uint32_t));

		float const* src = (float const*)m_data_ptr;
		uint32_t* dst = (uint32_t*)m_data_ptr;
		for (uint32_t i = 0; i < count; i++)
		{
			float d = *src++;
			uint8_t dq = (uint8_t)(d * 255.f);
			*dst++ = 0xFF000000 | (dq << 16) | (dq << 8) | dq;
		}
		m_format = Format::RGBA_8;
		return *this;
	}


	auto img = std::make_shared<Dynamic_Image>();
	img->allocate(Format::RGBA_8, m_size, nullptr);

	QASSERT(m_data_ptr && img->m_data_ptr);
	uint32_t* dst = (uint32_t*)img->m_data_ptr;
	switch (m_format)
	{
	case Format::A_8:
		{
			uint8_t const* src = m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint8_t c = *src++;
				*dst++ = c << 24;
			}
		}
		break;
	case Format::I_8:
		{
			uint8_t const* src = m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint8_t c = *src++;
				*dst++ = 0xFF000000 | (c << 16) | (c << 8) | c;
			}
		}
		break;
	case Format::AI_8:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint32_t a = c >> 8;
				uint32_t x = c & 0xFF;
				*dst++ = (a << 24) | (x << 16) | (x << 8) | x;
			}
		}
		break;
	case Format::RGBA_4:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint32_t a = ((c >> 12) & 0xF) << 28;
				uint32_t b = ((c >>  8) & 0xF) << 20;
				uint32_t g = ((c >>  4) & 0xF) << 12;
				uint32_t r = ((c      ) & 0xF) << 4;
				*dst++ = a | b | g | r;
			}
		}
		break;
	case Format::RGBA_5551:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint32_t a = ((c >> 15) & 1) << 31;
				uint32_t b = ((c >>  10) & 31) << 19;
				uint32_t g = ((c >>  5) & 31) << 11;
				uint32_t r = ((c      ) & 31) << 3;
				*dst++ = a | b | g | r;
			}
		}
		break;
	case Format::RGB_565:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint32_t b = ((c >>  11) & 31) << 19;
				uint32_t g = ((c >>  5) & 63) << 10;
				uint32_t r = ((c      ) & 31) << 3;
				*dst++ = 0xFF000000 | b | g | r;
			}
		}
		break;
	case Format::RGB_8:
		{
			uint8_t const* src = m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				//rgb rgb rgb
				//uint8_t layout: rgba rgba rgba -> uint32_t layout: abgr abgr abgr
				uint32_t r = *src++;
				uint32_t g = *src++;
				uint32_t b = *src++;
				*dst++ = 0xFF000000 | (b << 16) | (g << 8) | r;
			}
		}
		break;
	default:
		QASSERT(0);
		break;
	}

	return swap(*img);
}

Dynamic_Image& Dynamic_Image::convert_to_rgb8()
{
	if (!is_compact())
	{
		return *this;
	}
	if (m_format == Format::RGB_8)
	{
		return *this;
	}

	uint32_t count = m_size.x * m_size.y;

	auto img = std::make_shared<Dynamic_Image>();
	img->allocate(Format::RGB_8, m_size, nullptr);

	QASSERT(m_data_ptr && img->m_data_ptr);
	uint8_t* dst = (uint8_t*)img->m_data_ptr;
	switch (m_format)
	{
	case Format::I_32:
		{
			float const* src = (float const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				float d = *src++;
				uint8_t dq = (uint8_t)(d * 255.f);
				*dst++ = dq;
				*dst++ = dq;
				*dst++ = dq;
			}
		}
	case Format::A_8:
		{
			memset(dst, 0, count * 3);
		}
		break;
	case Format::I_8:
		{
			uint8_t const* src = m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint8_t c = *src++;
				*dst++ = c;
				*dst++ = c;
				*dst++ = c;
			}
		}
		break;
	case Format::AI_8:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint8_t x = static_cast<uint8_t>(c & 0xFF);
				*dst++ = x;
				*dst++ = x;
				*dst++ = x;
			}
		}
		break;
	case Format::RGBA_4:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint8_t b = static_cast<uint8_t>(((c >>  8) & 0xF) << 20);
				uint8_t g = static_cast<uint8_t>(((c >>  4) & 0xF) << 12);
				uint8_t r = static_cast<uint8_t>(((c      ) & 0xF) << 4);
				*dst++ = r;
				*dst++ = g;
				*dst++ = b;
			}
		}
		break;
	case Format::RGBA_5551:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint8_t b = static_cast<uint8_t>(((c >>  10) & 31) << 19);
				uint8_t g = static_cast<uint8_t>(((c >>  5) & 31) << 11);
				uint8_t r = static_cast<uint8_t>(((c      ) & 31) << 3);
				*dst++ = r;
				*dst++ = g;
				*dst++ = b;
			}
		}
		break;
	case Format::RGB_565:
		{
			uint16_t const* src = (uint16_t const*)m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				uint16_t c = *src++;
				uint8_t b = static_cast<uint8_t>(((c >>  11) & 31) << 19);
				uint8_t g = static_cast<uint8_t>(((c >>  5) & 63) << 10);
				uint8_t r = static_cast<uint8_t>(((c      ) & 31) << 3);
				*dst++ = r;
				*dst++ = g;
				*dst++ = b;
			}
		}
		break;
	case Format::RGBA_8:
		{
			uint8_t const* src = m_data_ptr;
			for (uint32_t i = 0; i < count; i++)
			{
				src++; //alpha
				uint8_t r = static_cast<uint8_t>(*src++);
				uint8_t g = static_cast<uint8_t>(*src++);
				uint8_t b = static_cast<uint8_t>(*src++);
				*dst++ = r;
				*dst++ = g;
				*dst++ = b;
			}
		}
		break;
	default:
		QASSERT(0);
		break;
	}

	return swap(*img);
}

Dynamic_Image& Dynamic_Image::swap(Dynamic_Image& img)
{
	std::swap(m_format, img.m_format);
	std::swap(m_data_ptr, img.m_data_ptr);
	std::swap(m_data, img.m_data);
	std::swap(m_owns_data, img.m_owns_data);
	std::swap(m_pitch, img.m_pitch);
	std::swap(m_size, img.m_size);
	return *this;
}
