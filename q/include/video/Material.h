#pragma once

#include "video/Uniform.h"
#include "video/Uniform_Def.h"
#include "video/Render_State.h"
#include "video/Sampler.h"
#include "Ptr_Fw_Declaration.h"
#include "Technique.h"

namespace q
{
namespace video
{

class Material
{
public:
    Material();
    Material(Technique_cptr const& technique);
    Material(Material const& other);
    //		Material(Material&& other); //CV: until I fix the swap semantics for the EmbeddedAllocator
    ~Material();

    Material&		operator=(Material const& other);
    //Material&		operator=(Material&& other);

    bool			operator==(Material const& other) const;
    DEFINE_EQ_OPERATORS(Material);

    void				set_technique(Technique_cptr const& t);
    Technique_cptr const&get_technique() const;

    size_t				get_pass_count() const;

    Render_State const&	get_render_state(size_t pass_idx) const;
    Render_State&		get_render_state(size_t pass_idx);
    void				set_render_state(size_t pass_idx, Render_State const& rs);

    int					find_pass_idx_by_name(std::string const& name) const;

    int					find_uniform_idx_by_name(size_t pass_idx, std::string const& name) const;
    size_t				get_uniform_count(size_t pass_idx) const;
    std::string			get_uniform_name(size_t pass_idx, size_t uniform_idx) const;
    void				set_uniform(size_t pass_idx, size_t uniform_idx, Uniform const& uniform);
    Uniform				get_uniform(size_t pass_idx, size_t uniform_idx) const;
    Uniform_Def const&	get_uniform_def(size_t pass_idx, size_t uniform_idx) const;
    uint8_t const*		get_uniform_data(size_t pass_idx, size_t uniform_idx) const;

    int					find_sampler_idx_by_name(size_t pass_idx, std::string const& name) const;
    size_t				get_sampler_count(size_t pass_idx) const;
    std::string			get_sampler_name(size_t pass_idx, size_t sampler_idx) const;
    void				set_sampler(size_t pass_idx, size_t sampler_idx, Sampler const& sampler);
    Sampler const&		get_sampler(size_t pass_idx, size_t sampler_idx) const;

    bool				is_valid() const;

private:
    Technique_cptr		m_technique;
    struct PassData
    {
        Render_State render_state;
        size_t uniform_start_idx = 0;
        size_t sampler_start_idx = 0;
        size_t sampler_count = 0;

        bool operator==(PassData const& other) const
        {
            return render_state == other.render_state &&
                    uniform_start_idx == other.uniform_start_idx &&
                    sampler_count == other.sampler_count &&
                    sampler_start_idx == other.sampler_start_idx;
        }
    };

    typedef mem::Embedded_Allocator<PassData, sizeof(PassData) * 2> PassDataAlloc;
    typedef mem::Embedded_Allocator<Sampler, sizeof(Sampler) * 4> SamplerAlloc;

    PassDataAlloc::arena_type m_passes_arena;
    SamplerAlloc::arena_type m_samplers_arena;

    std::vector<PassData, PassDataAlloc>	m_passes;
    std::vector<Sampler, SamplerAlloc>		m_samplers;

    //uniforms are created on the fly using this storage.
    mutable boost::auto_buffer<uint8_t, boost::store_n_bytes<64>> m_uniform_data;
};


inline Material::~Material()
{
}
inline Technique_cptr const& Material::get_technique() const
{
    return m_technique;
}
inline size_t Material::get_pass_count() const
{
    return m_technique ? m_technique->get_pass_count() : 0;
}
inline int Material::find_pass_idx_by_name(std::string const& name) const
{
    return m_technique ? m_technique->find_pass_idx_by_name(name) : -1;
}

inline size_t Material::get_uniform_count(size_t pass_idx) const
{
    return m_technique ? m_technique->get_pass(pass_idx).get_material_uniform_def_count() : 0u;
}
inline std::string Material::get_uniform_name(size_t pass_idx, size_t uniform_idx) const
{
    return m_technique ? m_technique->get_pass(pass_idx).get_material_uniform_def(uniform_idx).get_name() : std::string();
}

inline int Material::find_uniform_idx_by_name(size_t pass_idx, std::string const& name) const
{
    return m_technique ? m_technique->get_pass(pass_idx).find_material_uniform_def_idx_by_name(name) : -1;
}

inline size_t Material::get_sampler_count(size_t pass_idx) const
{
    return m_passes[pass_idx].sampler_count;
}
inline int Material::find_sampler_idx_by_name(size_t pass_idx, std::string const& name) const
{
    return m_technique ? m_technique->get_pass(pass_idx).find_material_sampler_def_idx_by_name(name) : -1;
}
inline Sampler const& Material::get_sampler(size_t pass_idx, size_t sampler_idx) const
{
    return m_samplers[m_passes[pass_idx].sampler_start_idx + sampler_idx];
}
inline std::string Material::get_sampler_name(size_t pass_idx, size_t sampler_idx) const
{
    return m_technique ? m_technique->get_pass(pass_idx).get_material_sampler_def(sampler_idx).get_name() : std::string();
}

inline Render_State const& Material::get_render_state(size_t pass_idx) const
{
    return m_passes[pass_idx].render_state;
}
inline Render_State&	Material::get_render_state(size_t pass_idx)
{
    return m_passes[pass_idx].render_state;
}
inline void Material::set_render_state(size_t pass_idx, Render_State const& rs)
{
    m_passes[pass_idx].render_state = rs;
}

inline bool Material::is_valid() const
{
    return m_technique && m_technique->is_valid() && !m_passes.empty();
}

}
}

namespace std 
{
template <>
struct hash<q::video::Material>
{
    auto operator()(q::video::Material const& m) const -> size_t
    {
        size_t h = (size_t)m.get_technique().get();
        size_t pc = m.get_pass_count();
        q::util::hash_combine(h, pc);
        //TODO - finish this - add uniforms and samplers
        for (size_t i = 0; i < pc; i++)
        {
            q::util::hash_combine(h, m.get_render_state(i));
        }
        return h & 0x7FFFFFFF;
    }
};
}

