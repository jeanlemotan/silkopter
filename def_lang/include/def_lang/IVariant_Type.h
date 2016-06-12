#pragma once

#include "ITemplated_Type.h"

namespace ts
{

class IVariant_Value;
class Qualified_Type;

class IVariant_Type: virtual public ITemplated_Type
{
public:
    virtual ~IVariant_Type() = default;

    typedef IVariant_Value value_type;

    virtual size_t get_inner_qualified_type_count() const = 0;
    virtual std::shared_ptr<const Qualified_Type> get_inner_qualified_type(size_t idx) const = 0;
    virtual boost::optional<size_t> find_inner_qualified_type_idx(std::shared_ptr<const IType> type) const = 0;

    virtual std::shared_ptr<IVariant_Value> create_specialized_value() const = 0;
};

}
