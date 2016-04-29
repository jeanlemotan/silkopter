#pragma once

#include "IValue.h"

namespace ts
{

class IEnum_Type;
class IEnum_Item;

class IEnum_Value : virtual public IValue
{
public:
    virtual ~IEnum_Value() = default;

    virtual std::shared_ptr<IEnum_Type const> get_specialized_type() const = 0;

    virtual Result<void> set_value(std::shared_ptr<const IEnum_Item> item) = 0;
    virtual std::shared_ptr<const IEnum_Item> get_value() const = 0;
};


}
