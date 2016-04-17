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

    virtual Result<void> insert_value(size_t idx, std::unique_ptr<IValue>&& value) = 0;
    virtual Result<void> erase_value(size_t idx) = 0;

    virtual size_t get_value_count() const = 0;

    virtual IValue const& get_value(size_t idx) const = 0;
    virtual IValue& get_value(size_t idx) = 0;
};

}
