#pragma once

#include "draw/Painter.h"

namespace q
{

namespace text
{

	class Font;

	enum class Anchor : uint8_t
	{
		BASELINE	= 1 << 0,
		TOP			= 1 << 1,
		LEFT		= 1 << 2,
		HCENTER		= 1 << 3,
		VCENTER		= 1 << 4,
		RIGHT		= 1 << 5,
		BOTTOM		= 1 << 6,
	};
	typedef util::Flag_Set<Anchor, uint8_t> Anchors;

	enum class Decoration
	{
		NONE = 0,
		SHADOW,
		OUTLINE,
	};

	enum class Color_Mode
	{
		NORMAL = 0,
		// (all gradients goes form gradientColor to character color)
		GRADIENT_LR,//from left to right
		GRADIENT_RL,//from right to left
		GRADIENT_TB,//from top to bottom
		GRADIENT_BT,//from bottom to top
	};

	struct Style
	{
		Style()
			: height(10)
			, scale(1.0f)
			, decoration(Decoration::NONE)
			, primary_color(0xFFFFFFFF)
			, decoration_color(0)
			, decoration_thickness(1)
			, strikeout_thickness(0)
			, underline_thickness(0)
			, color_mode(Color_Mode::NORMAL)
			, gradient_color(0xFFFFFFFF)
		{}

		Font_ptr font;
		uint32_t height;
		float scale;
		Decoration decoration;
		uint32_t primary_color; //base font color
		uint32_t decoration_color; //shadow or outline color
		size_t decoration_thickness; //shadow or outline
		size_t strikeout_thickness;
		size_t underline_thickness;
		Color_Mode color_mode;
		uint32_t gradient_color;

		bool operator==(const Style& r) const
		{
			return
				font == r.font &&
				height == r.height &&
				decoration == r.decoration &&
				primary_color == r.primary_color &&
				decoration_color == r.decoration_color &&
				decoration_thickness == r.decoration_thickness &&
				strikeout_thickness == r.strikeout_thickness &&
				underline_thickness == r.underline_thickness &&
				color_mode == r.color_mode &&
				gradient_color == r.gradient_color;
		}
		bool operator!=(const Style& r) const
		{
			return !operator==(r);
		}
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Texter base class declaration
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This class is used to draw text to screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// special notes:
// - global text color can be specified through the painter
// - text color can be specified  by inserting color tags in the utf8 stream: '#ff0000'
// - '#' character must be specified as ##
// - separator hints for hyphenation can be embedded in the utf8 stream with '|' (TODO)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Texter
{
public:

	Texter();
	explicit Texter(const Style& style);

public:
	//some language require to paginate on any char, not only between words
	void		swap_on_any_char(bool yes);

	//Draws the utf8 string at the specified position.
	//Returns the 'advanced' cursor position
	template<class String_T>
	math::vec2f draw_string(draw::Painter& painter, const String_T& utf8, const math::vec2f& pos, Anchors anchors = Anchors(Anchor::LEFT, Anchor::BASELINE)) const;
	math::vec2f draw_string(draw::Painter& painter, const char* utf8, const math::vec2f& pos, Anchors anchors = Anchors(Anchor::LEFT, Anchor::BASELINE)) const;

	//Draws the utf8 string at the specified position.
	//The string is paginated according to width
	//Returns the 'advanced' cursor position
	template<class String_T>
	math::vec2f draw_string(draw::Painter& painter, const String_T& utf8, const math::vec2f& pos, float width, Anchors anchors = Anchors(Anchor::LEFT, Anchor::BASELINE)) const;
	math::vec2f draw_string(draw::Painter& painter, const char* utf8, const math::vec2f& pos, float width, Anchors anchors = Anchors(Anchor::LEFT, Anchor::BASELINE)) const;

	//returns the width and height in pixels of the text
	math::vec2f compute_size(const char* utf8) const;

	//returns the width and height in pixels of the text
	//can do pagination
	math::vec2f compute_size(const char* utf8, float width) const;

	const Style& get_style() const;
	void set_style(const Style& style);

protected:

	math::vec2f draw_string_worker(draw::Painter& painter, const char* utf8, const math::vec2f& pos, float width, Anchors anchors) const;

	//returns the width and height in pixels of the text until the given character position
	//can do pagination
	math::vec2f compute_size(std::vector<Font::Glyph>& glyphs, float width) const;

	//returns the width and height in pixels of the text
	math::vec2f compute_size(const std::vector<Font::Glyph>& glyphs) const;

	// returns the width in pixels of the given text starting at specified offset
	float get_line_width(size_t idx, const std::vector<Font::Glyph>& glyphs) const;

	struct Params
	{
		Params()
			: painter(0), only_one_line(false), color(0), anchors(Anchor::TOP, Anchor::LEFT), line_count(1) {}

		draw::Painter* painter;
		bool only_one_line;
		uint32_t color;
		Style style;
		Anchors anchors;
		size_t line_count;//precomputed line count
		math::aabb2f rectangle;//text rectangle size
	};

	// draws a subset of characters using a custom style
	math::vec2f execute(
		Params& params,
		const math::vec2f& pos,
		size_t idx,
		const std::vector<Font::Glyph>& glyphs) const;

protected:
	// returns total number of lines after wrapping
	size_t word_wrap(std::vector<Font::Glyph>& glyphs, float width) const;
	size_t count_lines(std::vector<Font::Glyph>& glyphs) const;

protected:
	Style m_style;

	struct Line_Marker
	{
		math::vec2f pos;
		uint32_t color;
	};

	bool m_wrap_on_any_char;

	mutable std::vector<Font::Glyph> m_glyphs;
	mutable std::vector<video::Texture_ptr> m_pages;
};

template<class String_T>
math::vec2f Texter::draw_string(draw::Painter& painter, const String_T& utf8, const math::vec2f& pos, Anchors anchors) const
{
	return draw_string_worker(painter, utf8.c_str(), pos, -1, anchors);
}

template<class String_T>
math::vec2f Texter::draw_string(draw::Painter& painter, const String_T& utf8, const math::vec2f& pos, float width, Anchors anchors) const
{
	return draw_string_worker(painter, utf8.c_str(), pos, width, anchors);
}


}//text

}//q

