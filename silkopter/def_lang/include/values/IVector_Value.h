#pragma once

#include "IValue.h"

namespace ts
{

class IVector_Type;

class IVector_Value : virtual public IValue
{
public:
    virtual ~IVector_Value() = default;

    typedef IVector_Type type_interface;

    virtual auto get_specialized_type() const -> std::shared_ptr<type_interface> = 0;
};

}
