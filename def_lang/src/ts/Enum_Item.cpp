#include "def_lang/impl/Enum_Item.h"
#include "def_lang/impl/UI_Name_Attribute.h"
#include "def_lang/impl/Default_Attribute.h"

namespace ts
{

Enum_Item::Enum_Item(std::string const& name, int64_t integral_value)
    : Symbol_EP(name)
    , m_ui_name(name)
    , m_integral_value(integral_value)
{
}

int64_t Enum_Item::get_integral_value() const
{
    return m_integral_value;
}

std::string const& Enum_Item::get_ui_name() const
{
    return m_ui_name;
}

bool Enum_Item::is_default() const
{
    return m_is_default;
}

Result<void> Enum_Item::validate_attribute(IAttribute const& attribute)
{
    if (UI_Name_Attribute const* att = dynamic_cast<UI_Name_Attribute const*>(&attribute))
    {
        m_ui_name = att->get_ui_name();
    }
    else if (Default_Attribute const* att = dynamic_cast<Default_Attribute const*>(&attribute))
    {
        m_is_default = true;
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }

    return success;
}

}
