#pragma once

#include "Result.h"
#include "IValue.h"
#include "ITemplate_Argument.h"
#include "IBool_Type.h"

namespace ts
{

class IBool_Value : virtual public IValue, public virtual ITemplate_Argument
{
public:
    virtual ~IBool_Value() = default;

    typedef IBool_Traits traits;

    virtual std::shared_ptr<traits::type_interface const> get_specialized_type() const = 0;

    virtual Result<void> set_value(traits::fundamental_type value) = 0;
    virtual traits::fundamental_type get_value() const = 0;
};

}
