#pragma once

#include "IBool_Type.h"
#include "IBool_Value.h"
#include "ep/Value_Template_EP.h"

namespace ts
{

class Bool_Value;

struct Bool_Traits : public IBool_Traits
{
    typedef Bool_Value value_implementation;
};

class Bool_Value final : public Value_Template_EP<Bool_Traits>
{
public:

    Bool_Value(std::shared_ptr<IBool_Type const> type);

    std::string get_template_instantiation_string() const override;

    std::shared_ptr<IValue> clone() const override;

    using Value_Template_EP<Bool_Traits>::copy_assign;
    Result<void> copy_assign(IInitializer const& initializer) override;

    Result<void> serialize(ISerializer& serializer) const override;


private:
};

}
