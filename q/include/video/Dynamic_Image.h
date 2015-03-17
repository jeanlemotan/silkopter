#pragma once

#include "res/Resource.h"
#include "video/Color.h"
#include "Ptr_Fw_Declaration.h"

namespace q
{
namespace video
{

	class Dynamic_Image : public res::Resource
	{
		DEFINE_RTTI_CLASS(q::video::Dynamic_Image, q::res::Resource);
	public:
		enum class Format : uint8_t
		{
			RGBA_4,
			RGB_565,
			RGBA_5551,
			RGB_8,
			RGBA_8,
			RGB_32,
			RGBA_32,
			I_8,
			A_8,
			AI_8,
			I_32,
			NORMAL_8,
			NORMAL_32
		};

		Dynamic_Image(Path const& path = Path());
		virtual ~Dynamic_Image();

		virtual void unload();
		virtual bool is_valid() const;

		virtual void allocate(Format format, math::vec2u32 const& size, uint8_t const* data);
		virtual void adopt(Format format, math::vec2u32 const& size, size_t pitch, uint8_t* data);

		auto get_format() const -> Format;
		auto get_size() const->math::vec2u32 const&;

		//format dependent
		//bounds checking in debug only
		template <class Color_T>
		auto get_pixel(math::vec2u32 const& pos) const->Color_T;

		//format dependent
		//bounds checking in debug only
		template <class Color_T>
		auto set_pixel(math::vec2u32 const& pos, Color_T x)->Dynamic_Image&;

		//////////////////////////////////////////////////////////////////////////

		auto clear() -> Dynamic_Image&;
		auto clone() const -> Dynamic_Image_uptr;

		auto swap(Dynamic_Image& img) -> Dynamic_Image&;
		auto downscale(bool smooth) -> Dynamic_Image&;
		auto flip_h() -> Dynamic_Image&;
		auto flip_v() -> Dynamic_Image&;
		auto flip(bool horizontal, bool vertical) -> Dynamic_Image&;
		auto rotate_90_cw() -> Dynamic_Image&;
		auto rotate_90_ccw() -> Dynamic_Image&;
		auto convert_to_rgba8() -> Dynamic_Image&;
		auto convert_to_rgb8() -> Dynamic_Image&;
		auto convert_to_rgba4() -> Dynamic_Image&;
		auto convert_to_rgb565() -> Dynamic_Image&;
		auto convert_to_rgba5551() -> Dynamic_Image&;

		//returns true if pitch == size.x * pixelSize - that is its memory can be accessen in a linear fashion between lines
		bool is_compact() const;

		//returns a pointer to the start of line y
		auto get_line_data(uint32_t y) const -> uint8_t const*;
		auto get_mutable_line_data(uint32_t y) -> uint8_t*;

		//returns the size of a line in bytes
		auto get_line_size() const -> size_t;

		//returns a pointer to the beginning of data ONLY is isCompact is true. Otherwise returns NULL
		auto get_data() const -> uint8_t const*;
		auto get_mutable_data() -> uint8_t*;

		//returns the sizeof the data (regardless if compact or not)
		auto get_data_size() const -> size_t;

		//returns the size of a pixel in bytes
		auto get_pixel_size() const -> size_t;

		void save_as_tga(data::Sink& sink) const;

	protected:
		void deallocate();

		static void	save_as_tga(data::Sink& sink, bool has_alpha, math::vec2u32 const& size, size_t pitch, uint8_t const* data);

		Format		m_format;
		bool		m_owns_data;
		std::vector<uint8_t> m_data;
		uint8_t*	m_data_ptr;
		size_t		m_pitch;
		math::vec2u32	m_size;
	};

	namespace color
	{
		//empty getter to catch errors. Specializations follow
		template <class T> Dynamic_Image::Format get_format();
		template <> inline Dynamic_Image::Format get_format<color::A_8>() { return Dynamic_Image::Format::A_8; }
		template <> inline Dynamic_Image::Format get_format<color::RGBA_4>() { return Dynamic_Image::Format::RGBA_4; }
		template <> inline Dynamic_Image::Format get_format<color::RGB_565>() { return Dynamic_Image::Format::RGB_565; }
		template <> inline Dynamic_Image::Format get_format<color::RGBA_5551>() { return Dynamic_Image::Format::RGBA_5551; }
		template <> inline Dynamic_Image::Format get_format<color::RGB_8>() { return Dynamic_Image::Format::RGB_8; }
		template <> inline Dynamic_Image::Format get_format<color::RGBA_8>() { return Dynamic_Image::Format::RGBA_8; }
		template <> inline Dynamic_Image::Format get_format<color::RGB_32>() { return Dynamic_Image::Format::RGB_32; }
		template <> inline Dynamic_Image::Format get_format<color::RGBA_32>() { return Dynamic_Image::Format::RGBA_32; }
		template <> inline Dynamic_Image::Format get_format<color::I_8>() { return Dynamic_Image::Format::I_8; }
		template <> inline Dynamic_Image::Format get_format<color::AI_8>() { return Dynamic_Image::Format::AI_8; }
		template <> inline Dynamic_Image::Format get_format<color::I_32>() { return Dynamic_Image::Format::I_32; }
	}

	inline Dynamic_Image::Format Dynamic_Image::get_format() const
	{
		return m_format;
	}
	inline math::vec2u32 const& Dynamic_Image::get_size() const
	{
		return m_size;
	}
	template <class Color_T>
	inline auto Dynamic_Image::get_pixel(math::vec2u32 const& pos) const -> Color_T
	{
		QASSERT(pos.x < m_size.x && pos.y < m_size.y);
		auto format = color::get_format<Color_T>(); //if you get an error here, your type is not compatible with the image
		QASSERT(format == get_format());
		QASSERT(m_data_ptr);
		return ((Color_T const*)(m_data_ptr + m_pitch*pos.y))[pos.x];
	}
	template <class Color_T>
	inline auto Dynamic_Image::set_pixel(math::vec2u32 const& pos, Color_T x) -> Dynamic_Image&
	{
		QASSERT(pos.x < m_size.x && pos.y < m_size.y);
		auto format = color::get_format<Color_T>(); //if you get an error here, your type is not compatible with the image
		QASSERT(format == get_format());
		QASSERT(m_data_ptr);
		((Color_T*)(m_data_ptr + m_pitch*pos.y))[pos.x] = x;
		return *this;
	}
	inline bool Dynamic_Image::is_compact() const
	{
		return m_pitch == get_line_size();
	}

	inline uint8_t const* Dynamic_Image::get_data() const
	{
		return is_compact() ? m_data_ptr : nullptr;
	}
	inline uint8_t const* Dynamic_Image::get_line_data(uint32_t y) const
	{
		QASSERT(m_data_ptr);
		return m_data_ptr + m_pitch*y;
	}
	inline size_t Dynamic_Image::get_line_size() const
	{
		return m_size.x * get_pixel_size();
	}
	inline size_t Dynamic_Image::get_pixel_size() const
	{
		switch (m_format)
		{
		case Format::RGBA_4: return 2;
		case Format::RGB_565: return 2;
		case Format::RGBA_5551: return 2;
		case Format::RGB_8: return 3;
		case Format::RGBA_8: return 4;
		case Format::RGB_32: return 3*4;
		case Format::RGBA_32: return 4*4;
		case Format::I_8: return 1;
		case Format::A_8: return 1;
		case Format::AI_8: return 2;
		case Format::I_32: return 4;
		default: QASSERT(0); return 0;
		}
		return 0;
	}
	inline size_t Dynamic_Image::get_data_size() const
	{
		return m_size.y * get_line_size();
	}

	inline uint8_t* Dynamic_Image::get_mutable_data()
	{
		return is_compact() ? m_data_ptr : nullptr;
	}

	inline uint8_t* Dynamic_Image::get_mutable_line_data(uint32_t y)
	{
		QASSERT(m_data_ptr);
		return m_data_ptr + m_pitch * y;
	}

}
}
