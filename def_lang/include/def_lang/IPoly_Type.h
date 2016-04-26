#pragma once

#include "ITemplated_Type.h"

namespace ts
{

class IPoly_Value;
class IPoly_Type;

struct IPtr_Traits
{
    typedef IPoly_Value value_interface;
    typedef IPoly_Type type_interface;
    typedef std::shared_ptr<IValue> fundamental_type;
};

class IPoly_Type : virtual public ITemplated_Type
{
public:
    virtual ~IPoly_Type() = default;

    typedef IPtr_Traits traits;

    virtual std::shared_ptr<traits::value_interface> create_specialized_value() const = 0;

    virtual std::shared_ptr<const IType> get_inner_type() const = 0;
};

}

