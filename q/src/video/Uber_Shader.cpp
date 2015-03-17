#include "QStdAfx.h"
#include "util/Preprocessor.h"
#include "video/Uber_Shader.h"
#include "system/System.h"
#include "video/Renderer.h"

using namespace q;
using namespace video;

Uber_Shader::Uber_Shader()
{

}
Uber_Shader::Uber_Shader(Uber_Shader&& other)
	: m_root_path(std::move(other.m_root_path))
	, m_vertex_src(std::move(other.m_vertex_src))
	, m_fragment_src(std::move(other.m_fragment_src))
	, m_uniforms(std::move(other.m_uniforms))
	, m_samplers(std::move(other.m_samplers))
	, m_attributes(std::move(other.m_attributes))
	, m_shaders(std::move(other.m_shaders))
	, m_base(std::move(other.m_base))
{
}
void Uber_Shader::set_root_path(Path const& path)
{
	m_root_path = path;
}
Path const& Uber_Shader::get_root_path() const
{
	return m_root_path;
}

void Uber_Shader::set_source(String const& vs, String const& fs)
{
	util::Preprocessor pp(System::inst().get_file_system(), get_root_path());
	m_vertex_src = Shader_Source(pp.preprocess(vs));
	m_fragment_src = Shader_Source(pp.preprocess(fs));
}

void Uber_Shader::compile(
	std::vector<Uniform_Def>& uniforms,
	std::vector<Sampler_Def>& samplers,
	std::vector<Attribute_Def>& attributes)
{
	m_uniforms = &uniforms;
	m_samplers = &samplers;
	m_attributes = &attributes;
 	m_base = Shader::create(Shader::Type::NORMAL, m_vertex_src, m_fragment_src);
 	m_base->compile(uniforms, samplers, attributes);
}

//////////////////////////////////////////////////////////////////////////

uint64_t Uber_Shader::compute_key(Material const& /*material*/, Shader::Type type) const
{
	return ((uint64_t)type) << 56;
}

//////////////////////////////////////////////////////////////////////////

Shader_ptr const& Uber_Shader::get_shader(Material const& material, Shader::Type type) const
{
	uint64_t key = compute_key(material, type);
	if (key == 0)
	{
		return m_base;
	}
	else
	{
		//QASSERT(0);
		//return m_base;
		{
			auto it = m_shaders.find(key);
			if (it != m_shaders.end())
			{
				return it->second;
			}
		}

		auto shader = Shader::create(type, m_vertex_src, m_fragment_src);
		shader->compile(*m_uniforms, *m_samplers, *m_attributes);
		auto it = m_shaders.insert(std::make_pair(key, shader));
		return it.first->second;
	}
}

//////////////////////////////////////////////////////////////////////////

