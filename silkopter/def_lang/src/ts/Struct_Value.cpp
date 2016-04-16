#include "values/Struct_Value.h"
#include "Member.h"

namespace ts
{

Struct_Value::Struct_Value(IStruct_Type const& type)
    : m_type(type)
{
    for (size_t i = 0; i < type.get_member_def_count(); i++)
    {
        auto result = add_member(std::unique_ptr<Member>(new Member(type.get_member_def(i))));
        TS_ASSERT(result == success);
    }
}

auto Struct_Value::is_equal(IValue const& other) const -> Result<bool>
{
    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (&get_type() != &v->get_type())
    {
        return Error("incompatible types");
    }

    TS_ASSERT(get_member_count() == v->get_member_count());

    for (size_t i = 0; i < get_member_count(); i++)
    {
        auto result = get_member(i).get_value().is_equal(v->get_member(i).get_value());
        if (result != success)
        {
            return result;
        }
        if (result.payload() == false)
        {
            return false;
        }
    }

    return true;
}

auto Struct_Value::copy_assign(IValue const& other) -> Result<void>
{
    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (&get_type() != &v->get_type())
    {
        return Error("incompatible types");
    }

    TS_ASSERT(get_member_count() == v->get_member_count());

    for (size_t i = 0; i < get_member_count(); i++)
    {
        auto result = get_member(i).get_value().copy_assign(v->get_member(i).get_value());
        if (result != success)
        {
            return result;
        }
    }

    return success;
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
    return Error("Not Supported");
}
auto Struct_Value::get_ui_string() const -> Result<std::string>
{
    return Error("Not Supported");
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
