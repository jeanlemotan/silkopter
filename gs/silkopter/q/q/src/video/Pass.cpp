#include "QStdAfx.h"
#include "video/Pass.h"

using namespace q;
using namespace video;

Pass::Pass()
	: m_scope_index(0)
{
}
Pass::~Pass()
{
}
Pass::Pass(Pass&& other)
	: m_name(std::move(other.m_name))
	, m_scope_name(std::move(other.m_scope_name))
	, m_scope_index(std::move(other.m_scope_index))
	, m_uniforms(std::move(other.m_uniforms))
	, m_material_uniform_data(std::move(other.m_material_uniform_data))
	, m_material_uniform_idx_to_uniform_idx(std::move(other.m_material_uniform_idx_to_uniform_idx))
	, m_uniform_idx_to_material_uniform_idx(std::move(other.m_uniform_idx_to_material_uniform_idx))
	, m_material_uniform_map(std::move(other.m_material_uniform_map))
	, m_samplers(std::move(other.m_samplers))
	, m_material_samplers_idx(std::move(other.m_material_samplers_idx))
	, m_material_sampler_map(std::move(other.m_material_sampler_map))
	, m_attributes(std::move(other.m_attributes))
	, m_attribute_map(std::move(other.m_attribute_map))
	, m_render_state(std::move(other.m_render_state))
	, m_uber_shader(std::move(other.m_uber_shader))
	, m_render_target(std::move(other.m_render_target))
{
}
auto Pass::operator=(Pass&& other) -> Pass&
{
	std::swap(m_name, other.m_name);
	std::swap(m_scope_name, other.m_scope_name);
	std::swap(m_scope_index, other.m_scope_index);
	std::swap(m_uniforms, other.m_uniforms);
	std::swap(m_material_uniform_data, other.m_material_uniform_data);
	std::swap(m_material_uniform_idx_to_uniform_idx, other.m_material_uniform_idx_to_uniform_idx);
	std::swap(m_uniform_idx_to_material_uniform_idx, other.m_uniform_idx_to_material_uniform_idx);
	std::swap(m_material_uniform_map, other.m_material_uniform_map);
	std::swap(m_samplers, other.m_samplers);
	std::swap(m_material_samplers_idx, other.m_material_samplers_idx);
	std::swap(m_material_sampler_map, other.m_material_sampler_map);
	std::swap(m_attributes, other.m_attributes);
	std::swap(m_attribute_map, other.m_attribute_map);
	std::swap(m_render_state, other.m_render_state);
	std::swap(m_uber_shader, other.m_uber_shader);
	std::swap(m_render_target, other.m_render_target);
	return *this;
}

void Pass::set_name(String const& name)
{
	m_name = name;
}
auto Pass::get_name() const -> String
{
	return m_name;
}

void Pass::set_scope_name(String const& scopeName)
{
	m_scope_name = scopeName;
	QASSERT(0); //TODO
}
auto Pass::get_scope_name() const -> String
{
	return m_scope_name;
}
auto Pass::get_scope_idx() const -> size_t
{
	return m_scope_index;
}

//////////////////////////////////////////////////////////////////////////

void Pass::add_uniform_def(Uniform_Def const& def)
{
	QASSERT(find_uniform_def_idx_by_name(def.get_name()) == -1 && find_attribute_def_idx_by_name(def.get_name()) == -1 && find_sampler_def_idx_by_name(def.get_name()) == -1);
	size_t idx = m_uniforms.size();
	m_uniforms.push_back(def);
	m_uniform_idx_to_material_uniform_idx.push_back(static_cast<uint32_t>(-1));

	if (!def.has_getter())
	{
		if (m_material_uniform_data.empty())
		{
			m_material_uniform_data.emplace_back(0, def.get_default_value().get_data_size());
		}
		else
		{
			m_material_uniform_data.emplace_back(m_material_uniform_data.back().first + m_material_uniform_data.back().second, def.get_default_value().get_data_size());
		}

		m_material_uniform_idx_to_uniform_idx.push_back(idx);
		m_material_uniform_map[def.get_name()] = m_material_uniform_idx_to_uniform_idx.size() - 1;
		m_uniform_idx_to_material_uniform_idx.back() = m_material_uniform_idx_to_uniform_idx.size() - 1;
	}
}
auto Pass::get_material_uniform_idx_from_uniform_idx(size_t uniformIdx) const -> int
{
	return m_uniform_idx_to_material_uniform_idx[uniformIdx];
}
auto Pass::get_material_uniform_def_count() const -> size_t
{
	return m_material_uniform_idx_to_uniform_idx.size();
}
auto Pass::get_uniform_def_count() const -> size_t
{
	return m_uniforms.size();
}
auto Pass::get_material_uniform_def(size_t idx) const -> Uniform_Def const&
{
	return m_uniforms[m_material_uniform_idx_to_uniform_idx[idx]];
}
auto Pass::get_material_uniform_def_data_offset(size_t idx) const -> size_t
{
	return m_material_uniform_data[idx].first;
}
auto Pass::get_material_uniform_def_data_size(size_t idx) const -> size_t
{
	return m_material_uniform_data[idx].second;
}
auto Pass::find_material_uniform_def_idx_by_name(String const& name) const -> int
{
	auto it = m_material_uniform_map.find(name);
	return it != m_material_uniform_map.end() ? it->second : -1;
}
auto Pass::find_uniform_def_idx_by_name(String const& name) const -> int
{
	auto it = std::find_if(m_uniforms.begin(), m_uniforms.end(), [&](Uniform_Def const& def) { return def.get_name() == name; });
	return it != m_uniforms.end() ? std::distance(m_uniforms.begin(), it) : -1;
}

//////////////////////////////////////////////////////////////////////////

void Pass::add_sampler_def(Sampler_Def const& def)
{
	QASSERT(find_material_uniform_def_idx_by_name(def.get_name()) == -1 && find_attribute_def_idx_by_name(def.get_name()) == -1 && find_sampler_def_idx_by_name(def.get_name()) == -1);
	size_t idx = m_samplers.size();
	m_samplers.push_back(def);
	if (!def.has_getter())
	{
		m_material_samplers_idx.push_back(idx);
		m_material_sampler_map[def.get_name()] = m_material_samplers_idx.size() - 1;
	}
}
auto Pass::get_sampler_def_count() const -> size_t
{
	return m_samplers.size();
}
auto Pass::get_sampler_def(size_t idx) const -> Sampler_Def const&
{
	return m_samplers[idx];
}
auto Pass::find_sampler_def_idx_by_name(String const& name) const -> int
{
	auto it = std::find_if(m_samplers.begin(), m_samplers.end(), [&](Sampler_Def const& def) { return def.get_name() == name; });
	return it != m_samplers.end() ? std::distance(m_samplers.begin(), it) : -1;
}
auto Pass::get_material_sampler_def_count() const -> size_t
{
	return m_material_samplers_idx.size();
}
auto Pass::get_material_sampler_def(size_t idx) const -> Sampler_Def const&
{
	return m_samplers[m_material_samplers_idx[idx]];
}
auto Pass::find_material_sampler_def_idx_by_name(String const& name) const -> int 
{
	auto it = m_material_sampler_map.find(name);
	return it != m_material_sampler_map.end() ? it->second : -1;
}

//////////////////////////////////////////////////////////////////////////

void Pass::add_attribute_def(Attribute_Def const& def)
{
	QASSERT(find_material_uniform_def_idx_by_name(def.get_name()) == -1 && find_attribute_def_idx_by_name(def.get_name()) == -1 && find_sampler_def_idx_by_name(def.get_name()) == -1);
	size_t idx = m_attributes.size();
	m_attributes.push_back(def);
	m_attribute_map[def.get_name()] = idx;
}
auto Pass::get_attribute_def_count() const -> size_t
{
	return m_attributes.size();
}
auto Pass::get_attribute_def(size_t idx) const -> Attribute_Def const&
{
	QASSERT(idx < get_attribute_def_count());
	return m_attributes[idx];
}
auto Pass::find_attribute_def_idx_by_name(String const& name) const -> int
{
	auto it = m_attribute_map.find(name);
	return it != m_attribute_map.end() ? it->second : -1;
}

//////////////////////////////////////////////////////////////////////////

void Pass::set_render_state_def(Render_State_Def const& def)
{
	m_render_state = def;
}
auto Pass::get_render_state_def() const -> Render_State_Def const&
{
	return m_render_state;
}

//////////////////////////////////////////////////////////////////////////

void Pass::set_uber_shader(Uber_Shader const& p)
{
	m_uber_shader = p;
}
auto Pass::get_uber_shader() const -> Uber_Shader const&
{
	return m_uber_shader;
}

//////////////////////////////////////////////////////////////////////////

void Pass::set_render_target(Render_Target_ptr const& rt)
{
	QASSERT(rt);
	m_render_target = rt;
}

//////////////////////////////////////////////////////////////////////////

void Pass::compile()
{
	m_uber_shader.compile(m_uniforms, m_samplers, m_attributes);
}
