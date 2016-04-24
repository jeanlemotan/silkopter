#pragma once

#include "IType.h"

namespace ts
{

template<typename Traits>
class INumeric_Type_Template : virtual public IType
{
public:
    virtual ~INumeric_Type_Template() = default;

    typedef Traits traits;

    virtual std::shared_ptr<typename traits::value_interface> create_specialized_value() const = 0;

    virtual typename Traits::fundamental_type const& get_min_value() const = 0;
    virtual typename Traits::fundamental_type const& get_max_value() const = 0;
    virtual size_t get_decimals() const = 0;
};

}
