#include "def_lang/impl/Struct_Value.h"
#include "def_lang/impl/Member.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/IMember_Def.h"
#include "def_lang/impl/Initializer_List.h"

namespace ts
{

Struct_Value::Struct_Value(std::shared_ptr<IStruct_Type const> type)
    : m_type(type)
{
    for (size_t i = 0; i < type->get_member_def_count(); i++)
    {
        auto result = add_member(std::make_shared<Member>(type->get_member_def(i)));
        TS_ASSERT(result == success);
    }
}

bool Struct_Value::is_constructed() const
{
    return m_is_constructed;
}

Result<bool> Struct_Value::is_equal(IValue const& other) const
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    TS_ASSERT(get_member_count() == v->get_member_count());

    for (size_t i = 0; i < get_member_count(); i++)
    {
        auto result = get_member(i)->get_value()->is_equal(*v->get_member(i)->get_value());
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

Result<void> Struct_Value::construct(IInitializer_List const& initializer_list)
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
Result<void> Struct_Value::copy_construct(IValue const& other)
{
    auto result = construct();
    if (result != success)
    {
        return result;
    }
    return copy_assign(other);
}

Result<void> Struct_Value::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    IStruct_Value const* v = dynamic_cast<const IStruct_Value*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    if (get_type() != v->get_type())
    {
        return Error("incompatible types");
    }

    TS_ASSERT(get_member_count() == v->get_member_count());

    for (size_t i = 0; i < get_member_count(); i++)
    {
        auto result = get_member(i)->get_value()->copy_assign(*v->get_member(i)->get_value());
        if (result != success)
        {
            return result;
        }
    }

    return success;
}
Result<void> Struct_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    return Error("not implemented");
}

std::shared_ptr<IType const> Struct_Value::get_type() const
{
    return m_type;
}

Result<void> Struct_Value::parse_from_ui_string(std::string const& str)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not Supported");
}
Result<std::string> Struct_Value::get_ui_string() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    return Error("Not supported");
}

size_t Struct_Value::get_first_noninhereted_member_index() const
{
    return m_type->get_first_noninhereted_member_def_index();
}
size_t Struct_Value::get_noninherited_member_count() const
{
    return m_type->get_noninherited_member_def_count();
}
std::shared_ptr<IMember const> Struct_Value::get_noninherited_member(size_t idx) const
{
    return get_member(idx + get_first_noninhereted_member_index());
}
std::shared_ptr<IMember> Struct_Value::get_noninherited_member(size_t idx)
{
    return get_member(idx + get_first_noninhereted_member_index());
}

std::shared_ptr<const IValue> Struct_Value::select(Value_Selector&& selector) const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return nullptr;
    }
    return const_cast<Struct_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Struct_Value::select(Value_Selector&& selector)
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

    std::shared_ptr<IMember> member;
    Value_Selector::Element const& element = selector.front();
    if (element.type == Value_Selector::Element::Type::NAME)
    {
        member = find_member_by_name(element.name);
    }

    if (!member)
    {
        return nullptr;
    }

    selector.pop_front();
    if (selector.empty())
    {
        return member->get_value();
    }

    return member->get_value()->select(std::move(selector));
}

std::shared_ptr<IStruct_Type const> Struct_Value::get_specialized_type() const
{
    return m_type;
}

Result<sz::Value> Struct_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }
    sz::Value svalue(sz::Value::Type::OBJECT);

    for (size_t i = 0; i < get_member_count(); i++)
    {
        IMember const& member = *get_member(i);

        auto result = member.get_value()->serialize();
        if (result != success)
        {
            return result;
        }

        svalue.add_object_member(member.get_member_def()->get_name(), result.extract_payload());
    }

    return std::move(svalue);
}

Result<void> Struct_Value::deserialize(sz::Value const& sz_value)
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

    for (size_t i = 0; i < get_member_count(); i++)
    {
        std::string const& name = get_specialized_type()->get_member_def(i)->get_name();
        sz::Value const* member_sz_value = sz_value.find_object_member_by_name(name);
        if (!member_sz_value)
        {
            return Error("Cannot find member name '" + name + "'");
        }

        std::shared_ptr<IMember> member = get_member(i);
        auto result = member->get_value()->deserialize(*member_sz_value);
        if (result != ts::success)
        {
            return result;
        }
    }

    return success;
}

}
