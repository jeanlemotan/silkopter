#include "def_lang/Qualified_Type.h"
#include "def_lang/impl/Bool_Type.h"
#include "def_lang/impl/Bool_Value.h"

namespace ts
{

Qualified_Type::Qualified_Type(std::shared_ptr<const IType> type, bool is_const)
    : m_type(type)
    , m_is_const(is_const)
{
}

std::shared_ptr<const IType> Qualified_Type::get_type() const
{
    return m_type;
}
bool Qualified_Type::is_const() const
{
    return m_is_const;
}

std::string Qualified_Type::get_template_instantiation_string() const
{
    if (m_is_const)
    {
        return "const " + m_type->get_symbol_path().to_string();
    }
    else
    {
        return m_type->get_symbol_path().to_string();
    }
}


}
