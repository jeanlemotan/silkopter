#pragma once

#include "ITemplated_Type.h"

namespace ts
{

class IVector_Value;

class IVector_Type : virtual public ITemplated_Type
{
public:
    typedef IVector_Value value_type;

    virtual auto get_specialized_default_value() const -> std::shared_ptr<const value_type> = 0;
    virtual auto create_specialized_value() const -> std::unique_ptr<value_type> = 0;
};

}
