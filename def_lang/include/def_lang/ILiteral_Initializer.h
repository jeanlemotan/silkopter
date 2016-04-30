#pragma once

#include "def_lang/IInitializer.h"

namespace ts
{

class ILiteral;

class ILiteral_Initializer : public virtual IInitializer
{
public:
    virtual ~ILiteral_Initializer() = default;

    virtual std::shared_ptr<const ILiteral> get_literal() const = 0;
};

}
