#pragma once

#include "res/Resource.h"

namespace q
{
namespace res
{
namespace impl
{
	class Free_Type_Font_Loader;
}
}

namespace text
{

	//This class represents a fontface.
	class Font : public res::Resource
	{
		DEFINE_RTTI_CLASS(q::text::Font, res::Resource);
		friend class q::res::impl::Free_Type_Font_Loader;

	public:
		Font(Path const& path);
		virtual ~Font();

		struct Glyph
		{
			// the unicode code point
			uint32_t codepoint;
			uint32_t glyph_id;
			float x_advance;		 
			math::vec2f size;

			//render data - calculated only when rasterizing
			math::vec2<uint16_t> uv_pos; //uv position in the page
			math::vec2<uint16_t> uv_size; //uv size in pixels
			math::vec2<int16_t> offset;
			int16_t page_idx;
		};

		math::vec2u32 get_page_size() const;
		size_t get_page_count() const;
		video::Texture_ptr get_page(size_t idx) const;

		float get_space_width(uint32_t height) const;
		float get_center_height(uint32_t height) const;
		float get_ascender(uint32_t height) const;
		float get_descender(uint32_t height) const;
		float get_line_gap(uint32_t height) const;

		//for a given utf8 string this returns the glyph data.
		void get_glyphs(std::vector<Glyph>& glyphs, uint32_t height, char const* utf8);
		template<class T> void get_glyphs(std::vector<Glyph>& glyphs, uint32_t height, T const& codepoints);

		//for a given utf8 string this returns the glyph data and the pages needed to render it.
		void get_glyphs(std::vector<Glyph>& glyphs, std::vector<video::Texture_ptr>& pages, uint32_t height, char const* utf8);
		template<class T> void get_glyphs(std::vector<Glyph>& glyphs, std::vector<video::Texture_ptr>& pages, uint32_t height, T const& codepoints);

		//returns the kerning between codepoint0 and codepoint1 in font units
		float get_kerning(uint32_t height, Glyph const& g0, Glyph const& g1) const;

		virtual void		unload();
		virtual bool		is_valid() const;

	protected:
		bool load_free_type(data::Source& source);

	private:

		//glyph temp cached data
		struct Glyph_Cached_Data
		{
			uint32_t codepoint;
			uint32_t height;
			//int64_t timestamp;
		};

// 		static bool SortGlyphCachedData(const GlyphCachedData& a, const GlyphCachedData& b)
// 		{
// 			return a.timestamp > b.timestamp;
// 		}

		bool cache_glyphs(uint32_t height, std::vector<Glyph_Cached_Data> const& glyphs);

		//returns cache data for all glyphs in the font (all heights as well)
		void collect_all_glyphs_to_cache(std::vector<Glyph_Cached_Data>& dst);
		//returns cache data for the specified codepoints, in the specified height
		template<class T> void collect_glyphs_to_cache(std::vector<Glyph_Cached_Data>& dst, uint32_t height, T const& codepoints);

		void free_data();

		struct Free_Type_Data
		{
			Free_Type_Data() : library(0), face(0) {}
			void* library;//FT_Library
			void* face;//FT_Face
		} m_free_type_data;

		std::vector<uint8_t> m_data;

		//kerning map in font units
		mutable std::unordered_map<uint64_t, float> m_kernings;

		std::unordered_map<uint32_t, Glyph> m_glyph_metrics;

		struct Height_Data
		{
			Height_Data() : height(0), ascender(0), descender(0), font_height(0), line_gap(0) {}

			uint32_t height; //the height that this data caches

			float ascender;
			float descender;
			float font_height; //returned by freetype
			float line_gap;
			float center_height;
			float space_width;

			std::unordered_map<uint32_t, uint32_t> codepoint_to_glyph_idx;
			std::vector<Glyph> glyphs;
			//std::vector<int64_t> glyphTimestamps;
		};

		mutable uint32_t m_crt_height;
		bool set_height(uint32_t height) const;
		Height_Data& get_height_data(uint32_t height) const;

		mutable std::unordered_map<uint32_t, Height_Data> m_font_height_data;

		struct Page_Data
		{
			video::Texture_ptr texture;
		};

		std::vector<Page_Data> mPages;

		void reset_atlas();
		bool upload_page();
		bool render_glyph(Glyph& glyph, uint32_t codepoint, bool render);
		bool compute_glyph_metrics(Glyph& glyph, uint32_t codepoint);

		bool		m_atlas_is_empty;
		video::Atlaser m_atlas;
		std::vector<uint8_t> m_image_data;
	};

}//text

}//q

