#pragma once

#include "IStruct_Type.h"
#include "IStruct_Value.h"
#include "IMember.h"
#include "ep/Member_Container_EP.h"

namespace ts
{

class Struct_Value final : public virtual IStruct_Value, public Member_Container_EP
{
public:

    Struct_Value(std::shared_ptr<IStruct_Type const> type);

    Result<bool> is_equal(IValue const& other) const override;

    Result<void> copy_assign(IValue const& other) override;
    Result<void> copy_assign(IInitializer const& initializer) override;

    std::shared_ptr<IValue> clone() const override;

    std::shared_ptr<IType const> get_type() const override;

    Result<void> parse_from_ui_string(std::string const& str) override;
    Result<std::string> get_ui_string() const override;

    IValue const* select(Value_Selector const& selector) const override;
    IValue* select(Value_Selector const& selector) override;

    std::shared_ptr<IStruct_Type const> get_specialized_type() const override;

private:
    std::shared_ptr<IStruct_Type const> m_type;
};

}
