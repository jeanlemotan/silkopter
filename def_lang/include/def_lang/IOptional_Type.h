#pragma once

#include "ITemplated_Type.h"

namespace ts
{

class IOptional_Value;

class IOptional_Type: virtual public ITemplated_Type
{
public:
    virtual ~IOptional_Type() = default;

    typedef IOptional_Value value_type;

    virtual std::shared_ptr<const IType> get_inner_type() const = 0;

    virtual std::shared_ptr<IOptional_Value> create_specialized_value() const = 0;
};

}
