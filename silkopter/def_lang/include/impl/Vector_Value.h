#pragma once

#include "IVector_Type.h"
#include "IVector_Value.h"
#include "ep/Value_Container_EP.h"

namespace ts
{

class Vector_Value final : virtual public IVector_Value, public Value_Container_EP
{
public:

    Vector_Value(std::shared_ptr<IVector_Type const> type);

    Result<bool> is_equal(IValue const& other) const override;

    Result<void> copy_assign(IValue const& other) override;
    Result<void> copy_assign(IInitializer const& initializer) override;

    std::unique_ptr<IValue> clone() const override;

    std::shared_ptr<IType const> get_type() const override;

    Result<void> parse_from_ui_string(std::string const& str) override;
    Result<std::string> get_ui_string() const override;

    IValue const* select(Value_Selector const& selector) const override;
    IValue* select(Value_Selector const& selector) override;

    std::shared_ptr<IVector_Type const> get_specialized_type() const override;

private:
    std::shared_ptr<IVector_Type const> m_type;
};

}
