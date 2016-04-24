#pragma once

#include "IType.h"

namespace ts
{

class IString_Value;
class IString_Type;

struct IString_Traits
{
    typedef IString_Value value_interface;
    typedef IString_Type type_interface;
    typedef std::string fundamental_type;
};

class IString_Type : virtual public IType
{
public:
    virtual ~IString_Type() = default;

    typedef IString_Traits traits;

    virtual std::shared_ptr<traits::value_interface> create_specialized_value() const = 0;
};

}

