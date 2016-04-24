#pragma once

#include "Result.h"
#include "IValue.h"
#include "IString_Type.h"

namespace ts
{

class IString_Value : virtual public IValue
{
public:
    virtual ~IString_Value() = default;

    typedef IString_Traits traits;

    virtual std::shared_ptr<traits::type_interface const> get_specialized_type() const = 0;

    virtual Result<void> set_value(traits::fundamental_type value) = 0;
    virtual traits::fundamental_type get_value() const = 0;
};

}
