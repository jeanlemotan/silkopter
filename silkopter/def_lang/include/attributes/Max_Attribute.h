#pragma once

#include "IAttribute.h"

namespace ts
{

class IValue;

class IMax_Attribute : public virtual IAttribute
{
public:
    virtual ~IMax_Attribute() = default;

    virtual auto get_max_value() const -> IValue const& = 0;
};

}
