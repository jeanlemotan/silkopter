#pragma once

#include "def_lang/IEnum_Type.h"
#include "def_lang/IEnum_Value.h"
#include "def_lang/IEnum_Item.h"

namespace ts
{

class Enum_Value final : public virtual IEnum_Value
{
public:

    Enum_Value(std::shared_ptr<IEnum_Type const> type);

    bool is_constructed() const override;

    Result<bool> is_equal(IValue const& other) const override;

    Result<void> construct(IInitializer_List const& initializer_list) override;
    Result<void> copy_construct(IValue const& other) override;

    Result<void> copy_assign(IValue const& other) override;
    Result<void> copy_assign(IInitializer_List const& initializer_list) override;

    std::shared_ptr<IType const> get_type() const override;

    Result<void> parse_from_ui_string(std::string const& str) override;
    Result<std::string> get_ui_string() const override;

    std::shared_ptr<const IValue> select(Value_Selector&& selector) const override;
    std::shared_ptr<IValue> select(Value_Selector&& selector) override;

    Result<serialization::Value> serialize() const override;
    Result<void> deserialize(serialization::Value const&) override;

    std::shared_ptr<IEnum_Type const> get_specialized_type() const override;

    Result<void> set_value(std::shared_ptr<const IEnum_Item> item) override;
    std::shared_ptr<const IEnum_Item> get_value() const override;

private:
    bool m_is_constructed = false;
    std::shared_ptr<const IEnum_Type> m_type;
    std::shared_ptr<const IEnum_Item> m_value;
};

}
