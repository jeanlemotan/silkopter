#pragma once

#include "IType.h"

namespace ts
{

template<typename Traits> class IIntegral_Value_Template;


template<typename Traits>
class IIntegral_Type_Template : virtual public IType
{
public:
    typedef Traits traits;
    typedef IIntegral_Value_Template<Traits> value_type;
    typedef typename Traits::fundamental_type fundamental_type;

    virtual auto get_specialized_default_value() const -> std::shared_ptr<const value_type> = 0;
    virtual auto create_specialized_value() const -> std::unique_ptr<value_type> = 0;
};

}
