#pragma once

#include "video/Shader.h"
#include "video/GLES/GLES_Interface.h"
#include "video/GLES/GLES_Command_Stream.h"

namespace q
{
namespace video
{

class GLES_Shader : public Shader
{
    friend class Command_Stream;
public:
    GLES_Shader(Type type, Shader_Source const& vertex, Shader_Source const& fragment);
    ~GLES_Shader();

    virtual Type		get_type() const;
    virtual Shader_Source const& get_vertex_shader_source() const;
    virtual Shader_Source const& get_fragment_shader_source() const;
    virtual void		compile(std::vector<Uniform_Def>& uniforms,
                                std::vector<Sampler_Def>& samplers,
                                std::vector<Attribute_Def>& attributes);

    std::vector<Uniform_Def> const& get_uniforms() const;
    std::vector<Sampler_Def> const& get_samplers() const;
    std::vector<Attribute_Def> const& get_attributes() const;

protected:

    void		upload_uniform(uint32_t uniform_id, Uniform const& u, uint8_t const* data) const;
    uint32_t	compile_shader(uint32_t shader_type, std::string const& source);

    void		upload_data(gles::Interface& interf, const Command_Stream::Render_Context& context) const;

    std::string	patch_shader(std::vector<Uniform_Def>& uniforms,
                             std::vector<Sampler_Def>& samplers,
                             std::vector<Attribute_Def>& attributes,
                             Shader_Source const& src);

private:
    std::string get_shader_type_str(Uniform_Def const& def) const;
    std::string get_shader_type_precision_str(Uniform_Def const& def) const;

    Uniform const& get_uniform_skin_matrix(void* data) const;

    Type		m_type = Type::NORMAL;
    Shader_Source	m_vertex_shader_src;
    Shader_Source	m_fragment_shader_src;

    std::string		m_patched_vertex_shader_src;
    std::string		m_patched_fragment_shader_src;

    std::vector<Uniform_Def> m_uniforms;
    std::vector<Sampler_Def> m_samplers;
    std::vector<Attribute_Def> m_attributes;

    uint32_t			m_vertex_shader_id = 0;
    uint32_t			m_fragment_shader_id = 0;
    uint32_t			m_shader_id = 0;

    size_t			m_max_skin_nodes = 0;

    size_t			m_max_uniforms = 0;
    size_t			m_max_samplers = 0;
    size_t			m_max_attributes = 0;

    std::vector<int> m_uniform_gl_ids;

    mutable std::vector<Uniform> m_cached_uniforms;
};

DECLARE_CLASS_PTR(GLES_Shader);

inline std::vector<Uniform_Def> const& GLES_Shader::get_uniforms() const
{
    return m_uniforms;
}
inline std::vector<Sampler_Def> const& GLES_Shader::get_samplers() const
{
    return m_samplers;
}
inline std::vector<Attribute_Def> const& GLES_Shader::get_attributes() const
{
    return m_attributes;
}

}
}
