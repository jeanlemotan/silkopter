#pragma once

#include "IValue.h"
#include "Result.h"
#include "Value_Selector.h"

namespace ts
{
namespace mapper
{

    template<typename T>
    Result<void> get(IValue const& src, Value_Selector&& selector, T& dst);

    template<typename T>
    Result<void> set(IValue& src, Value_Selector&& selector, T const& dst);

}
}

#include "impl/Mapper.inl"
