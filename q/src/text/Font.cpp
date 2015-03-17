#include "QStdAfx.h"
#include "text/Font.h"
#include "text/utf8/utf8.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };
const struct 
{
	int          code;
	const char*  message;
} FT_Errors[] =
#include FT_ERRORS_H

#include "edtaa3func.h"

using namespace q;
using namespace text;

static const size_t s_image_size = 256;
static const size_t s_maxPages = 32;

Font::Font(Path const& path)
	: Resource(path)
    , m_atlas(math::vec2u32(s_image_size))
{
}

Font::~Font()
{
	free_data();
}

void Font::unload()
{
	free_data();
}

bool Font::is_valid() const
{
	return m_free_type_data.library != nullptr;
}

void Font::free_data()
{
	if (m_free_type_data.library)
	{
		FT_Done_FreeType((FT_Library)m_free_type_data.library);
		m_free_type_data.library = 0;
		m_free_type_data.face = 0;
	}
	m_data.clear();
}

bool Font::load_free_type(data::Source& source)
{
	unload();

	QASSERT(!m_free_type_data.library);

	// initialize the library
	int error = FT_Init_FreeType((FT_Library*)&m_free_type_data.library);
	if (error)
	{
		return false;
	}

	auto size = source.get_size();
	if (size == 0)
	{
		return false;
	}

	m_data.resize(size);
	auto read = source.read(m_data.data(), size);
	QASSERT(read == size);
	if (read != size)
	{
		m_data.clear();
		return false;
	}

	const FT_Byte* ptr = (const FT_Byte*)m_data.data();
	error = FT_New_Memory_Face((FT_Library)m_free_type_data.library, ptr, size, 0, (FT_Face*)&m_free_type_data.face);
	if (error != 0)
	{
		m_data.clear();
		return false;
	}

	return true;
}

float Font::get_kerning(uint32_t height, Glyph const& g0, Glyph const& g1) const
{
	uint64_t key = g0.codepoint | ((uint64_t)g1.codepoint << 32);
	auto it = m_kernings.find(key);
	if (it != m_kernings.cend())
	{
		return it->second * (float)height;
	}

	//no library?
	if (!m_free_type_data.library)
	{
		return 0;
	}

	FT_Vector kerning;
	FT_UInt kerningMode = FT_KERNING_UNSCALED;

	int32_t error = FT_Get_Kerning(
		(FT_Face)m_free_type_data.face,		// handle to face object 
		g0.glyph_id,							// left glyph index
		g1.glyph_id,							// right glyph index
		kerningMode,						// kerning mode 
		&kerning);							// target vector

	float kern = 0.f;
	if (!error)
	{
		kern = (float)kerning.x / (float)((FT_Face)m_free_type_data.face)->units_per_EM;
	}
	m_kernings[key] = kern;

	return kern * (float)height;
}


bool Font::set_height(uint32_t height) const
{
	if (m_crt_height != height)
	{
		m_crt_height = height;
		// Set char size
		int error = FT_Set_Char_Size((FT_Face)m_free_type_data.face, height << 6, 0, 72, 72);
		if (error)
		{
            QLOGE("FT_Error code 0x%02x : %s", FT_Errors[error].code, FT_Errors[error].message);
			return false;
		}	
	}
	return true;
}

void Font::reset_atlas()
{
	std::fill(m_image_data.begin(), m_image_data.end(), 0);
	m_atlas.reset();
	m_atlas_is_empty = true;
}

//static int xxx = 0;
//static int yyy = 0;
//static int zzz = 0;

bool Font::upload_page()
{
	QASSERT(!m_atlas_is_empty);

	auto texture = video::Texture::create(Path());
	bool res = texture->allocate(video::Texture::Format::A_8, math::vec2u32(s_image_size));
	if (!res)
	{
		return false;
	}

// 	xxx++;
// 	printf("\n Renders: %d, Loads: %d, Uploads: %d", yyy, zzz, xxx);

	texture->upload_data(0, &m_image_data[0]);

	Page_Data data;
	data.texture = texture;
	if (mPages.empty())
	{
		mPages.push_back(data);
	}
	else
	{
		mPages.back() = data;
	}
#if 0
	if (1)
	{
		stream::FileStream fs(String::Format("page_%s_%d.tga", GetResourceName().c_str(), mPages.size()), stream::FileStream::k_out);
		fs.Start();

		fs << (uint8_t)0; //idlength
		fs << (uint8_t)0; //colourmaptype
		fs << (uint8_t)2; //datatypecode
		fs << (uint16_t)0; //colourmaporigin
		fs << (uint16_t)0; //colourmaplength
		fs << (uint8_t)0; //colourmapdepth
		fs << (uint16_t)0; //x_origin
		fs << (uint16_t)0; //y_origin
		fs << (uint16_t)s_image_size; //width
		fs << (uint16_t)s_image_size; //height
		fs << (uint8_t)32; //bitsperpixel
		fs << (uint8_t)0; //descriptor

		for (int i = s_image_size - 1; i >= 0; i--)
		{
			for (int k = 0; k < s_image_size; k++)
			{
				uint8_t c = mImageData[i*s_image_size + k];
				uint32_t color = (0xFF << 24) | (c << 16) | (c << 8) | c;
				fs << color;
			}
		}

		fs.Finish();
	}
#endif
	return true;
}

bool Font::render_glyph(Glyph& glyph, uint32_t codepoint, bool render)
{
// 	yyy++;
// 	printf("\n Renders: %d, Loads: %d, Uploads: %d", yyy, zzz, xxx);

	FT_UInt glyphIdx = FT_Get_Char_Index((FT_Face)m_free_type_data.face, codepoint);
	FT_Int32 flags = FT_LOAD_RENDER | FT_LOAD_NO_HINTING;
	FT_Error error = FT_Load_Glyph((FT_Face)m_free_type_data.face, glyphIdx, flags);
	if (error)
	{
        QLOGE("FT_Error code 0x%02x : %s\n", FT_Errors[error].code, FT_Errors[error].message );
		return false;
	}

	FT_GlyphSlot slot = ((FT_Face)m_free_type_data.face)->glyph;
	FT_Bitmap ftBitmap= slot->bitmap;
 	int ftGlyphTop    = slot->bitmap_top;
 	int ftGlyphLeft   = slot->bitmap_left;

	// We want each glyph to be separated by at least one black pixel
	uint32_t w = slot->bitmap.width;
	uint32_t h = slot->bitmap.rows;
	uint32_t x = 0;
	uint32_t y = 0;

	QASSERT_MSG(w < s_image_size && h < s_image_size, "The font {} is being rendered at {} resulting in an image of {},{}.\nMax image size is {},{}", get_path(), m_crt_height, w, h, s_image_size, s_image_size);
	if (w >= s_image_size || h >= s_image_size)
	{
		return false;
	}

	if (render)
	{
		auto region = m_atlas.get_region(math::vec2u32(w + 2, h + 2));
		if (!region) //leave a border
		{
			if (!upload_page())
			{
				return false;
			}
			//create a new page for the next glyphs
			mPages.push_back(Page_Data());

			reset_atlas();
			region = m_atlas.get_region(math::vec2u32(w + 2, h + 2));
			if (!region)
			{
				return false;
			}
		}
		m_atlas_is_empty = false;
		x = region.get().x + 1;
		y = region.get().y + 1;
		if (w > 0 && h > 0)
		{
			for (uint32_t i = 0; i < h; i++)
			{
				memcpy(&m_image_data[(i + y)*s_image_size + x], ftBitmap.buffer + i*ftBitmap.pitch, w);
			}
		}
	}

	glyph.codepoint = codepoint;
	glyph.glyph_id = glyphIdx;
	glyph.uv_pos.set(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
	glyph.uv_size.set(static_cast<uint16_t>(w), static_cast<uint16_t>(h));
	glyph.offset.set((int16_t)ftGlyphLeft, (int16_t)-ftGlyphTop);

	glyph.size.x = (float)slot->metrics.width / 64.f;
	glyph.size.y = (float)slot->metrics.height / 64.f;
	glyph.x_advance = slot->advance.x/64.f;
	if (render)
	{
		glyph.page_idx = static_cast<int16_t>(mPages.empty() ? 0 : mPages.size() - 1);
	}
	else
	{
		glyph.page_idx = -1;
	}

// #ifndef NDEBUG
// 	{
// 		Glyph gmd;
// 		ComputeGlyphMetrics(gmd, codepoint);
// 		float xadvance = (float)mCrtHeight * gmd.xAdvance;
// 
// 		math::vec2f sizef = (float)mCrtHeight * gmd.size;
//  		math::vec2<uint16_t> size((uint16_t)math::round(sizef.x), (uint16_t)math::round(sizef.y));
// 
// 		math::vec2f offsetf = (float)mCrtHeight * gmd.offset;
// 		math::vec2<int16_t> offset((int16_t)math::round(offsetf.x), (int16_t)math::round(offsetf.y));
// 
// 		xadvance = xadvance;
// 		QASSERT(math::equals(xadvance, glyph.xAdvance, 0.01f));
// 	}
// #endif

	return true;
}

bool Font::compute_glyph_metrics(Glyph& glyph, uint32_t codepoint)
{
// 	zzz++;
// 	printf("\n Renders: %d, Loads: %d, Uploads: %d", yyy, zzz, xxx);

	FT_UInt glyphIdx = FT_Get_Char_Index((FT_Face)m_free_type_data.face, codepoint);
	FT_Int32 flags = FT_LOAD_NO_SCALE | FT_LOAD_NO_HINTING;
	FT_Error error = FT_Load_Glyph((FT_Face)m_free_type_data.face, glyphIdx, flags);
	if (error)
	{
		QASSERT_MSG("Q", "FT_Error code {:x} : {}\n", FT_Errors[error].code, FT_Errors[error].message);
		return false;
	}

	FT_GlyphSlot slot = ((FT_Face)m_free_type_data.face)->glyph;
	float unitsPerEm = (float)((FT_Face)m_free_type_data.face)->units_per_EM;

	glyph.codepoint = codepoint;
	glyph.glyph_id = glyphIdx;
	glyph.size.x = (float)slot->metrics.width / unitsPerEm;
	glyph.size.y = (float)slot->metrics.height / unitsPerEm;
	glyph.x_advance = (float)slot->advance.x / unitsPerEm;
	glyph.page_idx = -1;

	return true;
}

void Font::collect_all_glyphs_to_cache(std::vector<Glyph_Cached_Data>& dst)
{
	for (auto it: m_font_height_data)
	{
		Height_Data& hd = it.second;
		for (size_t i = 0; i < hd.glyphs.size(); i++)
		{
			Glyph_Cached_Data cache;
			cache.codepoint = hd.glyphs[i].codepoint;
			cache.height = hd.height;
			//cache.timestamp = hd.glyphTimestamps[i];
			dst.push_back(cache);
		}
	}
}

template<class T>
void Font::collect_glyphs_to_cache(std::vector<Glyph_Cached_Data>& dst, uint32_t height, T const& codepoints)
{
	//int64_t timestamp = System::GetTime();

	for (size_t i = 0; i < codepoints.size(); i++)
	{
		Glyph_Cached_Data cache;
		cache.codepoint = codepoints[i];
		cache.height = height;
		//cache.timestamp = timestamp;
		dst.push_back(cache);
	}
}

void Font::get_glyphs(std::vector<Glyph>& retGlyphs, uint32_t height, char const* utf8)
{
	retGlyphs.clear();
	if (!utf8 || utf8[0] == '\0')
	{
		return;
	}

	boost::auto_buffer<uint32_t, boost::store_n_objects<500> > codepoints;
	utf8::unchecked::utf8to32(utf8, utf8 + strlen(utf8), std::back_inserter(codepoints));
	get_glyphs(retGlyphs, height, codepoints);
}

//for a given utf8 string this returns the glyph data and the pages needed to render it.
template<class T>
void Font::get_glyphs(std::vector<Glyph>& retGlyphs, uint32_t height, T const& codepoints)
{
	retGlyphs.clear();
	if (codepoints.empty())
	{
		return;
	}

	float heightf = (float)height;

	retGlyphs.reserve(codepoints.size());

	//set a reference height
	//it should not matter since we're not really rendering the font, 
	//	but just to avoid weird behaviours in case Freetyle does use the height when computing metrics
	set_height(16);

	Glyph glyph;
	boost::auto_buffer<uint32_t, boost::store_n_objects<128> > missingCodepoints;
	for (size_t i = 0; i < codepoints.size(); i++)
	{
		uint32_t cp = codepoints[i];
		auto it = m_glyph_metrics.find(cp);
		if (it == m_glyph_metrics.cend())
		{
			if (!compute_glyph_metrics(glyph, cp))
			{
				return;
			}
			m_glyph_metrics[cp] = glyph;
		}
		else
		{
			glyph = it->second;
		}

		//convert from points to pixels
		glyph.size *= heightf;
		glyph.x_advance *= heightf;

		retGlyphs.push_back(glyph);
	}
}

void Font::get_glyphs(std::vector<Glyph>& retGlyphs, std::vector<video::Texture_ptr>& retPages, uint32_t height, char const* utf8)
{
	retGlyphs.clear();
	retPages.clear();
	if (!utf8 || utf8[0] == '\0')
	{
		return;
	}

	boost::auto_buffer<uint32_t, boost::store_n_objects<500> > codepoints;
	utf8::unchecked::utf8to32(utf8, utf8 + strlen(utf8), std::back_inserter(codepoints));
	get_glyphs(retGlyphs, retPages, height, codepoints);
}

//for a given utf8 string this returns the glyph data and the pages needed to render it.
template<class T>
void Font::get_glyphs(std::vector<Glyph>& retGlyphs, std::vector<video::Texture_ptr>& retPages, uint32_t height, T const& codepoints)
{
	retGlyphs.clear();
	retPages.clear();
	if (codepoints.empty())
	{
		return;
	}

	//first copy the pages in case we succeed
	retPages.resize(0);
	for (size_t i = 0; i < mPages.size(); i++)
	{
		retPages.push_back(mPages[i].texture);
	}
	retGlyphs.reserve(codepoints.size());

	//int64_t timestamp = System::GetTime();

	Height_Data& hd = get_height_data(height);

	//first check if all the codepoints are rendered
	boost::auto_buffer<uint32_t, boost::store_n_objects<128> > missingCodepoints;
	for (size_t i = 0; i < codepoints.size(); i++)
	{
		uint32_t cp = codepoints[i];
		std::unordered_map<uint32_t, uint32_t>::iterator it = hd.codepoint_to_glyph_idx.find(cp);
		if (it == hd.codepoint_to_glyph_idx.end())
		{
			missingCodepoints.push_back(cp);

			Glyph glyph; //the rest of the data will be filled by cache_glyphs later
			glyph.page_idx = -1;
			glyph.codepoint = cp;
			hd.codepoint_to_glyph_idx[cp] = hd.glyphs.size();
			hd.glyphs.push_back(glyph);
			//hd.glyphTimestamps.push_back(timestamp);
		}
		else
		{
			retGlyphs.push_back(hd.glyphs[it->second]);
		}
	}

	//nothing missing? good! job well done
	if (missingCodepoints.empty())
	{
		return;
	}

	//at least one glyph is not cached - clear the return arrays. We'll fill them at the end after the caching
	retPages.clear();
	retGlyphs.clear();

	//this vector holds either only the missing glyphs, or all the codepoints received as param (if we're over the page limit)
	std::vector<Glyph_Cached_Data> glyphsToCache;
	glyphsToCache.reserve(500);

	//////////////////////////////////////////////////////////////////////////

	bool res = false;

	if (mPages.size() > s_maxPages)
	{
		//we reached the max page count. Clear all pages and start from scratch
		//we'll only cache the current glyphs
		mPages.clear();
		reset_atlas();

		//clear all previous glyphs
		for (auto it: m_font_height_data)
		{
			Height_Data& hd = it.second;
			hd.glyphs.clear();
			hd.codepoint_to_glyph_idx.clear();
			//hd.glyphTimestamps.clear();
		}
		//re-add the current glyphs
		for (size_t i = 0; i < codepoints.size(); i++)
		{
			uint32_t cp = codepoints[i];
			Glyph glyph;
			glyph.page_idx = -1;
			glyph.codepoint = cp;
			hd.codepoint_to_glyph_idx[cp] = hd.glyphs.size();
			hd.glyphs.push_back(glyph);
			//hd.glyphTimestamps.push_back(timestamp);
		}
		
		collect_glyphs_to_cache(glyphsToCache, height, codepoints);

		//cache all the codepoints
		res = cache_glyphs(height, glyphsToCache);
	}
	else
	{
		collect_glyphs_to_cache(glyphsToCache, height, missingCodepoints);

		//cache only the missing codepoints
		res = cache_glyphs(height, glyphsToCache);
	}

	if (!res)
	{
		QASSERT_MSG("Q", "Cannot cache glyphs for {} codepoints.", (int)codepoints.size());
		return;
	}

	//all went ok, we have all the glyphs available
	retPages.resize(0);
	for (size_t i = 0; i < mPages.size(); i++)
	{
		retPages.push_back(mPages[i].texture);
	}
	for (size_t i = 0; i < codepoints.size(); i++)
	{
		uint32_t cp = codepoints[i];
		auto it = hd.codepoint_to_glyph_idx.find(cp);
		QASSERT(it != hd.codepoint_to_glyph_idx.end());
		uint32_t idx = it->second;
		//hd.glyphTimestamps[idx] = timestamp;
		retGlyphs.push_back(hd.glyphs[idx]);
	}
}

bool Font::cache_glyphs(uint32_t height, std::vector<Glyph_Cached_Data> const& glyphs)
{
	if (!set_height(height))
	{
		return false;
	}

	if (m_image_data.empty())
	{
		m_image_data.resize(s_image_size * s_image_size);
	}

	//Height_Data& data = get_height_data(height);

	//int64_t timestamp = System::GetTime();

	for (size_t i = 0; i < glyphs.size(); i++)
	{
		const Glyph_Cached_Data& gcd = glyphs[i];

		set_height(gcd.height);
		Height_Data& hd = get_height_data(gcd.height);

		Glyph glyph;
		if (!render_glyph(glyph, gcd.codepoint, true))
		{
			m_atlas.reset();
			return false;
		}

		//update the info in the map
		{
			std::unordered_map<uint32_t, uint32_t>::iterator it = hd.codepoint_to_glyph_idx.find(gcd.codepoint);
			QASSERT(it != hd.codepoint_to_glyph_idx.end());
			uint32_t idx = it->second;
			hd.glyphs[idx] = glyph;
			//hd.glyphTimestamps[idx] = gcd.timestamp;
		}
	}

	//last page has something in it. Update/Upload it
	if (!m_atlas_is_empty)
	{
		upload_page();
	}

	return true;
}

Font::Height_Data& Font::get_height_data(uint32_t height) const
{
	auto it = m_font_height_data.find(height);
	if (it != m_font_height_data.cend())
	{
		return it->second;
	}

	bool res = set_height(height);
	QASSERT(res);

	FT_Size_Metrics metrics = ((FT_Face)m_free_type_data.face)->size->metrics; 

	Height_Data& data = m_font_height_data[height];
	data.height = height;
	data.ascender = metrics.ascender / 64.f;
	data.descender = -metrics.descender / 64.f;  //we know it goes down from the name
	data.font_height = metrics.height / 64.f;
	data.line_gap = data.ascender + data.descender;

	{
		Glyph glyph;
		((Font*)this)->render_glyph(glyph, 'M', false);
		data.center_height = ((float)glyph.size.y) * 0.5f;
	}
	{
		Glyph glyph;
		((Font*)this)->render_glyph(glyph, ' ', false);
		data.space_width = glyph.x_advance;
	}

	return data;
}

size_t Font::get_page_count() const
{
	return mPages.size();
}
video::Texture_ptr Font::get_page(size_t idx) const
{
	return mPages[idx].texture;
}

math::vec2u32 Font::get_page_size() const
{
	return math::vec2u32(s_image_size, s_image_size);
}

float Font::get_ascender(uint32_t height) const
{
	Height_Data const& data = get_height_data(height);
	return data.ascender;
}
float Font::get_descender(uint32_t height) const
{
	Height_Data const& data = get_height_data(height);
	return data.descender;
}
float Font::get_line_gap(uint32_t height) const
{
	Height_Data const& data = get_height_data(height);
	return data.line_gap;
}
float Font::get_center_height(uint32_t height) const
{
	Height_Data const& data = get_height_data(height);
	return data.center_height;
}
float Font::get_space_width(uint32_t height) const
{
	Height_Data const& data = get_height_data(height);
	return data.space_width;
}
