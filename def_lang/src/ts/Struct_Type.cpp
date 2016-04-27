#include "def_lang/impl/Struct_Type.h"
#include "def_lang/IValue.h"
#include "def_lang/impl/Struct_Value.h"
#include "def_lang/IAttribute.h"

namespace ts
{

Struct_Type::Struct_Type(std::string const& name, std::shared_ptr<const IStruct_Type> parent)
    : Declaration_Scope_EP(name)
    , m_base_struct(parent)
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

Result<void> Struct_Type::validate_attribute(IAttribute const& attribute)
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

std::shared_ptr<IType> Struct_Type::clone(std::string const& name) const
{
    return std::shared_ptr<IType>(new Struct_Type(*this));
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


}
