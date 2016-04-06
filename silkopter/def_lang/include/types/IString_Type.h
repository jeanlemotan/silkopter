#pragma once

#include "IType.h"

namespace ts
{

class IString_Value;

class IString_Type : virtual public IType
{
public:
    typedef IString_Value value_type;
    typedef std::string fundamental_type;

    virtual auto get_specialized_default_value() const -> std::shared_ptr<const value_type> = 0;
    virtual auto create_specialized_value() const -> std::unique_ptr<value_type> = 0;
};

}
