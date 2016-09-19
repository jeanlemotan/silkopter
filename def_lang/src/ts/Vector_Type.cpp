#include "def_lang/impl/Vector_Type.h"
#include "def_lang/impl/Vector_Value.h"
#include "def_lang/IAttribute.h"
#include "def_lang/ILiteral_Attribute.h"
#include "def_lang/IString_Value.h"
#include "def_lang/Qualified_Type.h"

namespace ts
{

Vector_Type::Vector_Type(std::string const& name)
    : Symbol_EP(name)
    , m_ui_name(name)
{
}

Vector_Type::Vector_Type(Vector_Type const& other, std::string const& name)
    : Symbol_EP(other, name)
    , Attribute_Container_EP(other)
    , m_inner_type(other.m_inner_type)
    , m_ui_name(name)
    , m_native_type(other.m_ui_name)
{
}

Result<void> Vector_Type::init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments)
{
    if (arguments.size() != 1)
    {
        return Error("Expected only one template argument, got " + std::to_string(arguments.size()));
    }

    std::shared_ptr<const Qualified_Type> inner_type = std::dynamic_pointer_cast<const Qualified_Type>(arguments[0]);
    if (!inner_type)
    {
        return Error("Invalid template argument. Expected type");
    }

    if (inner_type->is_const())
    {
        return Error("Invalid template argument. vectors don't support const types");
    }

    m_inner_type = inner_type->get_type();

    return success;
}

std::string const& Vector_Type::get_ui_name() const
{
    return m_ui_name;
}
Symbol_Path Vector_Type::get_native_type() const
{
    return m_native_type;
}

Result<void> Vector_Type::validate_attribute(IAttribute const& attribute)
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
    else if (attribute.get_name() == "native_type")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IString_Value const* value = dynamic_cast<IString_Value const*>(&att->get_value());
            if (!value)
            {
                return Error("Attribute '" + attribute.get_name() + "' has to be a string.");
            }
            m_native_type = value->get_value();
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a string literal.");
        }
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }
}

std::shared_ptr<IType> Vector_Type::clone(std::string const& name) const
{
    return std::make_shared<Vector_Type>(*this, name);
}
std::shared_ptr<IType> Vector_Type::alias(std::string const& name) const
{
    std::shared_ptr<Vector_Type> alias = std::make_shared<Vector_Type>(*this, name);
    alias->m_native_type = Symbol_Path(); //clear the native type as this is an alias
    alias->m_aliased_type = this->shared_from_this();
    return alias;
}
std::shared_ptr<const IType> Vector_Type::get_aliased_type() const
{
    return m_aliased_type;
}

std::shared_ptr<const IType> Vector_Type::get_inner_type() const
{
    return m_inner_type;
}

std::shared_ptr<IValue> Vector_Type::create_value() const
{
    return create_specialized_value();
}

std::shared_ptr<Vector_Type::value_type> Vector_Type::create_specialized_value() const
{
    return std::make_shared<Vector_Value>(shared_from_this());
}

}
