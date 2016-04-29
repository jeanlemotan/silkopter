#pragma once

#include "IInitializer.h"
#include "ISymbol.h"
#include "IAttribute_Container.h"

namespace ts
{

class IEnum_Item: virtual public ISymbol, virtual public IAttribute_Container, public virtual IInitializer
{
public:
    virtual ~IEnum_Item() = default;

    virtual int64_t get_integral_value() const = 0;
    virtual std::string const& get_ui_name() const = 0;
    virtual bool is_default() const = 0;
};

}
