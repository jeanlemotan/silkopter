#include "types/String_Type.h"
#include "values/String_Value.h"

namespace ts
{

String_Type::String_Type(std::string const& name)
    : Symbol_Impl(name)
{

}

auto String_Type::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new String_Type(name));
}

auto String_Type::get_template_instantiation_string() const -> std::string const&
{
    return get_name();
}

auto String_Type::get_default_value() const -> std::shared_ptr<const IValue>
{
    return nullptr;
}

auto String_Type::create_value() const -> std::unique_ptr<IValue>
{
    return nullptr;
}

auto String_Type::get_specialized_default_value() const -> std::shared_ptr<const value_type>
{
    return nullptr;
}

auto String_Type::create_specialized_value() const -> std::unique_ptr<value_type>
{
    return nullptr;
}


}
