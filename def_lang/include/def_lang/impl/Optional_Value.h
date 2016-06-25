#pragma once

#include "def_lang/IOptional_Type.h"
#include "def_lang/IOptional_Value.h"

namespace ts
{

class Optional_Value final : virtual public IOptional_Value
{
public:

    Optional_Value(std::shared_ptr<IOptional_Type const> type);

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

    Result<sz::Value> serialize() const override;
    Result<void> deserialize(sz::Value const&) override;

    std::shared_ptr<IOptional_Type const> get_specialized_type() const override;

    bool is_set() const override;
    Result<void> mark_as_set(bool set) override;

    Result<void> set_value(std::shared_ptr<const IValue> value) override;
    std::shared_ptr<const IValue> get_value() const override;
    std::shared_ptr<IValue> get_value() override;

private:
    Result<void> create_value();

    bool m_is_constructed = false;
    bool m_is_set = false;
    std::shared_ptr<IOptional_Type const> m_type;
    std::shared_ptr<IValue> m_value;
};

}
