#include "QStdAfx.h"
#include "video/Material.h"

using namespace q;
using namespace video;

Material::Material()
	: m_passes(PassDataAlloc(m_passes_arena))
	, m_samplers(SamplerAlloc(m_samplers_arena))
{
}

Material::Material(Technique_cptr const& technique)
	: m_passes(PassDataAlloc(m_passes_arena))
	, m_samplers(SamplerAlloc(m_samplers_arena))
{
	set_technique(technique);
}

Material::Material(Material const& other)
	: m_technique(other.m_technique)
	, m_passes(PassDataAlloc(m_passes_arena))
	, m_samplers(SamplerAlloc(m_samplers_arena))
    , m_uniform_data(other.m_uniform_data)
{
	m_passes = other.m_passes;
	m_samplers = other.m_samplers;
}
// Material::Material(Material&& other)
// 	: m_technique(std::move(other.m_technique))
// 	, m_passes(PassDataAlloc(m_passes_arena))
// 	, m_uniform_data(std::move(other.m_uniform_data))
// 	, m_samplers(SamplerAlloc(m_samplers_arena))
// {
// 	m_passes = std::move(other.m_passes);
// 	m_samplers = std::move(other.m_samplers);
// }
Material& Material::operator=(Material const& other)
{
	m_technique = other.m_technique;
	m_passes = other.m_passes;
	m_uniform_data = other.m_uniform_data;
	m_samplers = other.m_samplers;
	return *this;
}
// Material& Material::operator=(Material&& other)
// {
// 	m_technique = std::move(other.m_technique);
// 	m_passes = std::move(other.m_passes);
// 	m_uniform_data = other.m_uniform_data;
// 	m_samplers = std::move(other.m_samplers);
// 	return *this;
// }
bool Material::operator==(Material const& other) const
{
	return m_technique == other.m_technique &&
		m_passes == other.m_passes &&
		m_uniform_data == other.m_uniform_data &&
		m_samplers == other.m_samplers;
}

void Material::set_technique(Technique_cptr const& t)
{
	if (m_technique == t)
	{
		return;
	}

	m_technique = t;

	m_samplers.clear();
	m_passes.clear();

	if (m_technique)
	{
		size_t ubytes = 0;
		size_t ucount = 0;
		//first count total uniform size to allocate data
		m_passes.resize(t->get_pass_count());
		for (size_t i = 0; i < m_passes.size(); i++)
		{
			Pass const& pass = t->get_pass(i);
			m_passes[i].render_state = pass.get_render_state_def().get_default_value();

			m_passes[i].uniform_start_idx = ubytes;
			for (size_t j = 0; j < pass.get_material_uniform_def_count(); j++)
			{
				QASSERT(!pass.get_material_uniform_def(j).has_getter());
				ubytes += pass.get_material_uniform_def(j).get_default_value().get_data_size();
				ucount++;
			}
		}

		m_uniform_data.uninitialized_resize(ubytes);

		for (size_t i = 0; i < m_passes.size(); i++)
		{
			Pass const& pass = t->get_pass(i);

			m_passes[i].render_state = pass.get_render_state_def().get_default_value();

			for (size_t j = 0; j < pass.get_material_uniform_def_count(); j++)
			{
				uint8_t* data = &m_uniform_data[m_passes[i].uniform_start_idx + pass.get_material_uniform_def_data_offset(j)];
				Uniform u(pass.get_material_uniform_def(j).get_default_value().get_type(), pass.get_material_uniform_def(j).get_default_value().get_count(), data);
				u = pass.get_material_uniform_def(j).get_default_value();
			}

			size_t sc = pass.get_material_sampler_def_count();
			m_passes[i].sampler_start_idx = m_samplers.size();
			m_passes[i].sampler_count = sc;
			for (size_t j = 0; j < sc; j++)
			{
				m_samplers.push_back(pass.get_material_sampler_def(j).get_default_value());
			}
		}
	}
}

Uniform Material::get_uniform(size_t pass_idx, size_t idx) const
{
	QASSERT(m_technique);
	Pass const& pass = m_technique->get_pass(pass_idx);
	uint8_t* data = &m_uniform_data[m_passes[pass_idx].uniform_start_idx + pass.get_material_uniform_def_data_offset(idx)];
	Uniform u(pass.get_material_uniform_def(idx).get_default_value().get_type(), pass.get_material_uniform_def(idx).get_default_value().get_count(), data);
	return u;
}

Uniform_Def const& Material::get_uniform_def(size_t pass_idx, size_t idx) const
{
	QASSERT(m_technique);
	Pass const& pass = m_technique->get_pass(pass_idx);
	return pass.get_material_uniform_def(idx);
}

void Material::set_uniform(size_t pass_idx, size_t idx, Uniform const& uniform)
{
	QASSERT(m_technique);
	Pass const& pass = m_technique->get_pass(pass_idx);
	uint8_t* data = &m_uniform_data[m_passes[pass_idx].uniform_start_idx + pass.get_material_uniform_def_data_offset(idx)];
	Uniform u(pass.get_material_uniform_def(idx).get_default_value().get_type(), pass.get_material_uniform_def(idx).get_default_value().get_count(), data);
	QASSERT(u.get_type() == uniform.get_type());
	QASSERT(u.get_count() >= uniform.get_count());
	u = uniform;
}

uint8_t const* Material::get_uniform_data(size_t pass_idx, size_t idx) const
{
	QASSERT(m_technique);
	Pass const& pass = m_technique->get_pass(pass_idx);
	uint8_t* data = &m_uniform_data[m_passes[pass_idx].uniform_start_idx + pass.get_material_uniform_def_data_offset(idx)];
	return data;
}

void Material::set_sampler(size_t pass_idx, size_t idx, Sampler const& sampler)
{
	m_samplers[m_passes[pass_idx].sampler_start_idx + idx] = sampler;
}
