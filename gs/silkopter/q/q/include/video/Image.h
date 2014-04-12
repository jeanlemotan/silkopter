#pragma once

namespace q
{
namespace video
{
	template<class Color_T>
	class Image : public res::Resource
	{
	public:
		typedef Color_T Color_Type;
		typedef color::Traits<Color_T> Color_Traits;

		Image(Path const& path = Path());
		~Image();

		void unload();
		bool is_valid() const;

		void allocate(math::vec2u32 const& size, Color_T const* data);
		//void adopt(math::vec2u32 const& size, size_t pitch, Color_T* data);

		auto get_size() const->math::vec2u32 const&;

		//format dependent
		//bounds checking in debug only
		auto get_pixel(math::vec2u32 const& pos) const->Color_T;

		//format dependent
		//bounds checking in debug only
		auto set_pixel(math::vec2u32 const& pos, Color_T x)->Image<Color_T>&;

		//////////////////////////////////////////////////////////////////////////

		auto clear(Color_T x = Color_T()) -> Image<Color_T>&;
		//auto clone() const -> Image_uptr;

		auto swap(Image<Color_T>& img) -> Image<Color_T>&;
		auto downscale(bool smooth) -> Image<Color_T>&;
		auto flip_h() -> Image<Color_T>&;
		auto flip_v() -> Image<Color_T>&;
		auto flip(bool horizontal, bool vertical) -> Image<Color_T>&;
		auto rotate_90_cw() -> Image<Color_T>&;
		auto rotate_90_ccw() -> Image<Color_T>&;
		auto resize(math::vec2u32 const& new_size, bool smooth)->Image<Color_T>&;

		template<class New_Color_T>
		auto convert_to() const -> Image<New_Color_T>;

		//returns true if pitch == size.x * pixelSize - that is its memory can be accessen in a linear fashion between lines
		bool is_compact() const;

		//returns a pointer to the start of line y
		auto get_line_data(uint32_t y) const -> Color_T const*;
		auto get_mutable_line_data(uint32_t y) -> Color_T*;

		//returns a pointer to the beginning of data ONLY if isCompact is true. Otherwise returns nullptr
		auto get_data() const -> Color_T const*;
		auto get_mutable_data() -> Color_T*;

		void save_as_tga(data::Sink& sink) const;

		bool is_hdr() const;
		bool has_alpha_channel() const;
		bool has_color_channel() const;
		bool has_grayscale_channel() const;
		size_t get_channel_count() const;

	protected:
		void save_as_tga(data::Sink& sink, std::true_type has_alpha) const;
		void save_as_tga(data::Sink& sink, std::false_type has_alpha) const;

		void deallocate();

		std::shared_ptr<std::vector<Color_T>> m_data;
		size_t		m_pitch;
		math::vec2u32	m_size;
	};

	template<class Color_T>
	Image<Color_T>::Image(Path const& path)
		: res::Resource(path)
		, m_pitch(0)
	{

	}
	template<class Color_T>
	Image<Color_T>::~Image()
	{

	}

	template<class Color_T>
	void Image<Color_T>::unload()
	{
		deallocate();
	}
	template<class Color_T>
	bool Image<Color_T>::is_valid() const
	{
		return m_data != nullptr;
	}

	template<class Color_T>
	void Image<Color_T>::allocate(math::vec2u32 const& size, Color_T const* data)
	{
		deallocate();

		QASSERT(size.x > 0 && size.y > 0);
		m_size = math::max(size, math::vec2u32(1, 1));

		size_t data_size = m_size.x * m_size.y;

		m_data.reset(new std::vector<Color_T>());
		m_data->resize(data_size);
		m_pitch = m_size.x;
		if (data)
		{
			std::copy(data, data + data_size, m_data->data());
		}
	}
// 	template<class Color_T>
// 	void Image<Color_T>::adopt(math::vec2u32 const& size, size_t pitch, Color_T* data)
// 	{
// 		QASSERT(data);
// 		QASSERT(pitch > 0 && size.x > 0 && size.y > 0);
// 		deallocate();
//
// 		m_size = math::max(size, math::vec2u32(1, 1));
//
// 		m_data->data() = data;
// 		m_owns_data = false;
// 		m_pitch = pitch;
// 	}

	template<class Color_T>
	auto Image<Color_T>::get_size() const -> math::vec2u32 const&
	{
		return m_size;
	}

	template<class Color_T>
	auto Image<Color_T>::get_pixel(math::vec2u32 const& pos) const			-> Color_T
	{
		QASSERT(pos.x < m_size.x && pos.y < m_size.y);
		QASSERT(m_data);
		return (m_data->data() + m_pitch*pos.y)[pos.x];
	}

	template<class Color_T>
	auto Image<Color_T>::set_pixel(math::vec2u32 const& pos, Color_T x)		-> Image<Color_T>&
	{
		QASSERT(pos.x < m_size.x && pos.y < m_size.y);
		QASSERT(m_data);
		(m_data->data() + m_pitch*pos.y)[pos.x] = x;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////

	template<class Color_T>
	auto Image<Color_T>::clear(Color_T x) -> Image<Color_T>&
	{
		if (is_compact())
		{
			std::fill(m_data->begin(), m_data->end(), x);
		}
		else
		{
			size_t line_size = m_size.x;
			auto* data = m_data->data();
			for (uint32_t i = 0; i < m_size.y; i++)
			{
				std::fill(data, data + line_size, x);
				data += m_pitch;
			}
		}
		return *this;
	}
// 	template<class Color_T>
// 	auto Image<Color_T>::clone() const -> Image_uptr
// 	{
//
// 	}

	template<class Color_T>
	auto Image<Color_T>::swap(Image<Color_T>& img) -> Image<Color_T>&
	{
		std::swap(m_data, img.m_data);
		std::swap(m_pitch, img.m_pitch);
		std::swap(m_size, img.m_size);
		return *this;
	}
	template<class Color_T>
	auto Image<Color_T>::downscale(bool smooth) -> Image<Color_T>&
	{
		if (m_size == math::vec2u32(1, 1) || !is_compact())
		{
			return *this;
		}

		auto new_size = math::max(m_size / 2, math::vec2u32(1, 1));

		Image<Color_T> img;
		img.allocate(new_size, nullptr);

		if (smooth)
		{
			auto* dst = img.get_mutable_data();
			auto ratio = color::Ratio<Color_T>(0.5f);
			for (uint32_t j = 0; j < img.get_size().y; j++)
			{
				auto* src1 = get_line_data(j * 2);
				auto* src2 = get_line_data(j * 2 + 1);
				for (uint32_t i = 0, szi = img.get_size().x; i < szi; i++)
				{
					auto c1 = color::lerp(*src1, *(src1 + 1), ratio);
					src1 += 2;
					auto c2 = color::lerp(*src2, *(src2 + 1), ratio);
					src2 += 2;
					*dst++ = color::lerp(c1, c2, ratio);
				}
			}
		}
		else
		{
			auto* dst = img.get_mutable_data();
			for (uint32_t j = 0; j < img.get_size().y; j++)
			{
				auto* src = get_line_data(j * 2);
				for (uint32_t i = 0, szi = img.get_size().x; i < szi; i++)
				{
					*dst++ = *src;
					src += 2;
				}
			}
		}
		swap(img);
		return *this;
	}
	template<class Color_T>
	auto Image<Color_T>::flip_h() -> Image<Color_T>&
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
		return *this;
	}
	template<class Color_T>
	auto Image<Color_T>::flip_v() -> Image<Color_T>&
	{
		boost::auto_buffer<Color_T, boost::store_n_bytes<8192>> buffer;

		size_t line_size = m_size.x;
		buffer.uninitialized_resize(line_size);

		auto* a = get_mutable_line_data(0);
		auto* b = get_mutable_line_data(m_size.y - 1);
		for (uint32_t i = 0, sz = m_size.y / 2; i < sz; i++)
		{
			std::copy(a, a + line_size, buffer.data());
			std::copy(b, b + line_size, a);
			std::copy(buffer.data(), buffer.data() + line_size, b);
			a += m_pitch;
			b -= m_pitch;
		}
		return *this;
	}
	template<class Color_T>
	auto Image<Color_T>::flip(bool horizontal, bool vertical) -> Image<Color_T>&
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
	template<class Color_T>
	auto Image<Color_T>::rotate_90_ccw() -> Image<Color_T>&
	{
		Image<Color_T> img;
		img.allocate(math::vec2u32(m_size.y, m_size.x), nullptr);

		for (uint32_t j = 0; j < m_size.y; j++)
		{
			auto* a = get_line_data(j);
			for (uint32_t i = 0, szi = m_size.x; i < szi; i++)
			{
				img.set_pixel(math::vec2u32(j, i), *a++);
			}
		}
		this->swap(img);
		return *this;
	}
	template<class Color_T>
	auto Image<Color_T>::rotate_90_cw() -> Image<Color_T>&
	{
		Image<Color_T> img;
		img.allocate(math::vec2u32(m_size.y, m_size.x), nullptr);

		for (uint32_t j = 0; j < m_size.y; j++)
		{
			auto* a = get_line_data(j);
			for (uint32_t i = 0, szi = m_size.x; i < szi; i++)
			{
				img.set_pixel(math::vec2u32(m_size.y - j - 1, i), *a++);
			}
		}
		this->swap(img);
		return *this;
	}

	template<class Color_T>
	auto Image<Color_T>::resize(math::vec2u32 const& new_size, bool smooth) -> Image<Color_T>&
	{
		return *this;
	}

 	template<class Color_T>
 	template<class New_Color_T>
 	auto Image<Color_T>::convert_to() const -> Image<New_Color_T>
	{
		Image<New_Color_T> img;
		img.allocate(m_size, nullptr);

		auto* dst = img.get_mutable_data();
		for (uint32_t j = 0; j < m_size.y; j++)
		{
			auto* src = get_line_data(j);
			for (uint32_t i = 0, szi = m_size.x; i < szi; i++)
			{
				*dst++ = color::convert_to<New_Color_T>(*src++);
			}
		}
		return img;
	}

	template<class Color_T>
	bool Image<Color_T>::is_compact() const
	{
		return m_pitch == m_size.x;
	}

	//returns a pointer to the start of line y
	template<class Color_T>
	auto Image<Color_T>::get_line_data(uint32_t y) const -> Color_T const*
	{
		QASSERT(m_data);
		return m_data->data() + m_pitch*y;
	}
	template<class Color_T>
	auto Image<Color_T>::get_mutable_line_data(uint32_t y) -> Color_T*
	{
		QASSERT(m_data);
		return m_data->data() + m_pitch*y;
	}

	template<class Color_T>
	auto Image<Color_T>::get_data() const -> Color_T const*
	{
		return m_data->data();
	}
	template<class Color_T>
	auto Image<Color_T>::get_mutable_data() -> Color_T*
	{
		return m_data->data();
	}

	template<class Color_T>
	void Image<Color_T>::deallocate()
	{
		m_data.reset();
	}

	template<class Color_T>
	bool Image<Color_T>::is_hdr() const
	{
		return Color_Traits::is_hdr::value;
	}
	template<class Color_T>
	bool Image<Color_T>::has_alpha_channel() const
	{
		return Color_Traits::has_alpha_channel::value;
	}
	template<class Color_T>
	bool Image<Color_T>::has_color_channel() const
	{
		return Color_Traits::has_color_channel::value;
	}
	template<class Color_T>
	bool Image<Color_T>::has_grayscale_channel() const
	{
		return Color_Traits::has_grayscale_channel::value;
	}
	template<class Color_T>
	size_t Image<Color_T>::get_channel_count() const
	{
		return Color_Traits::channel_count::value;
	}

	template<class Color_T>
	void Image<Color_T>::save_as_tga(data::Sink& sink) const
	{
		save_as_tga(sink, typename Color_Traits::has_alpha_channel());
	}

	template<class Color_T>
	void Image<Color_T>::save_as_tga(data::Sink& sink, std::true_type) const
	{
		auto tmp = convert_to<color::RGBA_8>();
		tmp.save_as_tga(sink);
	}
	template<class Color_T>
	void Image<Color_T>::save_as_tga(data::Sink& sink, std::false_type) const
	{
		auto tmp = convert_to<color::RGB_8>();
		tmp.save_as_tga(sink);
	}

	template<>
	inline void Image<color::RGBA_8>::save_as_tga(data::Sink& sink) const
	{
		sink << uint8_t(0); //idlength
		sink << uint8_t(0); //colourmaptype
		sink << uint8_t(2); //datatypecode
		sink << uint16_t(0); //colourmaporigin
		sink << uint16_t(0); //colourmaplength
		sink << uint8_t(0); //colourmapdepth
		sink << uint16_t(0); //x_origin
		sink << uint16_t(0); //y_origin
		sink << uint16_t(m_size.x); //width
		sink << uint16_t(m_size.y); //height
		sink << uint8_t(32); //bitsperpixel
		sink << uint8_t(0); //descriptor

		size_t line_size = m_size.x * 4;
		boost::auto_buffer<uint8_t, boost::store_n_bytes<8192>> temp;
		temp.uninitialized_resize(line_size);

		for (int i = m_size.y - 1; i >= 0; i--)
		{
			uint32_t const* src = (uint32_t const*)(m_data->data() + m_pitch * i);
			uint32_t* dst = (uint32_t*)temp.data();
			for (uint32_t x = 0; x < m_size.x; x++)
			{
				uint32_t c = *src++;
				c = math::color::swap_rb(c);
				*dst++ = c;
			}
			sink.write(temp.data(), line_size);
		}
	}
	template<>
	inline void Image<color::RGB_8>::save_as_tga(data::Sink& sink) const
	{
		sink << uint8_t(0); //idlength
		sink << uint8_t(0); //colourmaptype
		sink << uint8_t(2); //datatypecode
		sink << uint16_t(0); //colourmaporigin
		sink << uint16_t(0); //colourmaplength
		sink << uint8_t(0); //colourmapdepth
		sink << uint16_t(0); //x_origin
		sink << uint16_t(0); //y_origin
		sink << uint16_t(m_size.x); //width
		sink << uint16_t(m_size.y); //height
		sink << uint8_t(24); //bitsperpixel
		sink << uint8_t(0); //descriptor

		size_t line_size = m_size.x * 3;
		boost::auto_buffer<uint8_t, boost::store_n_bytes<8192>> temp;
		temp.uninitialized_resize(line_size);

		for (int i = m_size.y - 1; i >= 0; i--)
		{
			uint8_t const* src = reinterpret_cast<uint8_t const*>(m_data->data() + m_pitch * i);
			uint8_t* dst = temp.data();
			for (uint32_t x = 0; x < m_size.x; x++)
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
}
