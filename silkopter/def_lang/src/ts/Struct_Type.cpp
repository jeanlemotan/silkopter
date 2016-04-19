#include "impl/Struct_Type.h"
#include "IValue.h"
#include "impl/Struct_Value.h"
#include "IAttribute.h"

namespace ts
{

Struct_Type::Struct_Type(std::string const& name)
    : Symbol_EP(name)
{
}

Result<void> Struct_Type::validate_attribute(IAttribute const& attribute)
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

std::shared_ptr<IType> Struct_Type::clone(std::string const& name) const
{
    return std::shared_ptr<IType>(new Struct_Type(*this));
}

std::string Struct_Type::get_template_instantiation_string() const
{
    return get_name();
}

std::shared_ptr<IValue> Struct_Type::create_value() const
{
    return create_specialized_value();
}

std::shared_ptr<IStruct_Value> Struct_Type::create_specialized_value() const
{
    return std::make_shared<Struct_Value>(this->shared_from_this());
}


}
