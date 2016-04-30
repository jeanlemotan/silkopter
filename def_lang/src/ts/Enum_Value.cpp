#include "def_lang/impl/Enum_Type.h"
#include "def_lang/impl/Enum_Value.h"
#include "def_lang/IEnum_Item_Initializer.h"
#include "def_lang/impl/Initializer_List.h"

namespace ts
{

Enum_Value::Enum_Value(std::shared_ptr<IEnum_Type const> type)
    : m_type(type)
{
    m_value = m_type->get_default_item();
}

bool Enum_Value::is_constructed() const
{
    return m_is_constructed;
}

Result<bool> Enum_Value::is_equal(IValue const& other) const
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
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

Result<void> Enum_Value::construct(IInitializer_List const& initializer_list)
{
    if (is_constructed())
    {
        TS_ASSERT(false);
        return Error("Already constructed value");
    }

    if (initializer_list.get_initializer_count() != 1)
    {
        return Error("Expected an initializer list with one element");
    }
    std::shared_ptr<const IEnum_Item_Initializer> initializer = std::dynamic_pointer_cast<const IEnum_Item_Initializer>(initializer_list.get_initializer(0));
    if (!initializer)
    {
        return Error("Cannot evaluate initializer list");
    }

    m_is_constructed = true;

    auto result = set_value(initializer->get_enum_item());
    if (result != success)
    {
        m_is_constructed = false;
        return result;
    }

    return success;
}
Result<void> Enum_Value::copy_construct(IValue const& other)
{
    auto result = construct(Initializer_List({}));
    if (result != success)
    {
        return result;
    }
    return copy_assign(other);
}

Result<void> Enum_Value::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
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

Result<void> Enum_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (initializer_list.get_initializer_count() != 1)
    {
        return Error("Expected an initializer list with one element");
    }
    std::shared_ptr<const IEnum_Item_Initializer> initializer = std::dynamic_pointer_cast<const IEnum_Item_Initializer>(initializer_list.get_initializer(0));
    if (!initializer)
    {
        return Error("Cannot evaluate initializer list");
    }

    return set_value(initializer->get_enum_item());
}

std::shared_ptr<IType const> Enum_Value::get_type() const
{
    return m_type;
}

Result<void> Enum_Value::parse_from_ui_string(std::string const& str)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}

Result<std::string> Enum_Value::get_ui_string() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}

std::shared_ptr<const IValue> Enum_Value::select(Value_Selector&& selector) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return const_cast<Enum_Value*>(this)->select(std::move(selector));
}

std::shared_ptr<IValue> Enum_Value::select(Value_Selector&& selector)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return nullptr;
}

Result<serialization::Value> Enum_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return serialization::Value(m_value->get_name());
}

Result<void> Enum_Value::deserialize(serialization::Value const& sz_value)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
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
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (item->get_parent_scope() != m_type.get())
    {
        return Error("Cannot initialize a '" + m_type->get_symbol_path().to_string() + "' with a '" + item->get_symbol_path().to_string() + "'");
    }
    m_value = item;
    return success;
}

std::shared_ptr<const IEnum_Item> Enum_Value::get_value() const
{
    TS_ASSERT(is_constructed());
    return m_value;
}

}
