#pragma once

#include "types/IType.h"

namespace ts
{

class IVector_Value;

class IVector_Type: virtual public IType
{
public:

    virtual auto create_specialized_value() const -> std::unique_ptr<IVector_Value>;

};

}
