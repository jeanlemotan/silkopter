#pragma once

#include "IValue.h"
#include "IValue_Container.h"

namespace ts
{

class IPoly_Type;

class IPoly_Value : virtual public IValue
{
public:
    virtual ~IPoly_Value() = default;

    virtual std::shared_ptr<IPoly_Type const> get_specialized_type() const = 0;

    virtual std::shared_ptr<const IValue> get_value() const = 0;
    virtual std::shared_ptr<IValue> get_value() = 0;
    virtual Result<void> set_value(std::shared_ptr<IValue>) = 0;

    boost::signals2::signal<void()> sig_type_will_change;
    boost::signals2::signal<void()> sig_type_has_changed;
};

}
