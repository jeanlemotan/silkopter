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

    typedef IVector_Type type_interface;

    virtual auto get_specialized_type() const -> type_interface const& = 0;
};

}
