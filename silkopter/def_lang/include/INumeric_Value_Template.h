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

    virtual std::shared_ptr<typename traits::type_interface const> get_specialized_type() const = 0;

    virtual Result<void> set_value(typename traits::fundamental_type value) = 0;
    virtual typename traits::fundamental_type get_value() const = 0;
};

}
