#include "def_lang/impl/Enum_Type.h"
#include "def_lang/impl/Enum_Value.h"

namespace ts
{

Enum_Value::Enum_Value(std::shared_ptr<IEnum_Type const> type)
    : m_type(type)
{
    m_value = m_type->get_default_item();
}

Result<bool> Enum_Value::is_equal(IValue const& other) const
{
    IEnum_Value const* v = dynamic_cast<const IEnum_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    return m_value == v->get_value();
}

Result<void> Enum_Value::copy_assign(IValue const& other)
{
    IEnum_Value const* v = dynamic_cast<const IEnum_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    return set_value(v->get_value());
}

Result<void> Enum_Value::copy_assign(IInitializer const& initializer)
{
    if (IEnum_Item const* item = dynamic_cast<IEnum_Item const*>(&initializer))
    {
        if (item->get_parent_scope() != m_type.get())
        {
            return Error("Cannot initialize a '" + m_type->get_symbol_path().to_string() + "' with a '" + item->get_symbol_path().to_string() + "'");
        }
        std::shared_ptr<const IEnum_Item> i = m_type->find_item_by_name(item->get_name());
        return set_value(i);
    }
    return Error("Bad initializer item for '" + m_type->get_symbol_path().to_string() + "'");
}

std::shared_ptr<IValue> Enum_Value::clone() const
{
    return std::make_shared<Enum_Value>(*this);
}

std::shared_ptr<IType const> Enum_Value::get_type() const
{
    return m_type;
}

Result<void> Enum_Value::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}

Result<std::string> Enum_Value::get_ui_string() const
{
    return Error("Not Supported");
}

std::shared_ptr<const IValue> Enum_Value::select(Value_Selector&& selector) const
{
    return const_cast<Enum_Value*>(this)->select(std::move(selector));
}

std::shared_ptr<IValue> Enum_Value::select(Value_Selector&& selector)
{
    return nullptr;
}

Result<serialization::Value> Enum_Value::serialize() const
{
    return serialization::Value(m_value->get_name());
}

Result<void> Enum_Value::deserialize(serialization::Value const& sz_value)
{
    if (!sz_value.is_string())
    {
        return Error("Expected object value when deserializing");
    }

    std::shared_ptr<const IEnum_Item> item = m_type->find_item_by_name(sz_value.get_as_string());
    if (!item)
    {
        return Error("Cannot find enum item '" + sz_value.get_as_string() + "' in type '" + m_type->get_symbol_path().to_string() + "'");
    }

    return set_value(item);
}

std::shared_ptr<IEnum_Type const> Enum_Value::get_specialized_type() const
{
    return m_type;
}


Result<void> Enum_Value::set_value(std::shared_ptr<const IEnum_Item> item)
{
    if (item->get_parent_scope() != m_type.get())
    {
        return Error("Cannot initialize a '" + m_type->get_symbol_path().to_string() + "' with a '" + item->get_symbol_path().to_string() + "'");
    }
    m_value = item;
    return success;
}

std::shared_ptr<const IEnum_Item> Enum_Value::get_value() const
{
    return m_value;
}

}
