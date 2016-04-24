#pragma once

#include "def_lang/IStruct_Type.h"
#include "def_lang/IStruct_Value.h"
#include "def_lang/IMember.h"
#include "def_lang/ep/Member_Container_EP.h"

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

    std::shared_ptr<const IValue> select(Value_Selector&& selector) const override;
    std::shared_ptr<IValue> select(Value_Selector&& selector) override;

    Result<serialization::Value> serialize() const override;
    Result<void> deserialize(serialization::Value const&) override;

    std::shared_ptr<IStruct_Type const> get_specialized_type() const override;

private:
    std::shared_ptr<IStruct_Type const> m_type;
};

}
