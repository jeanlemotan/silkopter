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

    Bool_Value(IBool_Type const& type);

    auto get_template_instantiation_string() const -> std::string override;

    using Value_Template_EP<Bool_Traits>::copy_assign;
    auto copy_assign(IInitializer const& initializer) -> Result<void> override;

private:
};

}
