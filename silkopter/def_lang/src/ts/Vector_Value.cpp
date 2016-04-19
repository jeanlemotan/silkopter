#include "impl/Vector_Value.h"

namespace ts
{

Vector_Value::Vector_Value(std::shared_ptr<IVector_Type const> type)
    : m_type(type)
{
}

Result<bool> Vector_Value::is_equal(IValue const& other) const
{
    IVector_Value const* v = dynamic_cast<const IVector_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (get_value_count() != v->get_value_count())
    {
        return false;
    }

    for (size_t i = 0; i < get_value_count(); i++)
    {
        auto result = get_value(i).is_equal(v->get_value(i));
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

Result<void> Vector_Value::copy_assign(IValue const& other)
{
    IVector_Value const* v = dynamic_cast<const IVector_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    size_t common_size = std::min(get_value_count(), v->get_value_count());
    for (size_t i = 0; i < common_size; i++)
    {
        auto result = get_value(i).copy_assign(v->get_value(i));
        if (result != success)
        {
            return result;
        }
    }

    //insert missing values
    while (get_value_count() < v->get_value_count())
    {
        size_t idx = get_value_count();
        auto result = insert_value(idx, v->get_value(idx).clone());
        if (result != success)
        {
            return result;
        }
    }

    //remove extra values
    while (get_value_count() > v->get_value_count())
    {
        size_t idx = get_value_count() - 1;
        auto result = erase_value(idx);
        if (result != success)
        {
            return result;
        }
    }

    return success;
}
Result<void> Vector_Value::copy_assign(IInitializer const& initializer)
{
    return Error("not implemented");
}

std::shared_ptr<IValue> Vector_Value::clone() const
{
    return std::make_shared<Vector_Value>(*this);
}

std::shared_ptr<IType const> Vector_Value::get_type() const
{
    return m_type;
}

Result<void> Vector_Value::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}
Result<std::string> Vector_Value::get_ui_string() const
{
    return Error("Not Supported");
}

IValue const* Vector_Value::select(Value_Selector const& selector) const
{
    return nullptr;
}
IValue* Vector_Value::select(Value_Selector const& selector)
{
    return nullptr;
}

std::shared_ptr<IVector_Type const> Vector_Value::get_specialized_type() const
{
    return m_type;
}


}
