#include "impl/Struct_Value.h"
#include "impl/Member.h"

namespace ts
{

Struct_Value::Struct_Value(std::shared_ptr<IStruct_Type const> type)
    : m_type(type)
{
    for (size_t i = 0; i < type->get_member_def_count(); i++)
    {
        auto result = add_member(std::make_shared<Member>(type->get_member_def(i)));
        TS_ASSERT(result == success);
    }
}

Result<bool> Struct_Value::is_equal(IValue const& other) const
{
    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
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

Result<void> Struct_Value::copy_assign(IValue const& other)
{
    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
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
Result<void> Struct_Value::copy_assign(IInitializer const& initializer)
{
    return Error("not implemented");
}

std::shared_ptr<IValue> Struct_Value::clone() const
{
    return std::make_shared<Struct_Value>(*this);
}

std::shared_ptr<IType const> Struct_Value::get_type() const
{
    return m_type;
}

Result<void> Struct_Value::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}
Result<std::string> Struct_Value::get_ui_string() const
{
    return Error("Not Supported");
}

IValue const* Struct_Value::select(Value_Selector const& selector) const
{
    return nullptr;
}
IValue* Struct_Value::select(Value_Selector const& selector)
{
    return nullptr;
}

std::shared_ptr<IStruct_Type const> Struct_Value::get_specialized_type() const
{
    return m_type;
}


}
