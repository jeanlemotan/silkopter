#pragma once

#include "IAttribute.h"
#include "values/IValue.h"

namespace ts
{

class IMax_Attribute : public virtual IAttribute
{
public:
    virtual auto get_max_value() const -> std::shared_ptr<const IValue> = 0;
};

}
