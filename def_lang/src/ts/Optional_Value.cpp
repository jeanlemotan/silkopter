#include "def_lang/impl/Optional_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/IDeclaration_Scope.h"
#include "def_lang/impl/Initializer_List.h"

namespace ts
{

Optional_Value::Optional_Value(std::shared_ptr<IOptional_Type const> type)
    : m_type(type)
{
}

bool Optional_Value::is_constructed() const
{
    return m_is_constructed;
}

Result<bool> Optional_Value::is_equal(IValue const& other) const
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    IOptional_Value const* v = dynamic_cast<const IOptional_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (is_set() != v->is_set())
    {
        return false;
    }

    if (!is_set())
    {
        return true;
    }

    return get_value()->is_equal(*v->get_value());
}

Result<void> Optional_Value::construct(IInitializer_List const& initializer_list)
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
Result<void> Optional_Value::copy_construct(IValue const& other)
{
    auto result = construct();
    if (result != success)
    {
        return result;
    }
    return copy_assign(other);
}

Result<void> Optional_Value::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    IOptional_Value const* v = dynamic_cast<const IOptional_Value*>(&other);
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
Result<void> Optional_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("not implemented");
}

std::shared_ptr<IType const> Optional_Value::get_type() const
{
    return m_type;
}

Result<void> Optional_Value::parse_from_ui_string(std::string const& str)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}
Result<std::string> Optional_Value::get_ui_string() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}

std::shared_ptr<const IValue> Optional_Value::select(Value_Selector&& selector) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return const_cast<Optional_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Optional_Value::select(Value_Selector&& selector)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return nullptr;
}

std::shared_ptr<IOptional_Type const> Optional_Value::get_specialized_type() const
{
    return m_type;
}

Result<serialization::Value> Optional_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (!is_set())
    {
        return serialization::Value(serialization::Value::Type::EMPTY);
    }
    else
    {
        return get_value()->serialize();
    }
}

Result<void> Optional_Value::deserialize(serialization::Value const& sz_value)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (sz_value.is_empty())
    {
        set_value(std::shared_ptr<IValue>());
        return success;
    }

    if (!m_value)
    {
        auto result = create_value();
        if (result != success)
        {
            return result;
        }
    }

    return get_value()->deserialize(sz_value);
}

Result<void> Optional_Value::create_value()
{
    std::shared_ptr<IValue> value = m_type->get_inner_type()->create_value();
    auto result = value->construct();
    if (result != success)
    {
        return result;
    }

    m_is_set = true;
    m_value = value;
    return success;
}

bool Optional_Value::is_set() const
{
    return m_is_set;
}

std::shared_ptr<const IValue> Optional_Value::get_value() const
{
    TS_ASSERT(is_constructed());
    return m_is_set ? m_value : std::shared_ptr<const IValue>();
}
std::shared_ptr<IValue> Optional_Value::get_value()
{
    TS_ASSERT(is_constructed());
    return m_is_set ? m_value : std::shared_ptr<IValue>();
}

Result<void> Optional_Value::set_value(std::shared_ptr<const IValue> value)
{
    if (!value)
    {
        return Error("Cannot set null value in optional '" + m_type->get_symbol_path().to_string() + "'");
    }
    if (!is_constructed() || !value->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (value->get_type() != m_type->get_inner_type())
    {
        return Error("Type '" + value->get_type()->get_symbol_path().to_string() + "' not allowed in optional '" + m_type->get_symbol_path().to_string() + "'");
    }

    if (!m_value)
    {
        auto result = create_value();
        if (result != success)
        {
            return result;
        }
    }

    return m_value->copy_assign(*value);
}



}
