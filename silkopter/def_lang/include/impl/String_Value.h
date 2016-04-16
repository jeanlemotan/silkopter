#pragma once

#include "IString_Type.h"
#include "IString_Value.h"
#include "ep/Value_Template_EP.h"

namespace ts
{

class String_Value;

struct String_Traits : public IString_Traits
{
    typedef String_Value value_implementation;
};

class String_Value final : public Value_Template_EP<String_Traits>
{
public:

    String_Value(IString_Type const& type);

    using Value_Template_EP<String_Traits>::copy_assign;
    auto copy_assign(IInitializer const& initializer) -> Result<void> override;

private:
};

}
