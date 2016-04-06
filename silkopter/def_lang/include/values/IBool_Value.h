#pragma once

#include "IValue.h"

namespace ts
{

class IBool_Type;

class IBool_Value : virtual public IValue
{
public:
    typedef IBool_Type type_type;
    typedef bool fundamental_type;

    virtual auto get_specialized_type() const -> std::shared_ptr<type_type> = 0;
};

}
