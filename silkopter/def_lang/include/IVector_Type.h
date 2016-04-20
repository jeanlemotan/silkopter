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

    virtual std::shared_ptr<const IType> get_inner_type() const = 0;

    virtual std::shared_ptr<value_type> create_specialized_value() const = 0;
};

}
