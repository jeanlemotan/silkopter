#pragma once

#include "IValue.h"

namespace ts
{

class IStruct_Type;

class IStruct_Value : virtual public IValue
{
public:
    virtual ~IStruct_Value() = default;

    virtual auto get_specialized_type() const -> IStruct_Type const& = 0;
};


}
