#pragma once

#include "IValue.h"

namespace ts
{

class IOptional_Type;

class IOptional_Value : virtual public IValue
{
public:
    virtual ~IOptional_Value() = default;

    virtual std::shared_ptr<IOptional_Type const> get_specialized_type() const = 0;

    virtual bool is_set() const = 0;
    virtual Result<void> mark_as_set(bool set) = 0;

    virtual Result<void> set_value(std::shared_ptr<const IValue> value) = 0;
    virtual std::shared_ptr<const IValue> get_value() const = 0;
    virtual std::shared_ptr<IValue> get_value() = 0;

    boost::signals2::signal<void()> sig_will_be_unset;
    boost::signals2::signal<void()> sig_was_set;
};


}
