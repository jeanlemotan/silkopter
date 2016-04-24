#include "def_lang/impl/Vector_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"

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
        auto result = get_value(i)->is_equal(*v->get_value(i));
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
        auto result = get_value(i)->copy_assign(*v->get_value(i));
        if (result != success)
        {
            return result;
        }
    }

    //insert missing values
    while (get_value_count() < v->get_value_count())
    {
        size_t idx = get_value_count();
        auto result = insert_value(idx, v->get_value(idx)->clone());
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

std::shared_ptr<const IValue> Vector_Value::select(Value_Selector&& selector) const
{
    return const_cast<Vector_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Vector_Value::select(Value_Selector&& selector)
{
    TS_ASSERT(!selector.empty());
    if (selector.empty())
    {
        return nullptr;
    }

    std::shared_ptr<IValue> value;
    Value_Selector::Element const& element = selector.front();
    if (element.type == Value_Selector::Element::Type::INDEX)
    {
        value = element.index < get_value_count() ? get_value(element.index) : std::shared_ptr<IValue>();
    }

    if (!value)
    {
        return nullptr;
    }

    selector.pop_front();
    if (selector.empty())
    {
        return value;
    }

    return value->select(std::move(selector));
}

std::shared_ptr<IVector_Type const> Vector_Value::get_specialized_type() const
{
    return m_type;
}

Result<serialization::Value> Vector_Value::serialize() const
{
    serialization::Value svalue(serialization::Value::Type::ARRAY);

    for (size_t i = 0; i < get_value_count(); i++)
    {
        auto result = get_value(i)->serialize();
        if (result != success)
        {
            return result;
        }
        svalue.add_array_element(result.extract_payload());
    }

    return std::move(svalue);
}

Result<void> Vector_Value::deserialize(serialization::Value const& sz_value)
{
    if (sz_value.get_type() != serialization::Value::Type::ARRAY)
    {
        return Error("Expected array value when deserializing");
    }

    while (get_value_count() > sz_value.get_array_element_count())
    {
        erase_value(get_value_count() - 1);
    }
    while (get_value_count() < sz_value.get_array_element_count())
    {
        insert_default_value(get_value_count());
    }

    for (size_t i = 0; i < sz_value.get_array_element_count(); i++)
    {
        auto result = get_value(i)->deserialize(sz_value.get_array_element_value(i));
        if (result != ts::success)
        {
            return result;
        }
    }

    return success;
}

Result<void> Vector_Value::insert_default_value(size_t idx)
{
    std::shared_ptr<IValue> value = get_specialized_type()->get_inner_type()->create_value();
    return insert_value(idx, value);
}

Result<void> Vector_Value::insert_value(size_t idx, std::shared_ptr<IValue> value)
{
    if (!value)
    {
        return Error("Cannot insert null value");
    }

    if (idx > get_value_count())
    {
        return Error("Cannot insert beyond the end");
    }
    if (value->get_type() != get_specialized_type()->get_inner_type())
    {
        return Error("Cannot insert value of type '" + value->get_type()->get_name() + "'. Expected values of type '" + get_specialized_type()->get_inner_type()->get_name() + "'");
    }

    m_values.insert(m_values.begin() + idx, std::move(value));

    return success;
}
Result<void> Vector_Value::erase_value(size_t idx)
{
    if (idx >= get_value_count())
    {
        return Error("Cannot erase beyond the end");
    }

    m_values.erase(m_values.begin() + idx);

    return success;
}

void Vector_Value::clear()
{
    m_values.clear();
}

size_t Vector_Value::get_value_count() const
{
    return m_values.size();
}

std::shared_ptr<const IValue> Vector_Value::get_value(size_t idx) const
{
    return m_values[idx];
}
std::shared_ptr<IValue> Vector_Value::get_value(size_t idx)
{
    return m_values[idx];
}


}
