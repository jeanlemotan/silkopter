#pragma once

#include "IAttribute.h"

namespace ts
{

class IValue;

class IMin_Attribute : public virtual IAttribute
{
public:
    virtual ~IMin_Attribute() = default;

    virtual auto get_min_value() const -> IValue const& = 0;
};

}
