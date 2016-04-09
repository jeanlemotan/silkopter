#include "types/Struct_Type.h"
#include "values/IValue.h"
#include "values/IStruct_Value.h"

namespace ts
{

Struct_Type::Struct_Type(std::string const& name)
    : Symbol_Impl(name)
{
}

auto Struct_Type::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Struct_Type(name));
}

auto Struct_Type::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

auto Struct_Type::get_default_value() const -> std::shared_ptr<const IValue>
{
    return nullptr;
}
auto Struct_Type::create_value() const -> std::unique_ptr<IValue>
{
    return nullptr;
}

auto Struct_Type::get_specialized_default_value() const -> std::shared_ptr<const IStruct_Value>
{
    return nullptr;
}
auto Struct_Type::create_specialized_value() const -> std::unique_ptr<IStruct_Value>
{
    return nullptr;
}


}
