#pragma once

#include "def_lang/IPoly_Type.h"
#include "def_lang/IPoly_Value.h"

namespace ts
{

class Poly_Value final : virtual public IPoly_Value
{
public:

    Poly_Value(std::shared_ptr<IPoly_Type const> type);

    bool is_constructed() const override;

    Result<bool> is_equal(IValue const& other) const override;

    Result<void> construct(IInitializer_List const& initializer_list = Initializer_List({})) override;
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

    std::shared_ptr<IPoly_Type const> get_specialized_type() const override;

    std::shared_ptr<const IValue> get_value() const override;
    std::shared_ptr<IValue> get_value() override;
    Result<void> set_value(std::shared_ptr<IValue>) override;

private:
    bool is_type_allowed(IType const& type) const;

    bool m_is_constructed = false;
    std::shared_ptr<IPoly_Type const> m_type;
    std::shared_ptr<IValue> m_value;
};

}
