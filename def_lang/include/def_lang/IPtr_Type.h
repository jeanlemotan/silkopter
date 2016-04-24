#pragma once

#include "ITemplated_Type.h"

namespace ts
{

class IPtr_Value;
class IPtr_Type;

struct IPtr_Traits
{
    typedef IPtr_Value value_interface;
    typedef IPtr_Type type_interface;
    typedef std::shared_ptr<IValue> fundamental_type;
};

class IPtr_Type : virtual public ITemplated_Type
{
public:
    virtual ~IPtr_Type() = default;

    typedef IPtr_Traits traits;

    virtual std::shared_ptr<traits::value_interface> create_specialized_value() const = 0;

    virtual std::shared_ptr<const IType> get_inner_type() const = 0;
};

}

