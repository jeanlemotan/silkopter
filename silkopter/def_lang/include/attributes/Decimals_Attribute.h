#pragma once

#include "IAttribute.h"

namespace ts
{

class IDecimals_Attribute : public virtual IAttribute
{
public:
    virtual ~IDecimals_Attribute() = default;

    virtual auto get_decimals() const -> size_t = 0;
};

}
