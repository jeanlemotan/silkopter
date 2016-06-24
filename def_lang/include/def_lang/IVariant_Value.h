#pragma once

#include "IValue.h"

namespace ts
{

class IVariant_Type;

class IVariant_Value : virtual public IValue
{
public:
    virtual ~IVariant_Value() = default;

    virtual std::shared_ptr<IVariant_Type const> get_specialized_type() const = 0;

    virtual Result<void> set_value_type(std::shared_ptr<const IType> type) = 0;
    virtual Result<void> set_value_type_index(size_t idx) = 0;
    virtual size_t get_value_type_index() const = 0;

    virtual bool is_set() const = 0;

    virtual Result<void> set_value(std::shared_ptr<const IValue> value) = 0;
    virtual std::shared_ptr<const IValue> get_value() const = 0;
    virtual std::shared_ptr<IValue> get_value() = 0;
};


}
