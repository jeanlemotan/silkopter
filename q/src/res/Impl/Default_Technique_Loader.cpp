#include "QStdAfx.h"
#include "Default_Technique_Loader.h"
#include "system/System.h"
#include "video/Renderer.h"

#include "../../pugixml/src/pugixml.hpp"
#define PUGIXML_HEADER_ONLY
#include "../../pugixml/src/pugixml.cpp"

using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace video;

using namespace pugi;

bool DefaultTechniqueLoader::can_load_from_source(data::Source& source) const
{
	source.seek(0);

    auto str = data::read_whole_source_as_string<std::string>(source);

	xml_document doc;
	xml_parse_result res = doc.load(str.c_str());

	xml_node techn = doc.child("technique");
	if (!res || !techn)
	{
		return false;
	}

	return true;
}

void DefaultTechniqueLoader::load(Path const& path, data::Source& source, Technique& technique) const
{
	technique.unload();
	source.seek(0);

	auto str = data::read_whole_source_as_string<std::string>(source);

	xml_document doc;
	xml_parse_result res = doc.load(str.c_str());

	xml_node techn = doc.child("technique");
	if (!res || !techn)
	{
        QLOGE("Invalid technique '{}'", path);
		return;
	}

	if (!load_technique(path, techn, technique))
	{
		return;
	}

	technique.compile();
	//technique->set_is_valid(true);
}

bool DefaultTechniqueLoader::load_technique(Path const& path, const xml_node& root, Technique& t) const
{
	xml_node rtn = root.child("render_target");
	while (rtn)
	{
		if (!load_render_target(path, rtn))
		{
			return false;
		}
		rtn = rtn.next_sibling("render_target");
	}

	xml_node passn = root.child("pass");
	while (passn)
	{
		Pass pass;
		if (!load_pass(path, passn, pass))
		{
			return false;
		}
		t.add_pass(pass);
		passn = passn.next_sibling("pass");
	}
	if (t.get_pass_count() == 0)
	{
        QLOGE("No passes found in technique '{}'", path);
		return false;
	}
	return true;
}

bool DefaultTechniqueLoader::load_pass(Path const& path, const pugi::xml_node& root, Pass& p) const
{
	xml_attribute a = root.attribute("name");
	p.set_name(a.value());

	a = root.attribute("scope");
	if (a)
	{
		p.set_scope_name(a.value());
	}

	a = root.attribute("render_target");
	if (a)
	{
        std::string rtname(a.value());
		Render_Target_ptr rt = System::inst().get_renderer()->find_render_target_by_name(rtname);
		QASSERT(rt);
		if (!rt)
		{
            QLOGE("Cannot find render target '{}' necessary for pass '{}'", rtname, p.get_name());
			return false;
		}
		else
		{
			p.set_render_target(rt);
		}
	}

	xml_node unifn = root.child("uniform");
	while (unifn)
	{
		Uniform_Def unif;
		if (!load_uniform(path, unifn, unif))
		{
			return false;
		}
		p.add_uniform_def(unif);
		unifn = unifn.next_sibling("uniform");
	}

	xml_node sampn = root.child("sampler");
	while (sampn)
	{
		Sampler_Def samp;
		if (!load_sampler(path, sampn, samp))
		{
			return false;
		}
		p.add_sampler_def(samp);
		sampn = sampn.next_sibling("sampler");
	}

	xml_node attrn = root.child("attribute");
	while (attrn)
	{
		Attribute_Def attr;
		if (!load_attribute(path, attrn, attr))
		{
			return false;
		}
		p.add_attribute_def(attr);
		attrn = attrn.next_sibling("attribute");
	}

	xml_node rsn = root.child("render_state");
	Render_State_Def rs;
	if (!load_render_state(path, rsn, rs))
	{
		return false;
	}
	p.set_render_state_def(rs);

	xml_node shadern = root.child("shader");
	if (!shadern)
	{
        QLOGE("Cannot find load shader tag, technique '{}'", path);
		return false;
	}
	return load_shader(path, shadern, p);
}

bool DefaultTechniqueLoader::load_uniform(Path const& /*path*/, const pugi::xml_node& root, Uniform_Def& u) const
{
    std::string nameStr(root.attribute("name").value());
    std::string hnameStr(root.attribute("desc").value());
    std::string typeStr(root.attribute("type").value());
    std::string countStr(root.attribute("count").value());
    std::string precisionStr(root.attribute("precision").value());
    std::string linkStr(root.attribute("link").value());
    std::string semanticStr(root.attribute("semantic").value());

	hnameStr = hnameStr.empty() ? nameStr : hnameStr;

	if (!linkStr.empty())
	{
		u = Uniform_Def(nameStr, hnameStr);
		u.set_link(Path(linkStr));
		bool res = System::inst().get_renderer()->link_uniform(u);
		if (!res)
		{
            QLOGE("Cannot link uniform {} to {}", nameStr, linkStr);
			return false;
		}
	}
	else
	{
		Uniform uVal, uMinVal, uMaxVal;

		video::Uniform_Def::Precision precision;
		if (precisionStr == "low")
		{
			precision = video::Uniform_Def::Precision::LOW;
		}
		else if (precisionStr == "medium")
		{
			precision = video::Uniform_Def::Precision::MEDIUM;
		}
		else if (precisionStr == "high")
		{
			precision = video::Uniform_Def::Precision::HIGH;
		}
		else
		{
            QLOGE("Cannot find precision tag");
			return false;
		}

		if (!parse_uniform_value(uVal, typeStr, countStr, root.attribute("val").value()))
		{
			return false;
		}
		if (!parse_uniform_min_value(uMinVal, typeStr, root.attribute("min").value()))
		{
			return false;
		}
		if (!parse_uniform_max_value(uMaxVal, typeStr, root.attribute("max").value()))
		{
			return false;
		}

		u = Uniform_Def(nameStr, hnameStr, uVal, uMinVal, uMaxVal);
		u.set_precision(precision);
		u.set_semantic(semanticStr);
	}
	return true;
}
bool DefaultTechniqueLoader::parse_uniform_value(Uniform& u, std::string const& typeStr, std::string const& countStr, std::string const& val) const
{
	size_t count = 1;
	if (!countStr.empty())
	{
		count = (size_t)math::max(atoi(countStr.c_str()), 1);
	}

	if (typeStr == "bool")
	{
		u = Uniform(video::Uniform::Type::BOOL, count);
		u = parse_bool(val, true);
	}
	else if (typeStr == "int")
	{
		u = Uniform(video::Uniform::Type::INT, count);
		if (val.empty())
		{
			u = (int)0;
			return true;
		}
		int v;
		if (!sscanf(val.c_str(), "%d", &v))
		{
			return false;
		}
		u = v;
	}
	else if (typeStr == "float")
	{
		u = Uniform(video::Uniform::Type::FLOAT, count);
		if (val.empty())
		{
			u = 0.f;
			return true;
		}
		float v;
		if (!sscanf(val.c_str(), "%f", &v))
		{
			return false;
		}
		u = v;
	}
	else if (typeStr == "vec2")
	{
		u = Uniform(video::Uniform::Type::VEC2, count);
		if (val.empty())
		{
			u = math::vec2f(0.f);
			return true;
		}
		float x, y;
		if (sscanf(val.c_str(), "%f,%f", &x, &y) != 2)
		{
			return false;
		}
		u = math::vec2f(x, y);
	}
	else if (typeStr == "vec3")
	{
		u = Uniform(video::Uniform::Type::VEC3, count);
		if (val.empty())
		{
			u = math::vec3f(0.f);
			return true;
		}
		float x, y, z;
		if (sscanf(val.c_str(), "%f,%f,%f", &x, &y, &z) != 3)
		{
			return false;
		}
		u = math::vec3f(x, y, z);
	}
	else if (typeStr == "vec4")
	{
		u = Uniform(video::Uniform::Type::VEC4, count);
		if (val.empty())
		{
			u = math::vec4f(0.f);
			return true;
		}
		float x, y, z, w;
		if (sscanf(val.c_str(), "%f,%f,%f,%f", &x, &y, &z, &w) != 4)
		{
			return false;
		}
		u = math::vec4f(x, y, z, w);
	}
	else
	{
        QLOGE("Cannot find value tag");
		return false;
	}

	return true;
}
bool DefaultTechniqueLoader::parse_bool(std::string const& val, bool def) const
{
	if (val == "1" || val == "yes" || val == "true")
	{
		return true;
	}
	if (val == "0" || val == "no" || val == "false")
	{
		return false;
	}
	return def;
}

bool DefaultTechniqueLoader::parse_uniform_min_value(Uniform& u, std::string const& type, std::string const& val) const
{
	if (type == "bool")
	{
		return val.empty();
	}
	else if (type == "int")
	{
		if (val.empty())
		{
			u = Uniform(-2147483647);
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "float")
	{
		if (val.empty())
		{
			u = Uniform(-999999999.f);
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "vec2")
	{
		if (val.empty())
		{
			u = Uniform(math::vec2f(-999999999.f));
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "vec3")
	{
		if (val.empty())
		{
			u = Uniform(math::vec3f(-999999999.f));
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "vec4")
	{
		if (val.empty())
		{
			u = Uniform(math::vec4f(-999999999.f));
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}

	return val.empty(); //valid only if the value was not specified
}
bool DefaultTechniqueLoader::parse_uniform_max_value(Uniform& u, std::string const& type, std::string const& val) const
{
	if (type == "bool")
	{
		return val.empty();
	}
	else if (type == "int")
	{
		if (val.empty())
		{
			u = Uniform(2147483647);
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "float")
	{
		if (val.empty())
		{
			u = Uniform(999999999.f);
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "vec2")
	{
		if (val.empty())
		{
			u = Uniform(math::vec2f(999999999.f));
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "vec3")
	{
		if (val.empty())
		{
			u = Uniform(math::vec3f(999999999.f));
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}
	else if (type == "vec4")
	{
		if (val.empty())
		{
			u = Uniform(math::vec4f(999999999.f));
			return true;
		}
        return parse_uniform_value(u, type, std::string(), val);
	}

	return val.empty(); //valid only if the value was not specified
}

Sampler_Def::Semantic DefaultTechniqueLoader::parse_semantic(std::string const& value, Sampler_Def::Semantic def) const
{
	if (value.empty())
	{
		return def;
	}

	if (value == "diffuse")
	{
		return Sampler_Def::Semantic::DIFFUSE;
	}
	else if (value == "normal")
	{
		return Sampler_Def::Semantic::NORMAL;
	}
	else if (value == "lightmap")
	{
		return Sampler_Def::Semantic::LIGHTMAP;
	}
	else
	{
        QLOGE("Bad semantic - {}", value);
	}

	return def;
}

bool DefaultTechniqueLoader::load_sampler(Path const& /*path*/, const pugi::xml_node& root, Sampler_Def& s) const
{
    std::string name(root.attribute("name").value());
    std::string hname(root.attribute("desc").value());
    std::string filter(root.attribute("filter").value());
    std::string wrapu(root.attribute("wrapu").value());
    std::string wrapv(root.attribute("wrapv").value());
    std::string mipmap(root.attribute("mipmap").value());
    std::string link(root.attribute("link").value());
    std::string type(root.attribute("type").value());

	//TODO activate
//	if (name.empty() || link.empty())
//		return;

	Sampler sampler;
	sampler.set_filtering((filter == "nearest") ? Sampler::Filter::NEAREST : ((filter == "trilinear") ? Sampler::Filter::TRILINEAR : Sampler::Filter::BILINEAR));
	sampler.set_wrapping((wrapu == "wrap") ? Sampler::Wrap::WRAP : Sampler::Wrap::CLAMP, (wrapv == "wrap") ? Sampler::Wrap::WRAP : Sampler::Wrap::CLAMP);
	sampler.set_mipmapping(parse_bool(mipmap, true));
	sampler.set_type((type == "cube") ? Sampler::Type::TEXTURE_CUBE : Sampler::Type::TEXTURE_2D);

	s = Sampler_Def(name, hname, sampler);
	s.set_semantic(parse_semantic(root.attribute("semantic").value(), Sampler_Def::Semantic::USER));
	//s.setGetter(System::inst().getRenderer()->getSamplerGetter(Path(link)));
	if (!link.empty())
	{
		bool res = System::inst().get_renderer()->link_sampler(s, Path(link));
		if (!res)
		{
            QLOGE("Cannot link sampler {} to {}", name, link);
			return false;
		}
	}
	return true;
}

bool DefaultTechniqueLoader::load_attribute(Path const& path, const pugi::xml_node& root, Attribute_Def& a) const
{
    std::string name(root.attribute("name").value());
    std::string hname(root.attribute("desc").value());
    std::string link(root.attribute("link").value());
    std::string count(root.attribute("count").value());

	if (name.empty() || link.empty())
	{
        QLOGE("{}: Attribute without name or link.", path);
		return false;
	}

	a = Attribute_Def(name, hname);

	size_t icount = atoi(count.c_str());
	if (icount > 0)
	{
		a.set_component_count(icount);
	}

	a.set_attribute_name(link);

	if (a.get_component_count() == 0)
	{
        QLOGE("{}: Attribute with bad count {}.", path, icount);
		return false;
	}
	return true;
}

bool DefaultTechniqueLoader::load_render_state(Path const& /*path*/, const pugi::xml_node& root, Render_State_Def& r) const
{
	if (!root)
	{
		return true;
	}

	Render_State rs;

	xml_node blend = root.child("blend");
	if (blend)
	{
		Render_State::Blend_Formula formula;
		xml_node srcFactor = blend.child("src");
        std::string val = std::string(srcFactor.attribute("val").value());
		formula.src_color = formula.src_alpha = parse_blend_factor(val, Render_State::Blend_Formula::Factor::ONE);

		xml_node dstFactor = blend.child("dst");
        val = std::string(dstFactor.attribute("val").value());
		formula.dst_color = formula.dst_alpha = parse_blend_factor(val, Render_State::Blend_Formula::Factor::ZERO);
		rs.set_blend_formula(formula);
	}

	xml_node cull = root.child("cull");
	if (cull)
	{
        std::string val(cull.attribute("val").value());
		rs.set_culling(parse_bool(val, true));
		if (rs.get_culling())
		{
			xml_node face = cull.child("face");
            val = std::string(face.attribute("val").value());
			rs.set_cull_face(val == "front" ? Render_State::Cull_Face::FRONT : Render_State::Cull_Face::BACK);

			xml_node wind = cull.child("wind");
            val = std::string(wind.attribute("val").value());
			rs.set_winding(val == "ccw" ? Render_State::Winding::WINDING_CCW : Render_State::Winding::WINDING_CW);
		}
	}

	xml_node depth = root.child("depth");
	if (depth)
	{
		xml_node test = depth.child("test");
        std::string val(test.attribute("val").value());
		rs.set_depth_test(parse_bool(val, true));

		xml_node write = depth.child("write");
        val = std::string(write.attribute("val").value());
		rs.set_depth_write(parse_bool(val, true));
	}

	r = Render_State_Def(false, false, false, rs);

	return true;
}
Render_State::Blend_Formula::Factor DefaultTechniqueLoader::parse_blend_factor(std::string const& val, Render_State::Blend_Formula::Factor def) const
{
	if (val.empty())
	{
		return def;
	}
	if (val == "1" || val == "one")
	{
		return Render_State::Blend_Formula::Factor::ONE;
	}
	if (val == "0" || val == "zero")
	{
		return Render_State::Blend_Formula::Factor::ZERO;
	}
	if (val == "src_color")
	{
		return Render_State::Blend_Formula::Factor::SRC_COLOR;
	}
	if (val == "inv_src_color")
	{
		return Render_State::Blend_Formula::Factor::INV_SRC_COLOR;
	}
	if (val == "dst_color")
	{
		return Render_State::Blend_Formula::Factor::DST_COLOR;
	}
	if (val == "inv_dst_color")
	{
		return Render_State::Blend_Formula::Factor::INV_DST_COLOR;
	}
	if (val == "src_alpha")
	{
		return Render_State::Blend_Formula::Factor::SRC_ALPHA;
	}
	if (val == "inv_src_alpha")
	{
		return Render_State::Blend_Formula::Factor::INV_SRC_ALPHA;
	}
	if (val == "dst_alpha")
	{
		return Render_State::Blend_Formula::Factor::DST_ALPHA;
	}
	if (val == "inv_dst_alpha")
	{
		return Render_State::Blend_Formula::Factor::INV_DST_ALPHA;
	}
	if (val == "src_alpha_saturated")
	{
		return Render_State::Blend_Formula::Factor::SRC_ALPHA_SATURATED;
	}
	else
	{
        QLOGE("Bad blend factor - {}", val);
	}
	return def;
}

bool DefaultTechniqueLoader::load_shader(Path const& path, const pugi::xml_node& root, Pass& p) const
{
	if (!root)
	{
        QLOGE("Cannot load shader pass from '{}'", path);
		return false;
	}
	Uber_Shader shader;

    std::string vs, fs;
	xml_node pn = root.child("vshader");
	{
		pugi::xml_node cdata = pn.first_child();
		vs = cdata.value();
		if (vs.empty())
		{
            QLOGE("No source specified for vs, technique '{}'", path);
			return false;
		}
	}

	pn = root.child("fshader");
	{
		pugi::xml_node cdata = pn.first_child();
		fs = cdata.value();
		if (fs.empty())
		{
            QLOGE("No source specified for fs, technique '{}'", path);
			return false;
		}
	}
	shader.set_source(vs, fs);
	p.set_uber_shader(shader);
	return true;
}

bool DefaultTechniqueLoader::load_render_target(Path const& /*path*/, const pugi::xml_node& root) const
{
	Render_Target::Color_Format color_format = Render_Target::Color_Format::NONE;
	Render_Target::Depth_Format depth_format = Render_Target::Depth_Format::NONE;
	Render_Target::Stencil_Format stencil_format = Render_Target::Stencil_Format::NONE;
	Render_Target::AA_Format aa_format = Render_Target::AA_Format::NONE;
	math::vec2u32 size;

	pugi::xml_attribute att = root.attribute("color");
    std::string cf(att.value());

#define COLOR_FORMAT(format) else if (cf == #format) color_format = Render_Target::Color_Format::format

	if (cf.empty()) {}
	COLOR_FORMAT(RGBA_4);
	COLOR_FORMAT(RGB_565);
	COLOR_FORMAT(RGBA_5551);
	COLOR_FORMAT(RGB_8);
	COLOR_FORMAT(RGBA_4);
	COLOR_FORMAT(I_8);
	COLOR_FORMAT(A_8);
	COLOR_FORMAT(AI_8);
	COLOR_FORMAT(RGBA_16);
	COLOR_FORMAT(RGB_16);
	COLOR_FORMAT(I_16);
	COLOR_FORMAT(A_16);
	COLOR_FORMAT(AI_16);
	COLOR_FORMAT(RGBA_32);
	COLOR_FORMAT(RGB_32);
	COLOR_FORMAT(I_32);
	COLOR_FORMAT(A_32);
	COLOR_FORMAT(AI_32);
	else if (cf != "")
	{
        QLOGE("Unknown render target color format '{}'", cf);
		return false;
	}

	att = root.attribute("depth");
    std::string df(att.value());
	if (df == "HALF")
	{
		depth_format = Render_Target::Depth_Format::HALF;
	}
	else if (df == "FULL")
	{
		depth_format = Render_Target::Depth_Format::FULL;
	}
	else if (df != "")
	{
        QLOGE("Unknown render target depth format '{}'", df);
		return false;
	}

	att = root.attribute("stencil");
    std::string sf(att.value());
	if (sf == "FULL")
	{
		stencil_format = Render_Target::Stencil_Format::FULL;
	}
	else if (sf != "")
	{
        QLOGE("Unknown render target stencil format '{}'", sf);
		return false;
	}

	att = root.attribute("aa");
    std::string af(att.value());
	if (af == "2x")
	{
		aa_format = Render_Target::AA_Format::A2X;
	}
	else if (af == "4x")
	{
		aa_format = Render_Target::AA_Format::A4X;
	}
	else if (af == "8x")
	{
		aa_format = Render_Target::AA_Format::A8X;
	}
	else if (af == "16x")
	{
		aa_format = Render_Target::AA_Format::A16X;
	}
	else if (af != "")
	{
        QLOGE("Unknown render target aa format '{}'", af);
		return false;
	}

	att = root.attribute("size");
	if (att)
	{
		float ratio = att.as_float();
		if (math::is_zero(ratio))
		{
            QLOGE("Cannot have a ratio of 0");
			return false;
		}
		size = math::vec2u32(math::round(math::vec2f(System::inst().get_renderer()->get_viewport_size()) * ratio));
	}
	else
	{
		att = root.attribute("width");
		if (!att)
		{
            QLOGE("Missing width tag in render target definition");
			return false;
		}
		uint32_t w = att.as_uint();
		att = root.attribute("height");
		if (!att)
		{
            QLOGE("Missing height tag in render target definition");
			return false;
		}
		uint32_t h = att.as_uint();
		size = math::vec2u32(w, h);
	}

	att = root.attribute("name");
    std::string name(att.value());
	if (name.empty())
	{
        QLOGE("Missing name tag in render target definition");
		return false;
	}

	if (color_format == Render_Target::Color_Format::NONE &&
		depth_format == Render_Target::Depth_Format::NONE &&
		stencil_format == Render_Target::Stencil_Format::NONE &&
		aa_format == Render_Target::AA_Format::NONE)
	{
        QLOGE("No format specified for render target");
		return false;
	}

	Render_Target_ptr rt = System::inst().get_renderer()->find_render_target_by_name(name);
	if (rt)
	{
		if (color_format != rt->get_color_format() ||
			depth_format != rt->get_depth_format() ||
			stencil_format != rt->get_stencil_format() ||
			aa_format != rt->get_aa_format())
		{
            QLOGE("Render target '{}' was previously declared with a different format/size", name);
			return false;
		}
		return true;
	}

	rt = Render_Target::create();
	rt->allocate(size, color_format, depth_format, stencil_format, aa_format, false);

	System::inst().get_renderer()->add_render_target(name, rt);
	return true;
}
