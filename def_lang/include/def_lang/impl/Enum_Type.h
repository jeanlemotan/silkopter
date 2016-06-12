#pragma once

#include "def_lang/IEnum_Type.h"
#include "def_lang/ep/Declaration_Scope_EP.h"
#include "def_lang/ep/Attribute_Container_EP.h"

namespace ts
{

class Enum_Type final: virtual public IEnum_Type, public Declaration_Scope_EP, public Attribute_Container_EP, public std::enable_shared_from_this<Enum_Type>
{
public:
    typedef IEnum_Value value_type;

    Enum_Type(std::string const& name);
    Enum_Type(Enum_Type const& other, std::string const& name);

    std::shared_ptr<IType> clone(std::string const& name) const override;
    std::shared_ptr<IType> alias(std::string const& name) const override;
    std::shared_ptr<const IType> get_aliased_type() const override;

    std::string const& get_ui_name() const override;
    Symbol_Path get_native_type() const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<IEnum_Value> create_specialized_value() const override;

    size_t get_item_count() const override;
    std::shared_ptr<const IEnum_Item> get_item(size_t idx) const override;
    std::shared_ptr<const IEnum_Item> find_item_by_name(std::string const& name) const override;
    std::shared_ptr<const IEnum_Item> find_item_by_integral_value(int64_t value) const override;
    std::shared_ptr<const IEnum_Item> get_default_item() const override;

protected:
    Result<void> validate_symbol(std::shared_ptr<const ISymbol> symbol) override;
    Result<void> validate_attribute(IAttribute const& attribute) override;

private:
    std::shared_ptr<const IEnum_Item> m_default_item;
    std::shared_ptr<const IType> m_aliased_type;
    std::string m_ui_name;
    Symbol_Path m_native_type;
};

}
