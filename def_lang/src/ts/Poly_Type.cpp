#include "def_lang/impl/Poly_Type.h"
#include "def_lang/impl/Poly_Value.h"
#include "def_lang/IAttribute.h"
#include "def_lang/impl/UI_Name_Attribute.h"
#include "def_lang/impl/Native_Type_Attribute.h"

namespace ts
{

Poly_Type::Poly_Type(std::string const& name)
    : Symbol_EP(name)
    , m_ui_name(name)
{
}

Poly_Type::Poly_Type(Poly_Type const& other, std::string const& name)
    : Symbol_EP(other, name)
    , Attribute_Container_EP(other)
    , m_inner_type(other.m_inner_type)
    , m_ui_name(name)
    , m_native_type(other.m_ui_name)
{
}

Result<void> Poly_Type::init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments)
{
    if (arguments.size() != 1)
    {
        return Error("Expected only one template argument, got " + std::to_string(arguments.size()));
    }

    m_inner_type = std::dynamic_pointer_cast<const IType>(arguments[0]);
    if (!m_inner_type)
    {
        return Error("Invalid template argument. Expected type");
    }

    if (m_native_type.empty())
    {
        m_native_type = Symbol_Path("std::shared_ptr<" + m_inner_type->get_native_type().to_string() + ">");
    }

    return success;
}

std::string const& Poly_Type::get_ui_name() const
{
    return m_ui_name;
}

Symbol_Path Poly_Type::get_native_type() const
{
    return m_native_type.empty() ? get_symbol_path() : m_native_type;
}

Result<void> Poly_Type::validate_attribute(IAttribute const& attribute)
{
    if (UI_Name_Attribute const* att = dynamic_cast<UI_Name_Attribute const*>(&attribute))
    {
        m_ui_name = att->get_ui_name();
        return success;
    }
    else if (Native_Type_Attribute const* att = dynamic_cast<Native_Type_Attribute const*>(&attribute))
    {
        m_native_type = att->get_native_type();
        return success;
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }
}

std::shared_ptr<IType> Poly_Type::clone(std::string const& name) const
{
    return std::make_shared<Poly_Type>(*this, name);
}

std::string Poly_Type::get_template_instantiation_string() const
{
    return get_symbol_path().to_string();
}

std::shared_ptr<const IType> Poly_Type::get_inner_type() const
{
    return m_inner_type;
}

std::shared_ptr<IValue> Poly_Type::create_value() const
{
    return create_specialized_value();
}

std::shared_ptr<Poly_Type::value_type> Poly_Type::create_specialized_value() const
{
    return std::make_shared<Poly_Value>(shared_from_this());
}


}
