#pragma once

#include "IType.h"

namespace ts
{

class IBool_Value;
class IBool_Type;

struct IBool_Traits
{
    typedef IBool_Value value_interface;
    typedef IBool_Type type_interface;
    typedef bool fundamental_type;
};

class IBool_Type : virtual public IType
{
public:
    virtual ~IBool_Type() = default;

    typedef IBool_Traits traits;

    virtual std::shared_ptr<traits::value_interface> create_specialized_value() const = 0;
};

}

