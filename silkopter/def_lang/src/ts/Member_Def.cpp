#include "Member_Def.h"
#include "values/IValue.h"

namespace ts
{

Member_Def::Member_Def(std::string const& name, IType const& type, std::unique_ptr<const IValue> default_value)
    : Symbol_EP(name)
    , m_type(type)
    , m_default_value(std::move(default_value))
{

}

Member_Def::~Member_Def()
{

}

auto Member_Def::get_type() const -> IType const&
{
    return m_type;
}
auto Member_Def::get_default_value() const -> IValue const&
{
    return *m_default_value;
}

auto Member_Def::get_ui_name() const -> std::string const&
{
    return m_ui_name.empty() ? get_name() : m_ui_name;
}

auto Member_Def::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    if (UI_Name_Attribute const* att = dynamic_cast<UI_Name_Attribute const*>(&attribute))
    {
        m_ui_name = att->get_ui_name();
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }

    return success;
}

}
