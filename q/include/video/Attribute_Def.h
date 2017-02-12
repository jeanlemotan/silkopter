#pragma once

#include "video/Vertex_Declaration.h"

namespace q
{
namespace video
{

class Attribute_Def
{
public:
    Attribute_Def() = default;
    Attribute_Def(std::string const& name);
    Attribute_Def(std::string const& name, std::string const& human_name);

    auto get_human_name() const -> std::string;
    auto get_name() const -> std::string;

    void set_component_count(size_t count);
    auto get_component_count() const -> size_t;

    void set_semantic(Vertex_Declaration::Semantic s);
    auto get_semantic() const -> Vertex_Declaration::Semantic;

    void set_attribute_name(std::string const& name);
    auto get_attribute_name() const -> std::string;

    void set_enabled(bool yes);
    bool is_enabled() const;

private:
    std::string		m_name;
    std::string		m_human_name;
    std::string		m_atribute_name;
    Vertex_Declaration::Semantic m_semantic = Vertex_Declaration::Semantic::USER;
    size_t		m_component_count = 0;
    bool		m_is_enabled = false;
};

inline Attribute_Def::Attribute_Def(std::string const& name)
    : m_name(name)
    , m_human_name(name)
{
    ;
}
inline Attribute_Def::Attribute_Def(std::string const& name, std::string const& human_name)
    : m_name(name)
    , m_human_name(human_name)
{
    ;
}

inline void Attribute_Def::set_component_count(size_t count)
{
    QASSERT(count > 0 && count <= 4);
    m_component_count = count;
}
inline auto Attribute_Def::get_component_count() const -> size_t
{
    return m_component_count;
}

inline auto Attribute_Def::get_human_name() const -> std::string
{
    return m_human_name;
}
inline auto Attribute_Def::get_name() const -> std::string
{
    return m_name;
}
inline void Attribute_Def::set_attribute_name(std::string const& name)
{
    size_t ccount = 0;
    m_atribute_name = name;
    if (name == "positions")
    {
        set_semantic(Vertex_Declaration::Semantic::POSITIONS);
        ccount = 4;
    }
    else if (name == "normals")
    {
        set_semantic(Vertex_Declaration::Semantic::NORMALS);
        ccount = 4;
    }
    else if (name == "tangents")
    {
        set_semantic(Vertex_Declaration::Semantic::TANGENTS);
        ccount = 4;
    }
    else if (name == "colors")
    {
        set_semantic(Vertex_Declaration::Semantic::COLORS);
        ccount = 4;
    }
    else if (name == "tex_coords_0")
    {
        set_semantic(Vertex_Declaration::Semantic::TEX_COORDS0);
        ccount = 2;
    }
    else if (name == "tex_coords_1")
    {
        set_semantic(Vertex_Declaration::Semantic::TEX_COORDS1);
        ccount = 2;
    }
    else if (name == "tex_coords_2")
    {
        set_semantic(Vertex_Declaration::Semantic::TEX_COORDS2);
        ccount = 2;
    }
    else if (name == "tex_coords_3")
    {
        set_semantic(Vertex_Declaration::Semantic::TEX_COORDS3);
        ccount = 2;
    }
    else
    {
        set_semantic(Vertex_Declaration::Semantic::USER);
    }

    m_component_count = m_component_count == 0 ? ccount : m_component_count;
}
inline auto Attribute_Def::get_attribute_name() const -> std::string
{
    return m_atribute_name;
}
inline void Attribute_Def::set_semantic(Vertex_Declaration::Semantic s)
{
    m_semantic = s;
}
inline auto Attribute_Def::get_semantic() const -> Vertex_Declaration::Semantic
{
    return m_semantic;
}
inline void Attribute_Def::set_enabled(bool yes)
{
    m_is_enabled = yes;
}
inline bool Attribute_Def::is_enabled() const
{
    return m_is_enabled;
}

}
}
