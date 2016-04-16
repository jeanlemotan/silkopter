#pragma once

#include "types/IVector_Type.h"
#include "IVector_Value.h"
#include "impl/Value_Container_EP.h"

namespace ts
{

class Vector_Value final : virtual public IVector_Value, public Value_Container_EP
{
public:

    Vector_Value(IVector_Type const& type);

    auto is_equal(IValue const& other) const -> Result<bool> override;

    auto copy_assign(IValue const& other) -> Result<void> override;
    auto copy_assign(IInitializer const& initializer) -> Result<void> override;

    auto clone() const -> std::unique_ptr<IValue> override;

    auto get_type() const -> IType const& override;

    auto parse_from_ui_string(std::string const& str) -> Result<void> override;
    auto get_ui_string() const -> Result<std::string> override;

    auto select(Value_Selector const& selector) const -> IValue const* override;
    auto select(Value_Selector const& selector) -> IValue* override;

    auto get_specialized_type() const -> IVector_Type const& override;

private:
    IVector_Type const& m_type;
};

}
