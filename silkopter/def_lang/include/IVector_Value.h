#pragma once

#include "IValue.h"
#include "IValue_Container.h"

namespace ts
{

class IVector_Type;

class IVector_Value : virtual public IValue, virtual public IValue_Container
{
public:
    virtual ~IVector_Value() = default;

    virtual Result<void> insert_default_value(size_t idx) = 0;
    virtual Result<void> insert_value(size_t idx, std::shared_ptr<IValue> value) = 0;
    virtual Result<void> erase_value(size_t idx) = 0;

    virtual std::shared_ptr<IVector_Type const> get_specialized_type() const = 0;
};

}
