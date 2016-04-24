#include "def_lang/impl/Struct_Value.h"
#include "def_lang/impl/Member.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/Serialization.h"
#include "def_lang/IMember_Def.h"

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

Result<bool> Struct_Value::is_equal(IValue const& other) const
{
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

Result<void> Struct_Value::copy_assign(IValue const& other)
{
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
Result<void> Struct_Value::copy_assign(IInitializer const& initializer)
{
    return Error("not implemented");
}

std::shared_ptr<IValue> Struct_Value::clone() const
{
    return std::make_shared<Struct_Value>(*this);
}

std::shared_ptr<IType const> Struct_Value::get_type() const
{
    return m_type;
}

Result<void> Struct_Value::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}
Result<std::string> Struct_Value::get_ui_string() const
{
    return Error("Not Supported");
}

std::shared_ptr<const IValue> Struct_Value::select(Value_Selector&& selector) const
{
    return const_cast<Struct_Value*>(this)->select(std::move(selector));
}
std::shared_ptr<IValue> Struct_Value::select(Value_Selector&& selector)
{
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

Result<serialization::Value> Struct_Value::serialize() const
{
    serialization::Value svalue(serialization::Value::Type::OBJECT);

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

Result<void> Struct_Value::deserialize(serialization::Value const& sz_value)
{
    if (sz_value.get_type() != serialization::Value::Type::OBJECT)
    {
        return Error("Expected object value when deserializing");
    }

    for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
    {
        std::string const& name = sz_value.get_object_member_name(i);
        std::shared_ptr<IMember> member = find_member_by_name(name);
        if (!member)
        {
            return Error("Cannot find member name '" + name + "'");
        }
        auto result = member->get_value()->deserialize(sz_value.get_object_member_value(i));
        if (result != ts::success)
        {
            return result;
        }
    }

    return success;
}

}
