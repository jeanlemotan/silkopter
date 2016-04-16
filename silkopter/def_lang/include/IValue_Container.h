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

    virtual auto insert_value(size_t idx, std::unique_ptr<IValue>&& value) -> Result<void> = 0;
    virtual auto erase_value(size_t idx) -> Result<void> = 0;

    virtual auto get_value_count() const -> size_t = 0;

    virtual auto get_value(size_t idx) const -> IValue const& = 0;
    virtual auto get_value(size_t idx) -> IValue& = 0;
};

}
