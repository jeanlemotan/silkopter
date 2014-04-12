#include "QStdAfx.h"
#include "text/Font.h"
#include "text/Texter.h"
#include "draw/Painter.h"

namespace q
{
namespace text
{

#define TAB_SPACES 4.f

/////////////////////////////////////////////////////////////////////////////////////////

Texter::Texter() 
	: m_wrap_on_any_char(false)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

Texter::Texter(const Style& style)
	: m_style(style)
	, m_wrap_on_any_char(false)
{
	QASSERT(m_style.height > 0);
}

//////////////////////////////////////////////////////////////////////////
// returns true if a color is found
static bool parse_color_tag(size_t& idx, uint32_t* color, const std::vector<Font::Glyph>& glyphs)
{
	size_t count = glyphs.size();
	uint32_t c = glyphs[idx].codepoint;
	if (c == '#')
	{
		if ((idx + 1) == count)//last character
		{
			// do nothing special
		}
		else if ((idx + 1) < count && glyphs[idx + 1].codepoint == '#') // escape sequence ## -> #
		{
			// just increase idx
			idx++;
		}
		else // should be a color...
		{
			idx++;
			char buf[9];
			size_t len = 0;
			while (idx < count && len < 6)
			{
				const Font::Glyph& gl = glyphs[idx++];
				c = gl.codepoint;
				if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')))
				{
					break;
				}
				buf[len++] = static_cast<char>(c);
			}
			buf[len] = '\0';
			QASSERT_MSG(len == 6, "Specify the color as #RRGGBB, as a HTML color code.\n");
			if (color)
			{
				*color = strtol(buf, NULL, 16) | (*color & 0xFF000000);
			}
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

math::vec2f Texter::compute_size(const char* utf8) const
{
	return compute_size(utf8, 0.f);
}

/////////////////////////////////////////////////////////////////////////////////////////

math::vec2f Texter::compute_size(const char* utf8, float width) const
{
	if (!utf8 || *utf8 == '\0')
	{
		return math::vec2f(0, 0);
	}

	QASSERT(m_style.font);

	m_style.font->get_glyphs(m_glyphs, m_style.height, utf8);
	m_pages.resize(0);
	if (m_glyphs.empty())
	{
		return math::vec2f(0, 0);
	}

	math::vec2f size = compute_size(m_glyphs, width);

	m_glyphs.resize(0);

	return size;
}

//////////////////////////////////////////////////////////////////////////

math::vec2f Texter::compute_size(std::vector<Font::Glyph>& glyphs, float width) const
{
	if (width > 0)
	{
		word_wrap(glyphs, width);
	}

	if (glyphs.empty())
	{
		return math::vec2f(0, 0);
	}

	return compute_size(glyphs);
}

/////////////////////////////////////////////////////////////////////////////////////////

math::vec2f Texter::compute_size(const std::vector<Font::Glyph>& glyphs) const
{
	Params params;
	params.style = m_style;
	math::vec2f pos = execute(params, math::vec2f(0, 0), 0, glyphs);
	//return pos;
	math::vec2f size(params.rectangle.maxPoint.x - params.rectangle.minPoint.x, params.rectangle.maxPoint.y - params.rectangle.minPoint.y);
	return size;
}

/////////////////////////////////////////////////////////////////////////////////////////

float Texter::get_line_width(size_t idx, const std::vector<Font::Glyph>& glyphs) const
{
	Params params;
	params.style = m_style;
	params.only_one_line = true;
	math::vec2f pos = execute(params, math::vec2f(0, 0), idx, glyphs);
	return pos.x;
}

/////////////////////////////////////////////////////////////////////////////////////////

static uint32_t k_dontBreakBeforeThisChars[] = {'?','!',';',':','%','$','-','+'};
static uint32_t k_dontBreakBeforeThisCharsCount = sizeof(k_dontBreakBeforeThisChars)/sizeof(uint32_t);

size_t Texter::word_wrap(std::vector<Font::Glyph>& glyphs, float width) const
{
	size_t idx = 0;
	float spacing = m_style.font->get_space_width(m_style.height) * m_style.scale;

	int lastGlyphIdx = -1;

	float x = 0;

	int wordStartIdx = -1;
	int wordsOnLine = 0;
	int charsOnLine = 0;

	size_t lineCount = 0;

	while (idx < glyphs.size())
	{
		//skip color tags
		if (parse_color_tag(idx, NULL, glyphs)) 
		{ 
			continue; 
		}

		const Font::Glyph& gl = glyphs[idx];
		uint32_t nextc = (idx + 1) < glyphs.size() ? glyphs[idx + 1].codepoint : 0; //we need to know what follows the space to break or not

		uint32_t c = gl.codepoint;
		if (c == '\n')
		{
			charsOnLine = 0;
			wordsOnLine = 0;
			wordStartIdx = -1;
			x = 0;
			lastGlyphIdx = -1;
			idx++;

			lineCount++;
			continue;
		}
		else if (c == '\t')
		{
			if (wordStartIdx >= 0)
			{
				wordsOnLine++;
			}
			wordStartIdx = -1;
			x += TAB_SPACES * spacing;
			// not sure if we should do something here
		}
		else if (c == ' ' || c == '|')
		{
			uint32_t* end = k_dontBreakBeforeThisChars + k_dontBreakBeforeThisCharsCount;
			//always break on '|' but on space break ONLY if any of the special chars are not after the space
			if (c == '|' || std::find(k_dontBreakBeforeThisChars, end, nextc) == end)
			{
				if (wordStartIdx >= 0)
				{
					wordsOnLine++;
				}
				wordStartIdx = -1;
			}
		}
		else if (wordStartIdx < 0)
		{
			wordStartIdx = idx;
		}

		charsOnLine++;

		//the last can be reset when we change current idx
		if (lastGlyphIdx < 0 && idx > 0)
		{
			lastGlyphIdx = idx - 1;
		}

		//>>kerning
		if (lastGlyphIdx >= 0)
		{
			float k = m_style.font->get_kerning(m_style.height, glyphs[lastGlyphIdx], gl) * m_style.scale;
			x += k;
		}
		lastGlyphIdx = idx;
		//<<kerning
		float a = gl.x_advance * m_style.scale;
		if ((x + a) >= (int)width)
		{
			bool breakLine = false;

			if ( m_wrap_on_any_char && !( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ) )
			{
				if (charsOnLine > 1)
				{
					Font::Glyph newLine;
					newLine.codepoint = '\n';
					newLine.page_idx = -1;
					glyphs.insert(glyphs.begin() + idx, newLine);
					idx++;
					breakLine = true;
				}
			}
			else
			{
				if (wordStartIdx > 0 && wordsOnLine > 0)
				{
					Font::Glyph newLine;
					newLine.codepoint = '\n';
					newLine.page_idx = -1;
					//glyphs[wordStartIdx - 1] = newLine;
					glyphs.insert(glyphs.begin() + wordStartIdx, newLine);
					wordStartIdx++;
					idx = wordStartIdx;
					breakLine = true;
				}
				else if (wordStartIdx < 0 && wordsOnLine > 0)//break also if there is a whitespace and there are words in the line
				{
					Font::Glyph newLine;
					newLine.codepoint = '\n';
					newLine.page_idx = -1;
					// replace white space (this method should only insert newlines, rendering should remove the whitespaces depending on the alignment)
					glyphs[idx] = newLine;
					idx++;
					breakLine = true;
				}
			}

			if (breakLine)
			{
				lastGlyphIdx = -1;
				x = 0;
				wordsOnLine = 0;
				charsOnLine = 0;
				lineCount++;
				continue;
			}
		}

		x += a;
		idx++;
	}
	lineCount++;
	return lineCount;
}

size_t Texter::count_lines(std::vector<Font::Glyph>& glyphs) const
{
	size_t lineCount = 1;
	for (size_t i = 0, s = glyphs.size(); i < s; i++)
	{
		if (glyphs[i].codepoint == 10)
		{
			lineCount++;
		}
	}
	return lineCount;
}

math::vec2f Texter::draw_string(draw::Painter& painter, const char* utf8, const math::vec2f& pos, Anchors anchors) const
{
	return draw_string_worker(painter, utf8, pos, -1, anchors);
}

math::vec2f Texter::draw_string(draw::Painter& painter, const char* utf8, const math::vec2f& pos, float width, Anchors anchors) const
{
	return draw_string_worker(painter, utf8, pos, width, anchors);
}

// paging
math::vec2f Texter::draw_string_worker(draw::Painter& painter, const char* utf8, const math::vec2f& pos, float width, Anchors anchors) const
{
	if (!utf8 || *utf8 == '\0')
	{
		return pos;
	}

	if (!anchors.test(Anchor::BASELINE) && 
		!anchors.test(Anchor::TOP) && 
		!anchors.test(Anchor::VCENTER) && 
		!anchors.test(Anchor::BOTTOM))
	{
		anchors.set(Anchor::BASELINE);
	}
	if (!anchors.test(Anchor::LEFT) && 
		!anchors.test(Anchor::HCENTER) && 
		!anchors.test(Anchor::RIGHT))
	{
		anchors.set(Anchor::LEFT);
	}

	QASSERT(m_style.font);

	m_style.font->get_glyphs(m_glyphs, m_pages, m_style.height, utf8);
	if (m_glyphs.empty())
	{
		return pos;
	}

	if (width > 0.f)
	{
		word_wrap(m_glyphs, width);
	}

	// count number of lines
	size_t lineCount = count_lines(m_glyphs);
	math::vec2f position = pos;

	if (lineCount == 1)
	{
		if (anchors.test(Anchor::BOTTOM))
		{
			position.y -= m_style.font->get_descender(m_style.height) * m_style.scale;
		}
		else if (anchors.test(Anchor::VCENTER))
		{
			position.y += m_style.font->get_center_height(m_style.height) * m_style.scale;
		}
		else if (anchors.test(Anchor::TOP))
		{
			position.y += m_style.font->get_ascender(m_style.height) * m_style.scale;
		}
	}
	else
	{
		if (anchors.test(Anchor::BOTTOM))
		{
			float ch = m_style.font->get_line_gap(m_style.height) * m_style.scale;
			//position.y -= (lineCount * height);
			position.y -= m_style.font->get_descender(m_style.height) * m_style.scale;
			position.y -= ((lineCount - 1) * ch);
		}
		else if (anchors.test(Anchor::VCENTER))
		{
			float ch = m_style.font->get_line_gap(m_style.height) * m_style.scale;
			float totalHeight = (lineCount - 1) * ch;
			position.y -= totalHeight * 0.5f;
			position.y += m_style.font->get_center_height(m_style.height) * m_style.scale;

		}
		else if (anchors.test(Anchor::TOP))
		{
			position.y += m_style.font->get_ascender(m_style.height) * m_style.scale;
		}
	}

	uint32_t color = m_style.primary_color;
	if (m_style.decoration != Decoration::NONE)
	{
		for (int kx = m_style.decoration == Decoration::SHADOW ? 1 : -(int)m_style.decoration_thickness; kx <= (int)m_style.decoration_thickness; kx++)
		for (int ky = m_style.decoration == Decoration::SHADOW ? 1 : -(int)m_style.decoration_thickness; ky <= (int)m_style.decoration_thickness; ky++)
		{
			if (kx == 0 && ky == 0)
			{
				continue;
			}

			// dont use gradient color for effects
			Style newStyle = m_style;
			newStyle.color_mode = Color_Mode::NORMAL;

			math::vec4f c0 = math::color::u32_to_rgba<float>(color);
			math::vec4f dc = math::color::u32_to_rgba<float>(newStyle.decoration_color);
			math::vec4f decorationColorMixedAlpha = math::vec4f(dc.x, dc.y, dc.z, dc.w * c0.w);
			newStyle.decoration_color = math::color::rgba_to_u32(decorationColorMixedAlpha);

			Params params;
			params.line_count = lineCount;
			params.painter = &painter;
			params.style = newStyle;
			params.anchors = anchors;
			params.color = color;
			execute(params, position + math::vec2f((float)kx, (float)ky), 0, m_glyphs);
		}
	}
	Style style = m_style;
	style.decoration = Decoration::NONE;

	Params params;
	params.line_count = lineCount;
	params.painter = &painter;
	params.style = style;
	params.anchors = anchors;
	params.color = color;
	math::vec2f currentPos = execute(params, position, 0, m_glyphs);
	m_pages.resize(0);
	m_glyphs.resize(0);
	return currentPos;
}

//////////////////////////////////////////////////////////////////////////

math::vec2f Texter::execute(Params& params, const math::vec2f& pos, size_t idx, const std::vector<Font::Glyph>& glyphs) const
{
	if (!params.style.font || glyphs.empty())
	{
		return pos;
	}

	float ch = params.style.font->get_line_gap(m_style.height) * m_style.scale;
	float spacing = params.style.font->get_space_width(m_style.height) * m_style.scale;
	float x = pos.x;
	float y = pos.y;

	//>> handle horizontal alignment at first line
	if (params.anchors.test(Anchor::RIGHT) || params.anchors.test(Anchor::HCENTER))
	{
		float strWidth = get_line_width(0, glyphs);
		if (params.anchors.test(Anchor::RIGHT))
		{
			x -= strWidth;
		}
		else if (params.anchors.test(Anchor::HCENTER))
		{
			x -= strWidth * 0.5f;
		}
	}
	//<< handle horizontal alignment at first line

	params.rectangle.minPoint.set(x, y);
	params.rectangle.maxPoint.set(x, y);

	//>> strikeout, underline support
	Line_Marker marker;
	std::vector<Line_Marker> lineMarkers;
	bool computeLineMarker = (params.style.strikeout_thickness > 0) || (params.style.underline_thickness > 0);
	//<< strikeout, underline support

	video::Texture_ptr page;
	const Font::Glyph* lastGlyph = 0;
	int lastPageIdx = -1;
	math::vec3f vtxPos;
	math::vec2f vtxUv;
	draw::Vertex v[4];

	video::Material material;
	if (params.painter)
	{
		material = params.painter->get_material();
	}

	size_t count = glyphs.size();
	while (idx < count)
	{
		if (parse_color_tag(idx, &params.color, glyphs)) 
		{ 
			continue; 
		}

		if (params.style.decoration != Decoration::NONE)
		{
			params.color = params.style.decoration_color;
		}

		const Font::Glyph& gl = glyphs[idx++];
		uint32_t c = gl.codepoint;
		if (c == '|')
		{
			continue;
		}
		if (c == '\n')
		{
			if (params.only_one_line)
			{
				return math::vec2f(x, y);
			}

			y += ch;
			x = pos.x;
			
			//>> handle alignment after newline
			if (params.anchors.test(Anchor::RIGHT) || params.anchors.test(Anchor::HCENTER))
			{
				float strWidth = get_line_width(idx, glyphs);
				if (params.anchors.test(Anchor::RIGHT))
				{
					x -= strWidth;
				}
				else if (params.anchors.test(Anchor::HCENTER))
				{
					x -= strWidth * 0.5f;
				}
			//<< handle alignment after newline
			}
			continue;
		}
		else if (c == '\t')
		{
			x += TAB_SPACES * spacing;
			continue;
		}

		//>>kerning
		float k = lastGlyph ? params.style.font->get_kerning(m_style.height, *lastGlyph, gl) * m_style.scale : 0.f;
		x += k;
		lastGlyph = &gl;
		//<<kerning

		if (params.painter)
		{
			//>> page support
			if (gl.page_idx >= 0)
			{
				if (gl.page_idx != lastPageIdx)
				{
					page = m_pages[gl.page_idx];
					if (material.get_sampler_count(0) > 0)
					{
						auto sampler = material.get_sampler(0, 0);
						sampler.set_texture(page);
						material.set_sampler(0, 0, sampler);
						params.painter->set_material(material);
					}
					lastPageIdx = gl.page_idx;
				}
			}
			//<<page support

			if (!page)
			{
				continue;
			}

			//>> strikeout, underline support
			if (computeLineMarker)
			{
				marker.pos.set(x, y); 
				marker.color = params.color;
				lineMarkers.push_back(marker);
			}
			//<< strikeout, underline support

			float tw = 1.f / (float)(page->get_size().x);
			float th = 1.f / (float)(page->get_size().y);

			float w = (float)gl.uv_size.x * params.style.scale;
			float h = (float)gl.uv_size.y * params.style.scale;
			float ox = gl.offset.x * params.style.scale;
			float oy = gl.offset.y * params.style.scale;

			float u0 = gl.uv_pos.x*tw;
			float v0 = gl.uv_pos.y*th;
			float u1 = (gl.uv_pos.x + gl.uv_size.x)*tw;
			float v1 = (gl.uv_pos.y + gl.uv_size.y)*th;

			//In some languages there are glyphs of 1 pixel height (like '-')
			//When scaled down these disappear. 
			//We force the min height to 2 pixels and it's fixed
			if (h == 1.f)
			{
				h = 2.f;
			}

			v[0].set_position(math::vec2f(x+ox, y+h+oy));
			v[0].set_tex_coord(math::vec2f(u0, v1));

			v[1].set_position(math::vec2f(x+ox, y+oy));
			v[1].set_tex_coord(math::vec2f(u0, v0));

			v[2].set_position(math::vec2f(x+w+ox, y+h+oy));
			v[2].set_tex_coord(math::vec2f(u1, v1));

			v[3].set_position(math::vec2f(x+w+ox, y+oy));
			v[3].set_tex_coord(math::vec2f(u1, v0));

			if (params.style.color_mode == Color_Mode::GRADIENT_TB)
			{
				v[0].set_color(params.color);
				v[1].set_color(params.style.gradient_color);
				v[2].set_color(params.color);
				v[3].set_color(params.style.gradient_color);
			}
			else if (params.style.color_mode == Color_Mode::GRADIENT_BT)
			{
				v[0].set_color(params.style.gradient_color);
				v[1].set_color(params.color);
				v[2].set_color(params.style.gradient_color);
				v[3].set_color(params.color);
			}
			else if (params.style.color_mode == Color_Mode::GRADIENT_LR)
			{
				v[0].set_color(params.color);
				v[1].set_color(params.color);
				v[2].set_color(params.style.gradient_color);
				v[3].set_color(params.style.gradient_color);
			}
			else if (params.style.color_mode == Color_Mode::GRADIENT_RL)
			{
				v[0].set_color(params.style.gradient_color);
				v[1].set_color(params.style.gradient_color);
				v[2].set_color(params.color);
				v[3].set_color(params.color);
			}
			else
			{
				v[0].set_color(params.color);
				v[1].set_color(params.color);
				v[2].set_color(params.color);
				v[3].set_color(params.color);
			}

			params.painter->fill_quad(v[1], v[3], v[2], v[0]);

			//>> strikeout, underline support
			if (computeLineMarker)
			{
				marker.pos.set(x, y); 
				marker.color = params.color;
				lineMarkers.push_back(marker);
			}
			//<< strikeout, underline support
		}
		float a = gl.x_advance * params.style.scale;
		x += a;

		params.rectangle.add_internal_point(math::vec2f(x, y));
	}//while (idx<left)

	//>> strikeout, underline support
/*
	if (params.painter && computeLineMarker)
	{
		// use a material without alpha blending
		//jet::video::Material oldMtl = m_painter->GetMaterial();
		//jet::video::Material newMtl = oldMtl;
		material.GetRenderState(0).SetBlending(false);
		params.painter->SetMaterial(material);

		int st  = params.style.strikeoutThickness;
		int yso = ch/2 + st/2;

		int ut  = params.style.underlineThickness;
		int yuo = ch;

		size_t i = 0;
		while (i < lineMarkers.size())
		{
			LineMarker& a = lineMarkers[i];
			size_t j = i + 1;
			while (j < lineMarkers.size())
			{
				LineMarker& b = lineMarkers[j];
				if (!(b.y == a.y && b.color && a.color))
				{
					break;
				}
				j++;
			}
			LineMarker& b = lineMarkers[j-1];
			i = j;
			uint32_t color = a.color;

			if (st > 0)
			{
// 				POS(a.x,a.y + yso); VTX(v[0]);
// 				POS(b.x,a.y + yso); VTX(v[1]);
// 				POS(b.x,a.y + yso + st); VTX(v[2]);
// 				POS(a.x,a.y + yso + st); VTX(v[3]);
// 				m_painter->FillQuad(v[0],v[1],v[2],v[3]);
			}
			if (ut > 0)
			{
// 				POS(a.x,a.y + yuo); VTX(v[0]);
// 				POS(b.x,a.y + yuo); VTX(v[1]);
// 				POS(b.x,a.y + yuo + ut); VTX(v[2]);
// 				POS(a.x,a.y + yuo + ut); VTX(v[3]);
// 				m_painter->FillQuad(v[0],v[1],v[2],v[3]);
			}

		}
		//m_painter->SetMaterial(oldMtl);
	}
	//<< strikeout, underline support
*/

	params.rectangle.add_internal_point(math::vec2f(x, y + ch));
	return math::vec2f(x, y + ch);
}

const Style& Texter::get_style() const
{
	return m_style;
}

void Texter::set_style(const Style& style)
{
	m_style = style;
}

//////////////////////////////////////////////////////////////////////////

void Texter::swap_on_any_char(bool yes)
{
	m_wrap_on_any_char = yes;
}

}//namespace text

}//namespace q

