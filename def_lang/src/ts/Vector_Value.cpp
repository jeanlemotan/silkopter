#include "def_lang/impl/Vector_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/impl/Initializer_List.h"
#include "def_lang/Qualified_Type.h"

namespace ts
{

Vector_Value::Vector_Value(std::shared_ptr<IVector_Type const> type)
    : m_type(type)
{
}

Vector_Value::~Vector_Value()
{
    clear();
}

bool Vector_Value::is_constructed() const
{
    return m_is_constructed;
}

Result<bool> Vector_Value::is_equal(IValue const& other) const
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

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

Result<void> Vector_Value::construct(IInitializer_List const& initializer_list)
{
    if (is_constructed())
    {
        TS_ASSERT(false);
        return Error("Already constructed value");
    }
    if (initializer_list.get_initializer_count() != 0)
    {
        return Error("Not supported");
    }

    m_is_constructed = true;
    return success;
}
Result<void> Vector_Value::copy_construct(IValue const& other)
{
    auto result = construct();
    if (result != success)
    {
        return result;
    }
    return copy_assign(other);
}

Result<void> Vector_Value::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

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
        std::shared_ptr<IValue> value = v->get_value(idx)->get_type()->create_value();
        auto result = value->copy_construct(*v->get_value(idx));
        if (result != success)
        {
            return result;
        }

        result = insert_value(idx, value);
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
Result<void> Vector_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("not implemented");
}

std::shared_ptr<IType const> Vector_Value::get_type() const
{
    return m_type;
}

Result<void> Vector_Value::parse_from_ui_string(std::string const& str)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}
Result<std::string> Vector_Value::get_ui_string() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return std::to_string(get_value_count()) + " elements";
}

std::shared_ptr<const IValue> Vector_Value::select(Value_Selector&& selector) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return const_cast<Vector_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Vector_Value::select(Value_Selector&& selector)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
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

Result<sz::Value> Vector_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    sz::Value svalue(sz::Value::Type::ARRAY);

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

Result<void> Vector_Value::deserialize(sz::Value const& sz_value)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (!sz_value.is_array())
    {
        return Error("Expected array value when deserializing");
    }

    while (get_value_count() > sz_value.get_array_element_count())
    {
        auto result = erase_value(get_value_count() - 1);
        TS_ASSERT(result == success);
    }
    while (get_value_count() < sz_value.get_array_element_count())
    {
        auto result = insert_default_value(get_value_count());
        TS_ASSERT(result == success);
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

Result<std::shared_ptr<IValue>> Vector_Value::insert_default_value(size_t idx)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    std::shared_ptr<IValue> value = get_specialized_type()->get_inner_type()->create_value();
    auto construct_result = value->construct();
    if (construct_result != success)
    {
        return construct_result.error();
    }
    auto insert_result = insert_value(idx, value);
    if (insert_result != success)
    {
        return insert_result.error();
    }
    return value;
}

Result<void> Vector_Value::insert_value(size_t idx, std::shared_ptr<IValue> value)
{
    if (!value)
    {
        return Error("Cannot insert null value");
    }
    if (!is_constructed() || !value->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (idx > get_value_count())
    {
        return Error("Cannot insert beyond the end");
    }
    if (value->get_type() != get_specialized_type()->get_inner_type())
    {
        return Error("Cannot insert value of type '" + value->get_type()->get_name() + "'. Expected values of type '" + get_specialized_type()->get_inner_type()->get_name() + "'");
    }


    boost::signals2::connection connection = value->sig_value_changed.connect([this]
    {
        sig_value_changed();
    });

    m_values.insert(m_values.begin() + idx, std::move(value));
    m_value_changed_connections.insert(m_value_changed_connections.begin() + idx, connection);

    sig_value_changed();

    return success;
}
Result<void> Vector_Value::erase_value(size_t idx)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (idx >= get_value_count())
    {
        return Error("Cannot erase beyond the end");
    }

    m_values.erase(m_values.begin() + idx);

    m_value_changed_connections[idx].disconnect();
    m_value_changed_connections.erase(m_value_changed_connections.begin() + idx);

    sig_value_changed();

    return success;
}

void Vector_Value::clear()
{
    TS_ASSERT(is_constructed());
    if (!m_values.empty())
    {
        m_values.clear();
        for (boost::signals2::connection& c: m_value_changed_connections)
        {
            c.disconnect();
        }
        m_value_changed_connections.clear();
        sig_value_changed();
    }
}

size_t Vector_Value::get_value_count() const
{
    TS_ASSERT(is_constructed());
    return m_values.size();
}

std::shared_ptr<const IValue> Vector_Value::get_value(size_t idx) const
{
    TS_ASSERT(is_constructed());
    return m_values[idx];
}
std::shared_ptr<IValue> Vector_Value::get_value(size_t idx)
{
    TS_ASSERT(is_constructed());
    return m_values[idx];
}


}
