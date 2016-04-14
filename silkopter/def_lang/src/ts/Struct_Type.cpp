#include "types/Struct_Type.h"
#include "values/IValue.h"
#include "values/Struct_Value.h"
#include "attributes/IAttribute.h"

namespace ts
{

Struct_Type::Struct_Type(std::string const& name)
    : Symbol_EP(name)
{
}

auto Struct_Type::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

auto Struct_Type::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Struct_Type(*this));
}

auto Struct_Type::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

auto Struct_Type::create_value() const -> std::unique_ptr<IValue>
{
    return create_specialized_value();
}

auto Struct_Type::create_specialized_value() const -> std::unique_ptr<IStruct_Value>
{
    return std::unique_ptr<IStruct_Value>(new Struct_Value(*this));
}


}
