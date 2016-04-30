#include "def_lang/impl/Variant_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/IDeclaration_Scope.h"

namespace ts
{

Variant_Value::Variant_Value(std::shared_ptr<IVariant_Type const> type)
    : m_type(type)
{
    auto result = set_value(m_type->get_inner_type(0)->create_value());
    TS_ASSERT(result == success);
}

Result<bool> Variant_Value::is_equal(IValue const& other) const
{
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

Result<void> Variant_Value::copy_assign(IValue const& other)
{
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
Result<void> Variant_Value::copy_assign(IInitializer const& initializer)
{
    return Error("not implemented");
}

std::shared_ptr<IValue> Variant_Value::clone() const
{
    return std::make_shared<Variant_Value>(*this);
}

std::shared_ptr<IType const> Variant_Value::get_type() const
{
    return m_type;
}

Result<void> Variant_Value::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}
Result<std::string> Variant_Value::get_ui_string() const
{
    return Error("Not Supported");
}

std::shared_ptr<const IValue> Variant_Value::select(Value_Selector&& selector) const
{
    return const_cast<Variant_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Variant_Value::select(Value_Selector&& selector)
{
    return nullptr;
}

std::shared_ptr<IVariant_Type const> Variant_Value::get_specialized_type() const
{
    return m_type;
}

Result<serialization::Value> Variant_Value::serialize() const
{
    serialization::Value svalue(serialization::Value::Type::OBJECT);

    svalue.add_object_member("type", serialization::Value(m_type->get_inner_type(get_value_type_index())->get_symbol_path().to_string()));

    auto result = get_value()->serialize();
    if (result != success)
    {
        return result;
    }
    svalue.add_object_member("value", result.extract_payload());

    return std::move(svalue);
}

Result<void> Variant_Value::deserialize(serialization::Value const& sz_value)
{
    if (!sz_value.is_object())
    {
        return Error("Expected object value when deserializing");
    }


    serialization::Value const* type_sz_value = sz_value.find_object_member_by_name("type");
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

    serialization::Value const* value_sz_value = sz_value.find_object_member_by_name("value");
    if (!value_sz_value)
    {
        return Error("Expected 'value' when deserializing");
    }

    return get_value()->deserialize(*value_sz_value);
}

std::shared_ptr<const IValue> Variant_Value::get_value() const
{
    return m_value;
}
std::shared_ptr<IValue> Variant_Value::get_value()
{
    return m_value;
}

Result<void> Variant_Value::set_value(std::shared_ptr<const IValue> value)
{
    if (!value)
    {
        return Error("Cannot set null value in variant '" + m_type->get_symbol_path().to_string() + "'");
    }

    boost::optional<size_t> idx = m_type->find_inner_type_idx(value->get_type());
    if (idx == boost::none)
    {
        return Error("Type '" + value->get_type()->get_symbol_path().to_string() + "' not allowed in variant '" + m_type->get_symbol_path().to_string() + "'");
    }

    m_value = value->clone();
    return success;
}

Result<void> Variant_Value::set_value_type(std::shared_ptr<const IType> type)
{
    if (!type)
    {
        return Error("Cannot set null type in variant '" + m_type->get_symbol_path().to_string() + "'");
    }
    return set_value(type->create_value());
}

Result<void> Variant_Value::set_value_type_index(size_t idx)
{
    return set_value(m_type->get_inner_type(idx)->create_value());
}

size_t Variant_Value::get_value_type_index() const
{
    boost::optional<size_t> idx = m_type->find_inner_type_idx(m_value->get_type());
    if (idx == boost::none)
    {
        TS_ASSERT(false);
        return 0;
    }
    return *idx;
}



}
