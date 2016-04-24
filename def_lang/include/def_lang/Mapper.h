#pragma once

#include "IValue.h"
#include "Result.h"
#include "Value_Selector.h"
#include "All_INumeric_Values.h"
#include "IVector_Value.h"
#include "IString_Value.h"
#include "IBool_Value.h"

namespace ts
{
namespace mapper
{

template<typename T>
Result<void> get(IValue const& src, T& dst);

template<typename T>
Result<void> set(IValue& dst, T const& src);


template<typename T>
Result<void> get(IValue const& src, Value_Selector&& selector, T& dst);

template<typename T>
Result<void> set(IValue& src, Value_Selector&& selector, T const& dst);

}
}

#include "impl/Mapper.inl"
