#include "values/Struct_Value.h"

namespace ts
{

Struct_Value::Struct_Value(IStruct_Type const& type)
    : m_type(type)
{

}

auto Struct_Value::is_equal(IValue const& other) const -> Result<bool>
{
    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible types");
    }

    return Error("not implemented");
}

auto Struct_Value::copy_assign(IValue const& other) -> Result<void>
{
    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible types");
    }

    return Error("not implemented");
}
auto Struct_Value::copy_assign(IInitializer const& initializer) -> Result<void>
{
    return Error("not implemented");
}

auto Struct_Value::clone() const -> std::unique_ptr<IValue>
{
    return std::unique_ptr<IValue>(new Struct_Value(*this));
}

auto Struct_Value::get_type() const -> IType const&
{
    return m_type;
}

auto Struct_Value::parse_from_ui_string(std::string const& str) -> Result<void>
{
    return Error("not supported");
}
auto Struct_Value::get_ui_string() const -> std::string
{
    return std::string();
}

auto Struct_Value::select(Value_Selector const& selector) const -> IValue const*
{
    return nullptr;
}
auto Struct_Value::select(Value_Selector const& selector) -> IValue*
{
    return nullptr;
}

auto Struct_Value::get_specialized_type() const -> IStruct_Type const&
{
    return m_type;
}


}
