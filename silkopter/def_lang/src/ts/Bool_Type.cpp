#include "types/Bool_Type.h"
#include "values/Bool_Value.h"

namespace ts
{

Bool_Type::Bool_Type(std::string const& name)
    : Symbol_Impl(name)
{

}

auto Bool_Type::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Bool_Type(name));
}

auto Bool_Type::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

auto Bool_Type::get_default_value() const -> std::shared_ptr<const IValue>
{
    return nullptr;
}

auto Bool_Type::create_value() const -> std::unique_ptr<IValue>
{
    return nullptr;
}

auto Bool_Type::get_specialized_default_value() const -> std::shared_ptr<const value_type>
{
    return nullptr;
}

auto Bool_Type::create_specialized_value() const -> std::unique_ptr<value_type>
{
    return nullptr;
}


}
