#include "types/Vector_Type.h"
#include "values/Vector_Value.h"

namespace ts
{

Vector_Type::Vector_Type(std::string const& name)
    : Symbol_Impl(name)
{

}

auto Vector_Type::init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) -> bool
{
    if (arguments.size() != 1)
    {
        return false;
    }

    m_inner_type = std::dynamic_pointer_cast<const IType>(arguments[0]);
    if (!m_inner_type)
    {
        return false;
    }

    return true;
}

auto Vector_Type::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Vector_Type(name));
}

auto Vector_Type::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

auto Vector_Type::get_default_value() const -> std::shared_ptr<const IValue>
{
    return nullptr;
}

auto Vector_Type::create_value() const -> std::unique_ptr<IValue>
{
    return nullptr;
}

auto Vector_Type::get_specialized_default_value() const -> std::shared_ptr<const value_type>
{
    return nullptr;
}

auto Vector_Type::create_specialized_value() const -> std::unique_ptr<value_type>
{
    return nullptr;
}


}
