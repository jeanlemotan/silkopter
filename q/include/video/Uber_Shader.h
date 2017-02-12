#pragma once

#include "video/Shader.h"
#include "video/Shader_Source.h"
#include "Ptr_Fw_Declaration.h"

namespace q
{
namespace video
{

class Uber_Shader
{
public:
    Uber_Shader();
    Uber_Shader(Uber_Shader&& other);

    Uber_Shader(Uber_Shader const&) = default;
    Uber_Shader& operator=(Uber_Shader const&) = default;

    void		set_root_path(Path const& path);
    Path const&	get_root_path() const;

    void		set_source(std::string const& vs, std::string const& fs);
    void		compile(
            std::vector<Uniform_Def>& uniforms,
            std::vector<Sampler_Def>& samplers,
            std::vector<Attribute_Def>& attributes);

    Shader_ptr const&	get_shader(Material const& material, Shader::Type type) const;

private:
    uint64_t			compute_key(Material const& material, Shader::Type type) const;

    Path		m_root_path;
    Shader_Source m_vertex_src;
    Shader_Source m_fragment_src;

    std::vector<Uniform_Def>* m_uniforms = nullptr;
    std::vector<Sampler_Def>* m_samplers = nullptr;
    std::vector<Attribute_Def>* m_attributes = nullptr;

    mutable std::unordered_map<uint64_t, Shader_ptr> m_shaders;
    mutable Shader_ptr	m_base;
};

}
}

