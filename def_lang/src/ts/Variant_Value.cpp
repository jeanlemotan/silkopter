#include "def_lang/impl/Variant_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/IDeclaration_Scope.h"
#include "def_lang/Qualified_Type.h"

namespace ts
{

Variant_Value::Variant_Value(std::shared_ptr<IVariant_Type const> type)
    : m_type(type)
{
}

Variant_Value::~Variant_Value()
{
    for (boost::signals2::connection& c: m_value_changed_connections)
    {
        c.disconnect();
    }
}

bool Variant_Value::is_constructed() const
{
    return m_is_constructed;
}

Result<bool> Variant_Value::is_equal(IValue const& other) const
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    IVariant_Value const* v = dynamic_cast<const IVariant_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (get_value_type_index() != v->get_value_type_index())
    {
        return false;
    }

    return get_value()->is_equal(*v->get_value());
}

Result<void> Variant_Value::construct(IInitializer_List const& initializer_list)
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

    //construct all inner values
    for (size_t i = 0; i < m_type->get_inner_qualified_type_count(); i++)
    {
        std::shared_ptr<IValue> value = m_type->get_inner_qualified_type(i)->get_type()->create_value();
        auto result = value->construct();
        if (result != success)
        {
            m_values.clear();
            return result;
        }

        m_values.push_back(value);
        m_value_changed_connections.push_back(value->sig_value_changed.connect([this]
        {
            sig_value_changed();
        }));
    }

    m_is_constructed = true;

    return success;
}
Result<void> Variant_Value::copy_construct(IValue const& other)
{
    auto result = construct();
    if (result != success)
    {
        return result;
    }
    return copy_assign(other);
}

Result<void> Variant_Value::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    IVariant_Value const* v = dynamic_cast<const IVariant_Value*>(&other);
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
Result<void> Variant_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("not implemented");
}

std::shared_ptr<IType const> Variant_Value::get_type() const
{
    return m_type;
}

Result<void> Variant_Value::parse_from_ui_string(std::string const& str)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}
Result<std::string> Variant_Value::get_ui_string() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    auto result = get_value()->get_ui_string();
    if (result == ts::success)
    {
        return get_value()->get_type()->get_ui_name() + ": " + result.payload();
    }
    else
    {
        return get_value()->get_type()->get_ui_name();
    }
}

std::shared_ptr<const IValue> Variant_Value::select(Value_Selector&& selector) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return const_cast<Variant_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Variant_Value::select(Value_Selector&& selector)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return nullptr;
}

std::shared_ptr<IVariant_Type const> Variant_Value::get_specialized_type() const
{
    return m_type;
}

Result<sz::Value> Variant_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    sz::Value svalue(sz::Value::Type::OBJECT);

    svalue.add_object_member("type", sz::Value(m_type->get_inner_qualified_type(get_value_type_index())->get_type()->get_symbol_path().to_string()));

    auto result = get_value()->serialize();
    if (result != success)
    {
        return result;
    }
    svalue.add_object_member("value", result.extract_payload());

    return std::move(svalue);
}

Result<void> Variant_Value::deserialize(sz::Value const& sz_value)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (!sz_value.is_object())
    {
        return Error("Expected object value when deserializing");
    }


    sz::Value const* type_sz_value = sz_value.find_object_member_by_name("type");
    if (!type_sz_value || !type_sz_value->is_string())
    {
        return Error("Expected 'type' string value when deserializing");
    }

    std::shared_ptr<const IType> type = get_type()->get_parent_scope()->find_specialized_symbol_by_path<const IType>(Symbol_Path(type_sz_value->get_as_string()));
    if (!type)
    {
        return Error("Cannot find type '" + type_sz_value->get_as_string() + "' when deserializing");
    }

    auto result = set_value_type(type);
    if (result != success)
    {
        return Error("Cannot create type '" + type->get_symbol_path().to_string() + "' when deserializing: " + result.error().what());
    }

    sz::Value const* value_sz_value = sz_value.find_object_member_by_name("value");
    if (!value_sz_value)
    {
        return Error("Expected 'value' when deserializing");
    }

    return get_value()->deserialize(*value_sz_value);
}

bool Variant_Value::is_set() const
{
    return true;
}


std::shared_ptr<const IValue> Variant_Value::get_value() const
{
    TS_ASSERT(is_constructed());
    return m_values[m_crt_type_index];
}
std::shared_ptr<IValue> Variant_Value::get_value()
{
    if (get_specialized_type()->get_inner_qualified_type(get_value_type_index())->is_const())
    {
        return nullptr;
    }

    TS_ASSERT(is_constructed());
    return m_values[m_crt_type_index];
}

Result<void> Variant_Value::set_value(std::shared_ptr<const IValue> value)
{
    if (!value)
    {
        return Error("Cannot set null value in variant '" + m_type->get_symbol_path().to_string() + "'");
    }
    if (!is_constructed() || !value->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    auto result = set_value_type(value->get_type());
    if (result != ts::success)
    {
        return result;
    }

    return get_value()->copy_assign(*value);
}

Result<void> Variant_Value::set_value_type(std::shared_ptr<const IType> type)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (!type)
    {
        return Error("Cannot set null type in variant '" + m_type->get_symbol_path().to_string() + "'");
    }

    boost::optional<size_t> idx = m_type->find_inner_qualified_type_idx(type);
    if (idx == boost::none)
    {
        return Error("Type '" + type->get_symbol_path().to_string() + "' not allowed in variant '" + m_type->get_symbol_path().to_string() + "'");
    }

    return set_value_type_index(*idx);
}

Result<void> Variant_Value::set_value_type_index(size_t idx)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (m_crt_type_index != idx)
    {
        sig_type_index_will_change();

        TS_ASSERT(idx < m_values.size());
        m_crt_type_index = idx;

        sig_type_index_has_changed();
        sig_value_changed();
    }

    return ts::success;
}

size_t Variant_Value::get_value_type_index() const
{
    TS_ASSERT(is_constructed());
    return m_crt_type_index;
}



}
