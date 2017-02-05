#pragma once

#include "def_lang/IVector_Type.h"
#include "def_lang/IVector_Value.h"

namespace ts
{

class Vector_Value final : virtual public IVector_Value
{
public:
    Vector_Value(std::shared_ptr<IVector_Type const> type);
    ~Vector_Value();

    Result<bool> is_equal(IValue const& other) const override;

    bool is_constructed() const override;

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

    std::shared_ptr<IVector_Type const> get_specialized_type() const override;

    Result<std::shared_ptr<IValue>> insert_default_value(size_t idx) override;
    Result<void> insert_value(size_t idx, std::shared_ptr<IValue> value) override;
    Result<void> erase_value(size_t idx) override;
    void clear() override;

    size_t get_value_count() const override;

    std::shared_ptr<const IValue> get_value(size_t idx) const override;
    std::shared_ptr<IValue> get_value(size_t idx) override;

private:
    bool m_is_constructed = false;
    std::shared_ptr<IVector_Type const> m_type;
    std::vector<std::shared_ptr<IValue>> m_values;
    std::vector<boost::signals2::connection> m_value_changed_connections;
};

}
