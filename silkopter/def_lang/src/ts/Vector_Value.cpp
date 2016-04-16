#include "impl/Vector_Value.h"

namespace ts
{

Vector_Value::Vector_Value(IVector_Type const& type)
    : m_type(type)
{
}

auto Vector_Value::is_equal(IValue const& other) const -> Result<bool>
{
    IVector_Value const* v = dynamic_cast<const IVector_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (&get_type() != &v->get_type())
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

auto Vector_Value::copy_assign(IValue const& other) -> Result<void>
{
    IVector_Value const* v = dynamic_cast<const IVector_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (&get_type() != &v->get_type())
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
auto Vector_Value::copy_assign(IInitializer const& initializer) -> Result<void>
{
    return Error("not implemented");
}

auto Vector_Value::clone() const -> std::unique_ptr<IValue>
{
    return std::unique_ptr<IValue>(new Vector_Value(*this));
}

auto Vector_Value::get_type() const -> IType const&
{
    return m_type;
}

auto Vector_Value::parse_from_ui_string(std::string const& str) -> Result<void>
{
    return Error("Not Supported");
}
auto Vector_Value::get_ui_string() const -> Result<std::string>
{
    return Error("Not Supported");
}

auto Vector_Value::select(Value_Selector const& selector) const -> IValue const*
{
    return nullptr;
}
auto Vector_Value::select(Value_Selector const& selector) -> IValue*
{
    return nullptr;
}

auto Vector_Value::get_specialized_type() const -> IVector_Type const&
{
    return m_type;
}


}
