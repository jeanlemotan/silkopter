#pragma once

#include "IValue.h"
#include "IValue_Container.h"

namespace ts
{

class IPtr_Type;

class IPtr_Value : virtual public IValue
{
public:
    virtual ~IPtr_Value() = default;

    virtual std::shared_ptr<IPtr_Type const> get_specialized_type() const = 0;

    virtual std::shared_ptr<const IValue> get_value() const = 0;
    virtual std::shared_ptr<IValue> get_value() = 0;
    virtual Result<void> set_value(std::shared_ptr<IValue>) = 0;
};

}
