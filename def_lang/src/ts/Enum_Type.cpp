#include "def_lang/impl/Enum_Type.h"
#include "def_lang/impl/Enum_Value.h"
#include "def_lang/IEnum_Item.h"
#include "def_lang/IAttribute.h"

namespace ts
{

Enum_Type::Enum_Type(std::string const& name)
    : Declaration_Scope_EP(name)
{

}

std::shared_ptr<IType> Enum_Type::clone(std::string const& name) const
{
    return std::shared_ptr<IType>(new Enum_Type(*this));
}

std::string Enum_Type::get_template_instantiation_string() const
{
    return get_symbol_path().to_string();
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
    return Error("Attribute " + attribute.get_name() + " not supported");
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
