#pragma once

#include "Result.h"
#include "IValue.h"
#include "types/IString_Type.h"

namespace ts
{

class IString_Value : virtual public IValue
{
public:
    virtual ~IString_Value() = default;

    typedef IString_Traits traits;

    virtual auto get_specialized_type() const -> traits::type_interface const& = 0;

    virtual auto set_value(traits::fundamental_type value) -> Result<void> = 0;
    virtual auto get_value() const -> traits::fundamental_type = 0;
};

}
