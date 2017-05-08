#include "def_lang/impl/Struct_Type.h"
#include "def_lang/IValue.h"
#include "def_lang/impl/Struct_Value.h"
#include "def_lang/IString_Value.h"
#include "def_lang/IAttribute.h"
#include "def_lang/ILiteral_Attribute.h"
#include "def_lang/IName_Attribute.h"

#include <deque>

namespace ts
{

Struct_Type::Struct_Type(std::string const& name, std::shared_ptr<const IStruct_Type> parent)
    : Declaration_Scope_EP(name)
    , m_base_struct(parent)
    , m_ui_name(name)
{
    replicate_base_struct();
    m_first_noninherited_member_def_index = get_member_def_count();
}

Struct_Type::Struct_Type(Struct_Type const& other, std::string const& name)
    : Declaration_Scope_EP(other, name)
    , Member_Def_Container_EP(other)
    , Attribute_Container_EP(other)
    , m_base_struct(other.m_base_struct)
    , m_ui_name(name)
    , m_native_type(other.m_native_type)
    , m_first_noninherited_member_def_index(other.m_first_noninherited_member_def_index)
    , m_is_public(other.m_is_public)
{
}

void Struct_Type::replicate_base_struct()
{
    if (m_base_struct)
    {
        for (size_t i = 0; i < m_base_struct->get_member_def_count(); i++)
        {
            auto result = add_member_def(m_base_struct->get_member_def(i));
            TS_ASSERT(result == success);
        }
        for (size_t i = 0; i < m_base_struct->get_attribute_count(); i++)
        {
            auto result = add_attribute(m_base_struct->get_attribute(i));
            TS_ASSERT(result == success);
        }
    }
}

size_t Struct_Type::get_first_noninhereted_member_def_index() const
{
    return m_first_noninherited_member_def_index;
}
size_t Struct_Type::get_noninherited_member_def_count() const
{
    return get_member_def_count() - m_first_noninherited_member_def_index;
}

std::shared_ptr<IMember_Def const> Struct_Type::get_noninherited_member_def(size_t idx) const
{
    return get_member_def(idx + m_first_noninherited_member_def_index);
}

std::string const& Struct_Type::get_ui_name() const
{
    return m_ui_name;
}
Symbol_Path Struct_Type::get_native_type() const
{
    return m_native_type;
}
bool Struct_Type::is_public() const
{
    return m_is_public;
}

std::vector<std::shared_ptr<const IStruct_Type>> Struct_Type::get_all_inheriting_types() const
{
    IDeclaration_Scope const* top_scope = get_parent_scope();
    if (!top_scope)
    {
        return {};
    }
    while (top_scope->get_parent_scope())
    {
        top_scope = top_scope->get_parent_scope();
    }

    std::vector<std::shared_ptr<const IStruct_Type>> results;

    std::deque<const IDeclaration_Scope*> stack{ top_scope };
    while (!stack.empty())
    {
        IDeclaration_Scope const* scope = stack.front();
        stack.pop_front();

        for (size_t i = 0; i < scope->get_symbol_count(); i++)
        {
            std::shared_ptr<ISymbol const> symbol = scope->get_symbol(i);
            if (std::shared_ptr<IStruct_Type const> struct_type = std::dynamic_pointer_cast<IStruct_Type const>(symbol))
            {
                if (is_base_of(*struct_type))
                {
                    results.emplace_back(struct_type);
                }
            }

            if (std::shared_ptr<IDeclaration_Scope const> s = std::dynamic_pointer_cast<IDeclaration_Scope const>(symbol))
            {
                stack.push_back(s.get());
            }
        }
    }

    return results;
}


Result<void> Struct_Type::validate_symbol(std::shared_ptr<const ISymbol> symbol)
{
    return ts::success;
}

Result<void> Struct_Type::validate_attribute(IAttribute const& attribute)
{
    if (attribute.get_name() == "ui_name")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IString_Value const* value = dynamic_cast<IString_Value const*>(&att->get_value());
            if (!value)
            {
                return Error("Attribute '" + attribute.get_name() + "' has to be a string.");
            }
            m_ui_name = value->get_value();
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a string literal.");
        }
    }
    else if (attribute.get_name() == "native_type")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IString_Value const* value = dynamic_cast<IString_Value const*>(&att->get_value());
            if (!value)
            {
                return Error("Attribute '" + attribute.get_name() + "' has to be a string.");
            }
            m_native_type = value->get_value();
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a string literal.");
        }
    }
    else if (attribute.get_name() == "public")
    {
        if (IName_Attribute const* att = dynamic_cast<IName_Attribute const*>(&attribute))
        {
            m_is_public = true;
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a named attribute.");
        }
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }
}

std::shared_ptr<IType> Struct_Type::clone(std::string const& name) const
{
    return std::make_shared<Struct_Type>(*this, name);
}
std::shared_ptr<IType> Struct_Type::alias(std::string const& name) const
{
    std::shared_ptr<Struct_Type> alias = std::make_shared<Struct_Type>(*this, name);
    alias->m_native_type = Symbol_Path(); //clear the native type as this is an alias
    alias->m_aliased_type = this->shared_from_this();
    return alias;
}
std::shared_ptr<const IType> Struct_Type::get_aliased_type() const
{
    return m_aliased_type;
}

std::shared_ptr<IValue> Struct_Type::create_value() const
{
    return create_specialized_value();
}

std::shared_ptr<IStruct_Value> Struct_Type::create_specialized_value() const
{
    return std::make_shared<Struct_Value>(this->shared_from_this());
}

std::shared_ptr<const IStruct_Type> Struct_Type::get_base_struct() const
{
    return m_base_struct;
}

bool Struct_Type::is_base_of(IStruct_Type const& type) const
{
    std::shared_ptr<const IStruct_Type> type_base = type.get_base_struct();
    if (type_base == nullptr)
    {
        return false;
    }
    return type_base.get() == this || is_base_of(*type_base);
}

}
