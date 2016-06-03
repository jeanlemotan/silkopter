#include "def_lang/impl/Struct_Type.h"
#include "def_lang/IValue.h"
#include "def_lang/impl/Struct_Value.h"
#include "def_lang/IAttribute.h"
#include "def_lang/impl/UI_Name_Attribute.h"
#include "def_lang/impl/Native_Type_Attribute.h"

namespace ts
{

Struct_Type::Struct_Type(std::string const& name, std::shared_ptr<const IStruct_Type> parent)
    : Declaration_Scope_EP(name)
    , m_base_struct(parent)
    , m_ui_name(name)
{
    replicate_base_struct();
}

Struct_Type::Struct_Type(Struct_Type const& other, std::string const& name)
    : Declaration_Scope_EP(other, name)
    , Member_Def_Container_EP(other)
    , Attribute_Container_EP(other)
    , m_base_struct(other.m_base_struct)
    , m_ui_name(name)
    , m_native_type(other.m_native_type)
    , m_first_noninherited_member_def_index(other.m_first_noninherited_member_def_index)
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
    return m_native_type.empty() ? get_symbol_path() : m_native_type;
}

Result<void> Struct_Type::validate_symbol(std::shared_ptr<const ISymbol> symbol)
{
    return ts::success;
}

Result<void> Struct_Type::validate_attribute(IAttribute const& attribute)
{
    if (UI_Name_Attribute const* att = dynamic_cast<UI_Name_Attribute const*>(&attribute))
    {
        m_ui_name = att->get_ui_name();
        return success;
    }
    else if (Native_Type_Attribute const* att = dynamic_cast<Native_Type_Attribute const*>(&attribute))
    {
        m_native_type = att->get_native_type();
        return success;
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

std::string Struct_Type::get_template_instantiation_string() const
{
    return get_symbol_path().to_string();
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

std::string Struct_Type::generate_serialization_code() const
{
    return "";
}


}
