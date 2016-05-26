#pragma once

#include "IValue.h"
#include "IMember_Container.h"

namespace ts
{

class IStruct_Type;

class IStruct_Value : virtual public IValue, virtual public IMember_Container
{
public:
    virtual ~IStruct_Value() = default;

    virtual std::shared_ptr<IStruct_Type const> get_specialized_type() const = 0;

    virtual size_t get_first_noninhereted_member_index() const = 0;
    virtual size_t get_noninherited_member_count() const = 0;
    virtual std::shared_ptr<IMember const> get_noninherited_member(size_t idx) const = 0;
    virtual std::shared_ptr<IMember> get_noninherited_member(size_t idx) = 0;
};


}
