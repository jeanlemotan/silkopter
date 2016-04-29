#pragma once

#include "IDeclaration_Scope.h"
#include "IType.h"

namespace ts
{

class IEnum_Value;
class IEnum_Item;

class IEnum_Type: virtual public IType, virtual public IDeclaration_Scope
{
public:
    virtual ~IEnum_Type() = default;

    typedef IEnum_Value value_type;

    virtual size_t get_item_count() const = 0;
    virtual std::shared_ptr<const IEnum_Item> get_item(size_t idx) const = 0;
    virtual std::shared_ptr<const IEnum_Item> find_item_by_name(std::string const& name) const = 0;
    virtual std::shared_ptr<const IEnum_Item> find_item_by_integral_value(int64_t value) const = 0;
    virtual std::shared_ptr<const IEnum_Item> get_default_item() const = 0;

    virtual std::shared_ptr<IEnum_Value> create_specialized_value() const = 0;
};

}
