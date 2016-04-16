#include "types/Vector_Type.h"
#include "values/Vector_Value.h"
#include "attributes/IAttribute.h"

namespace ts
{

Vector_Type::Vector_Type(std::string const& name)
    : Symbol_EP(name)
{

}

auto Vector_Type::init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) -> Result<void>
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

auto Vector_Type::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

auto Vector_Type::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Vector_Type(*this));
}

auto Vector_Type::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

auto Vector_Type::create_value() const -> std::unique_ptr<IValue>
{
    return create_specialized_value();
}

auto Vector_Type::create_specialized_value() const -> std::unique_ptr<value_type>
{
    return std::unique_ptr<IVector_Value>(new Vector_Value(*this));
}


}
