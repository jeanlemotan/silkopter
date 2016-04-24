#include "def_lang/impl/Ptr_Type.h"
#include "def_lang/impl/Ptr_Value.h"
#include "def_lang/IAttribute.h"

namespace ts
{

Ptr_Type::Ptr_Type(std::string const& name)
    : Symbol_EP(name)
{

}

Result<void> Ptr_Type::init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments)
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

    return success;
}

Result<void> Ptr_Type::validate_attribute(IAttribute const& attribute)
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

std::shared_ptr<IType> Ptr_Type::clone(std::string const& name) const
{
    return std::shared_ptr<IType>(new Ptr_Type(*this));
}

std::string Ptr_Type::get_template_instantiation_string() const
{
    return get_name();
}

std::shared_ptr<const IType> Ptr_Type::get_inner_type() const
{
    return m_inner_type;
}

std::shared_ptr<IValue> Ptr_Type::create_value() const
{
    return create_specialized_value();
}

std::shared_ptr<Ptr_Type::value_type> Ptr_Type::create_specialized_value() const
{
    return std::make_shared<Ptr_Value>(shared_from_this());
}


}
