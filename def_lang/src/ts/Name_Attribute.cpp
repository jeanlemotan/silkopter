#include "def_lang/impl/Name_Attribute.h"

namespace ts
{

Name_Attribute::Name_Attribute(std::string const& name)
    : m_name(name)
{
}
Name_Attribute::~Name_Attribute()
{
}
std::string Name_Attribute::get_name() const
{
    return m_name;
}

}
