#pragma once

#include "IValue.h"

namespace ts
{

template<typename Traits> class IReal_Type;

template<typename Traits>
class IReal_Value : virtual public IValue
{
public:
    typedef Traits traits;
    typedef IReal_Type<Traits> type_type;
    typedef typename Traits::fundamental_type fundamental_type;

    virtual auto get_specialized_type() const -> std::shared_ptr<type_type> = 0;
};

}
