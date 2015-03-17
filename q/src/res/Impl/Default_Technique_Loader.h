#pragma once

#include "res/Loader.h"
#include "video/Technique.h"
#include "video/Uniform.h"
#include "video/Render_State.h"
#include "video/Sampler_Def.h"
#include "video/Attribute_Def.h"
#include "video/Pass.h"
#include "video/Render_State_Def.h"
#include "video/Uniform_Def.h"

namespace pugi
{
	class xml_node;
}

namespace q
{
namespace res
{
namespace impl
{

	class DefaultTechniqueLoader : public res::Loader<video::Technique>
	{
		virtual void load(Path const& path, data::Source& source, video::Technique& r) const;
		virtual bool can_load_from_source(data::Source& source) const;

	private:
		bool load_technique(Path const& path, const pugi::xml_node& root, video::Technique& t) const;
		bool load_pass(Path const& path, const pugi::xml_node& root, video::Pass& p) const;
		bool load_uniform(Path const& path, const pugi::xml_node& root, video::Uniform_Def& u) const;
		bool load_sampler(Path const& path, const pugi::xml_node& root, video::Sampler_Def& s) const;
		bool load_attribute(Path const& path, const pugi::xml_node& root, video::Attribute_Def& a) const;
		bool load_render_state(Path const& path, const pugi::xml_node& root, video::Render_State_Def& r) const;
		bool load_render_target(Path const& path, const pugi::xml_node& root) const;

		bool load_shader(Path const& path, const pugi::xml_node& root, video::Pass& p) const;

		video::Sampler_Def::Semantic parse_semantic(String const& value, video::Sampler_Def::Semantic def) const;
		bool parse_bool(String const& val, bool def) const;
		bool parse_uniform_value(video::Uniform& u, String const& type, String const& count, String const& val) const;
		bool parse_uniform_min_value(video::Uniform& u, String const& type, String const& val) const;
		bool parse_uniform_max_value(video::Uniform& u, String const& type, String const& val) const;
		video::Render_State::Blend_Formula::Factor parse_blend_factor(String const& val, video::Render_State::Blend_Formula::Factor def) const;
	};


}
}
}
