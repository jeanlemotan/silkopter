#pragma once

#include "Result.h"
#include "IValue.h"
#include "ITemplate_Argument.h"
#include "types/IBool_Type.h"

namespace ts
{

class IBool_Value : virtual public IValue, public virtual ITemplate_Argument
{
public:
    virtual ~IBool_Value() = default;

    typedef IBool_Traits traits;

    virtual auto get_specialized_type() const -> traits::type_interface const& = 0;

    virtual auto set_value(traits::fundamental_type value) -> Result<void> = 0;
    virtual auto get_value() const -> traits::fundamental_type = 0;
};

}
