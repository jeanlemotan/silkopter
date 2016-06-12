#include "def_lang/impl/Enum_Type.h"
#include "def_lang/impl/Enum_Value.h"
#include "def_lang/IEnum_Item.h"
#include "def_lang/IAttribute.h"
#include "def_lang/impl/UI_Name_Attribute.h"
#include "def_lang/impl/Native_Type_Attribute.h"

namespace ts
{

Enum_Type::Enum_Type(std::string const& name)
    : Declaration_Scope_EP(name)
    , m_ui_name(name)
{

}

Enum_Type::Enum_Type(Enum_Type const& other, std::string const& name)
    : Declaration_Scope_EP(other, name)
    , Attribute_Container_EP(other)
    , m_default_item(other.m_default_item)
    , m_ui_name(name)
    , m_native_type(other.m_ui_name)
{

}

std::shared_ptr<IType> Enum_Type::clone(std::string const& name) const
{
    return std::make_shared<Enum_Type>(*this, name);
}
std::shared_ptr<IType> Enum_Type::alias(std::string const& name) const
{
    std::shared_ptr<Enum_Type> alias = std::make_shared<Enum_Type>(*this, name);
    alias->m_native_type = Symbol_Path(); //clear the native type as this is an alias
    alias->m_aliased_type = this->shared_from_this();
    return alias;
}
std::shared_ptr<const IType> Enum_Type::get_aliased_type() const
{
    return m_aliased_type;
}

std::string Enum_Type::get_template_instantiation_string() const
{
    return get_symbol_path().to_string();
}

std::string const& Enum_Type::get_ui_name() const
{
    return m_ui_name;
}

Symbol_Path Enum_Type::get_native_type() const
{
    return m_native_type;
}

std::shared_ptr<IValue> Enum_Type::create_value() const
{
    return create_specialized_value();
}
std::shared_ptr<IEnum_Value> Enum_Type::create_specialized_value() const
{
    return std::make_shared<Enum_Value>(this->shared_from_this());
}

size_t Enum_Type::get_item_count() const
{
    return get_symbol_count();
}
std::shared_ptr<const IEnum_Item> Enum_Type::get_item(size_t idx) const
{
    return std::dynamic_pointer_cast<const IEnum_Item>(get_symbol(idx));
}
std::shared_ptr<const IEnum_Item> Enum_Type::find_item_by_name(std::string const& name) const
{
    return std::dynamic_pointer_cast<const IEnum_Item>(find_symbol_by_name(name));
}
std::shared_ptr<const IEnum_Item> Enum_Type::find_item_by_integral_value(int64_t value) const
{
    for (size_t i = 0; i < get_item_count(); i++)
    {
        std::shared_ptr<const IEnum_Item> item = get_item(i);
        if (item->get_integral_value() == value)
        {
            return item;
        }
    }
    return nullptr;
}
std::shared_ptr<const IEnum_Item> Enum_Type::get_default_item() const
{
    return m_default_item;
}

Result<void> Enum_Type::validate_attribute(IAttribute const& attribute)
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

Result<void> Enum_Type::validate_symbol(std::shared_ptr<const ISymbol> symbol)
{
    std::shared_ptr<const IEnum_Item> item = std::dynamic_pointer_cast<const IEnum_Item>(symbol);
    if (item == nullptr)
    {
        return Error("Only enum items are allowed inside the enum");
    }

    if (item->is_default())
    {
        if (get_default_item() != nullptr)
        {
            return Error("Only one item can be defaulted");
        }
        m_default_item = item;
    }

    return ts::success;
}

}
