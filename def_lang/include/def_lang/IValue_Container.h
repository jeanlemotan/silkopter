#pragma once

#include <stdint.h>
#include <memory>
#include "Result.h"

namespace ts
{

class IValue;

class IValue_Container
{
public:
    virtual ~IValue_Container() = default;

    virtual size_t get_value_count() const = 0;

    virtual std::shared_ptr<const IValue> get_value(size_t idx) const = 0;
    virtual std::shared_ptr<IValue> get_value(size_t idx) = 0;
};

}
