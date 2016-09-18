#include "def_lang/impl/Poly_Value.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/IStruct_Type.h"
#include "def_lang/impl/Initializer_List.h"
#include "def_lang/Qualified_Type.h"

namespace ts
{

Poly_Value::Poly_Value(std::shared_ptr<IPoly_Type const> type)
    : m_type(type)
{
}

bool Poly_Value::is_constructed() const
{
    return m_is_constructed;
}

Result<bool> Poly_Value::is_equal(IValue const& other) const
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    IPoly_Value const* v = dynamic_cast<const IPoly_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (get_specialized_type()->get_inner_qualified_type() != v->get_specialized_type()->get_inner_qualified_type())
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

Result<void> Poly_Value::construct(IInitializer_List const& initializer_list)
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
Result<void> Poly_Value::copy_construct(IValue const& other)
{
    auto result = construct();
    if (result != success)
    {
        return result;
    }
    return copy_assign(other);
}

Result<void> Poly_Value::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    IPoly_Value const* v = dynamic_cast<const IPoly_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    if (!is_type_allowed(*v->get_specialized_type()->get_inner_qualified_type()->get_type()))
    {
        return Error("Cannot point to type " + v->get_specialized_type()->get_inner_qualified_type()->get_type()->get_symbol_path().to_string());
    }

    if (get_value() == nullptr)
    {
        if (v->get_value() == nullptr)
        {
            return success;
        }


        std::shared_ptr<IValue> value = v->get_value()->get_type()->create_value();
        auto result = value->copy_construct(*v->get_value());
        if (result != success)
        {
            return result;
        }

        return set_value(value);
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
Result<void> Poly_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("not implemented");
}

std::shared_ptr<IType const> Poly_Value::get_type() const
{
    return m_type;
}

Result<void> Poly_Value::parse_from_ui_string(std::string const& str)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}
Result<std::string> Poly_Value::get_ui_string() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (m_value)
    {
        auto result = m_value->get_ui_string();
        if (result == ts::success)
        {
            return m_value->get_type()->get_ui_name() + ": " + result.payload();
        }
        else
        {
            return m_value->get_type()->get_ui_name();
        }
    }
    return Error("<null>");
}

std::shared_ptr<const IValue> Poly_Value::select(Value_Selector&& selector) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return const_cast<Poly_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Poly_Value::select(Value_Selector&& selector)
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

    if (get_value() == nullptr)
    {
        return nullptr;
    }

    return get_value()->select(std::move(selector));
}

std::shared_ptr<IPoly_Type const> Poly_Value::get_specialized_type() const
{
    return m_type;
}

Result<sz::Value> Poly_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    std::shared_ptr<const IValue> value = get_value();
    if (value)
    {
        sz::Value sz_value(sz::Value::Type::OBJECT);
        sz_value.add_object_member("type", sz::Value(value->get_type()->get_symbol_path().to_string()));
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
        return sz::Value();
    }
}

Result<void> Poly_Value::deserialize(sz::Value const& sz_value)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    if (sz_value.is_empty())
    {
        return set_value(std::shared_ptr<IValue>());
    }

    if (!sz_value.is_object())
    {
        return Error("Expected object or null value when deserializing");
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

    std::shared_ptr<IValue> value = type->create_value();
    auto result = value->construct();
    if (result != success)
    {
        return Error("Cannot construct value when deserializing: " + result.error().what());
    }

    result = set_value(value);
    if (result != success)
    {
        return Error("Cannot create value when deserializing: " + result.error().what());
    }

    sz::Value const* value_sz_value = sz_value.find_object_member_by_name("value");
    if (!value_sz_value)
    {
        return Error("Expected 'value' when deserializing");
    }

    return get_value()->deserialize(*value_sz_value);
}

std::shared_ptr<const IValue> Poly_Value::get_value() const
{
    TS_ASSERT(is_constructed());
    return m_value;
}
std::shared_ptr<IValue> Poly_Value::get_value()
{
    if (get_specialized_type()->get_inner_qualified_type()->is_const())
    {
        return nullptr;
    }
    TS_ASSERT(is_constructed());
    return m_value;
}

Result<void> Poly_Value::set_value(std::shared_ptr<IValue> value)
{
//    if (!value)
//    {
//        return Error("Cannot insert null value");
//    }
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (value)
    {
        if (!value->is_constructed())
        {
            TS_ASSERT(false);
            return Error("Cannot set an unconstructed value");
        }

        if (!is_type_allowed(*value->get_type()))
        {
            return Error("Cannot point to type " + value->get_type()->get_symbol_path().to_string());
        }
    }

    if (m_value != value)
    {

        bool type_changes = false;
        if (value != nullptr && m_value != nullptr)
        {
            type_changes = value->get_type() != m_value->get_type();
        }
        else if (value == nullptr && m_value == nullptr)
        {
            type_changes = false;
        }
        else
        {
            type_changes = true;
        }

        //first notify that the type will change
        if (type_changes)
        {
            sig_type_will_change();
        }

        //the actual assignment
        m_value = value;

        //now that the type changed
        if (type_changes)
        {
            sig_type_has_changed();
        }

        //notify the value changed as well
        sig_value_changed();
    }

    return success;
}

bool Poly_Value::is_type_allowed(IType const& type) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return false;
    }

    IType const* inner_type = get_specialized_type()->get_inner_qualified_type()->get_type().get();
    if (inner_type == &type)
    {
        return true;
    }

    IStruct_Type const* struct_inner_type = dynamic_cast<IStruct_Type const*>(inner_type);
    IStruct_Type const* struct_other_type = dynamic_cast<IStruct_Type const*>(&type);

    if (struct_inner_type && struct_other_type)
    {
        return struct_inner_type->is_base_of(*struct_other_type);
    }

    return false;
}


}
