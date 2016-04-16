#pragma once

#include "types/IStruct_Type.h"
#include "IStruct_Value.h"
#include "IMember.h"
#include "impl/Member_Container_EP.h"

namespace ts
{

class Struct_Value final : public virtual IStruct_Value, public Member_Container_EP
{
public:

    Struct_Value(IStruct_Type const& type);

    auto is_equal(IValue const& other) const -> Result<bool> override;

    auto copy_assign(IValue const& other) -> Result<void> override;
    auto copy_assign(IInitializer const& initializer) -> Result<void> override;

    auto clone() const -> std::unique_ptr<IValue> override;

    auto get_type() const -> IType const& override;

    auto parse_from_ui_string(std::string const& str) -> Result<void> override;
    auto get_ui_string() const -> std::string override;

    auto select(Value_Selector const& selector) const -> IValue const* override;
    auto select(Value_Selector const& selector) -> IValue* override;

    auto get_specialized_type() const -> IStruct_Type const& override;

private:
    IStruct_Type const& m_type;
};

}
