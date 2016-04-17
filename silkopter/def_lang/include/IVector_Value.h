#pragma once

#include "IValue.h"
#include "IValue_Container.h"

namespace ts
{

class IVector_Type;

class IVector_Value : virtual public IValue, virtual public IValue_Container
{
public:
    virtual ~IVector_Value() = default;

    virtual std::shared_ptr<IVector_Type const> get_specialized_type() const = 0;
};

}
