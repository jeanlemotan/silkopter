#include "def_lang/impl/Ptr_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"

namespace ts
{

Ptr_Value::Ptr_Value(std::shared_ptr<IPtr_Type const> type)
    : m_type(type)
{
}

Result<bool> Ptr_Value::is_equal(IValue const& other) const
{
    IPtr_Value const* v = dynamic_cast<const IPtr_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (get_specialized_type()->get_inner_type() != v->get_specialized_type()->get_inner_type())
    {
        return Error("incompatible inner types");
    }

    //if both are null, then they are equal
    if (get_value() == nullptr && v->get_value() == nullptr)
    {
        return true;
    }

    //if one is null and the other not null, the values are different
    if ((get_value() == nullptr) != (v->get_value() == nullptr))
    {
        return true;
    }

    return get_value()->is_equal(*v->get_value());
}

Result<void> Ptr_Value::copy_assign(IValue const& other)
{
    IPtr_Value const* v = dynamic_cast<const IPtr_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (get_specialized_type()->get_inner_type() != v->get_specialized_type()->get_inner_type())
    {
        return Error("incompatible inner types");
    }

    if (get_value() == nullptr)
    {
        if (v->get_value() == nullptr)
        {
            return success;
        }
        return set_value(v->get_value()->clone());
    }
    else
    {
        if (v->get_value() == nullptr)
        {
            return set_value(nullptr);
        }
        return get_value()->copy_assign(*v->get_value());
    }
}
Result<void> Ptr_Value::copy_assign(IInitializer const& initializer)
{
    return Error("not implemented");
}

std::shared_ptr<IValue> Ptr_Value::clone() const
{
    return std::make_shared<Ptr_Value>(*this);
}

std::shared_ptr<IType const> Ptr_Value::get_type() const
{
    return m_type;
}

Result<void> Ptr_Value::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}
Result<std::string> Ptr_Value::get_ui_string() const
{
    return Error("Not Supported");
}

std::shared_ptr<const IValue> Ptr_Value::select(Value_Selector&& selector) const
{
    return const_cast<Ptr_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Ptr_Value::select(Value_Selector&& selector)
{
    TS_ASSERT(!selector.empty());
    if (selector.empty())
    {
        return nullptr;
    }

    Value_Selector::Element const& element = selector.front();
    if (element.type == Value_Selector::Element::Type::DEREFERENCE)
    {
        selector.pop_front();
        if (selector.empty())
        {
            return get_value();
        }
        return nullptr;
    }
    else
    {
        selector.pop_front();
        return get_value()->select(std::move(selector));
    }
}

std::shared_ptr<IPtr_Type const> Ptr_Value::get_specialized_type() const
{
    return m_type;
}

Result<serialization::Value> Ptr_Value::serialize() const
{
    std::shared_ptr<const IValue> value = get_value();
    if (value)
    {
        serialization::Value sz_value(serialization::Value::Type::OBJECT);
        sz_value.add_object_member("type", serialization::Value(value->get_type()->get_symbol_path().to_string()));
        auto result = value->serialize();
        if (result != success)
        {
            return result;
        }
        sz_value.add_object_member("value", result.extract_payload());
        return std::move(sz_value);
    }
    else
    {
        return serialization::Value(serialization::Value::Type::EMPTY);
    }
}

Result<void> Ptr_Value::deserialize(serialization::Value const& sz_value)
{
    if (sz_value.get_type() == serialization::Value::Type::EMPTY)
    {
        set_value(std::shared_ptr<IValue>());
        return success;
    }

    if (sz_value.get_type() != serialization::Value::Type::OBJECT)
    {
        return Error("Expected object or null value when deserializing");
    }

    TS_ASSERT(false);
    return Error("Not implemented");
}

std::shared_ptr<const IValue> Ptr_Value::get_value() const
{
    return m_value;
}
std::shared_ptr<IValue> Ptr_Value::get_value()
{
    return m_value;
}

Result<void> Ptr_Value::set_value(std::shared_ptr<IValue> value)
{
    if (value && value->get_type() != get_specialized_type()->get_inner_type())
    {
        return Error("Wrong value type. Got '" +
                     value->get_type()->get_symbol_path().to_string() +
                     "', expected '" + get_specialized_type()->get_inner_type()->get_symbol_path().to_string() + "'");
    }
    m_value = value;
}


}
