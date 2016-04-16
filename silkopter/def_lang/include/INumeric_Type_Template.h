#pragma once

#include "IType.h"

namespace ts
{

template<typename Traits>
class INumeric_Type_Template : virtual public IType
{
public:
    virtual ~INumeric_Type_Template() = default;

    typedef Traits traits;

    virtual auto create_specialized_value() const -> std::unique_ptr<typename traits::value_interface> = 0;
};

}
