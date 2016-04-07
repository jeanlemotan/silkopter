#pragma once

#include "IValue.h"

namespace ts
{

class IVector_Type;

class IVector_Value : virtual public IValue
{
public:
    typedef IVector_Type type_type;

    virtual auto get_specialized_type() const -> std::shared_ptr<type_type> = 0;
};

}
