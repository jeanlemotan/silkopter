#include "def_lang/impl/Enum_Item.h"
#include "def_lang/ILiteral_Attribute.h"
#include "def_lang/IName_Attribute.h"
#include "def_lang/IString_Value.h"

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
    if (attribute.get_name() == "ui_name")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IString_Value const* value = dynamic_cast<IString_Value const*>(&att->get_value());
            if (!value)
            {
                return Error("Attribute '" + attribute.get_name() + "' has to be a string.");
            }
            m_ui_name = value->get_value();
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a string literal.");
        }
    }
    else if (attribute.get_name() == "default")
    {
        if (IName_Attribute const* att = dynamic_cast<IName_Attribute const*>(&attribute))
        {
            m_is_default = true;
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a named attribute.");
        }
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }

    return success;
}

}
