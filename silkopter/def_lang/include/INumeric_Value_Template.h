#pragma once

#include "Result.h"
#include "IValue.h"

namespace ts
{

template<typename Traits>
class INumeric_Value_Template : virtual public IValue
{
public:
    virtual ~INumeric_Value_Template() = default;

    typedef Traits traits;

    virtual auto get_specialized_type() const -> typename traits::type_interface const& = 0;

    virtual auto set_value(typename traits::fundamental_type value) -> Result<void> = 0;
    virtual auto get_value() const -> typename traits::fundamental_type = 0;
};

}
