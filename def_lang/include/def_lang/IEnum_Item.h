#pragma once

#include "IDeclaration_Scope.h"

namespace ts
{

class IEnum_Item: virtual public ISymbol
{
public:
    virtual ~IEnum_Item() = default;

    virtual int64_t get_integral_value() const = 0;

    virtual std::string const& get_ui_name() const = 0;

    virtual std::shared_ptr<IEnum_Value> create_specialized_value() const = 0;
};

}
