#pragma once

#include "def_lang/IVector_Type.h"
#include "def_lang/IVector_Value.h"

namespace ts
{

class Vector_Value final : virtual public IVector_Value
{
public:

    Vector_Value(std::shared_ptr<IVector_Type const> type);

    Result<bool> is_equal(IValue const& other) const override;

    Result<void> copy_assign(IValue const& other) override;
    Result<void> copy_assign(IInitializer const& initializer) override;

    std::shared_ptr<IValue> clone() const override;

    std::shared_ptr<IType const> get_type() const override;

    Result<void> parse_from_ui_string(std::string const& str) override;
    Result<std::string> get_ui_string() const override;

    std::shared_ptr<const IValue> select(Value_Selector&& selector) const override;
    std::shared_ptr<IValue> select(Value_Selector&& selector) override;

    Result<serialization::Value> serialize() const override;
    Result<void> deserialize(serialization::Value const&) override;

    std::shared_ptr<IVector_Type const> get_specialized_type() const override;

    Result<std::shared_ptr<IValue>> insert_default_value(size_t idx) override;
    Result<void> insert_value(size_t idx, std::shared_ptr<IValue> value) override;
    Result<void> erase_value(size_t idx) override;
    void clear() override;

    size_t get_value_count() const override;

    std::shared_ptr<const IValue> get_value(size_t idx) const override;
    std::shared_ptr<IValue> get_value(size_t idx) override;

private:
    std::shared_ptr<IVector_Type const> m_type;
    std::vector<std::shared_ptr<IValue>> m_values;
};

}
