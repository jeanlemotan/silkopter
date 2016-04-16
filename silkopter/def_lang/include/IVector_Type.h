#pragma once

#include "ITemplated_Type.h"

namespace ts
{

class IVector_Value;

class IVector_Type : virtual public ITemplated_Type
{
public:
    virtual ~IVector_Type() = default;

    typedef IVector_Value value_type;

    virtual auto create_specialized_value() const -> std::unique_ptr<value_type> = 0;
};

}
