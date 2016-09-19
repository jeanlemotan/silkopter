#pragma once

#include "def_lang/IAttribute.h"

namespace ts
{

class IValue;

class ILiteral_Attribute : public virtual IAttribute
{
public:
    virtual IValue const& get_value() const = 0;
};

}
