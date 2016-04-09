#pragma once

#include "IValue.h"

namespace ts
{

template<typename Traits> class IIntegral_Type_Template;

template<typename Traits>
class IIntegral_Value_Template : virtual public IValue
{
public:
    typedef Traits traits;
    typedef IIntegral_Type_Template<Traits> type_type;
    typedef typename Traits::fundamental_type fundamental_type;

    virtual auto get_specialized_type() const -> std::shared_ptr<type_type> = 0;

    virtual auto set_value(fundamental_type value) -> Result<void> = 0;
    virtual auto get_value() const -> fundamental_type = 0;
};

}
