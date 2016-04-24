#pragma once

#include "Serialization.h"
#include "Result.h"

namespace ts
{
namespace serialization
{

std::string to_json(Value const& value);
Result<Value> from_json(std::string const& value);


}
}
