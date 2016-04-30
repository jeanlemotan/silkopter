#pragma once

#include "def_lang/IInitializer.h"

namespace ts
{

class IEnum_Item;

class IEnum_Item_Initializer : public virtual IInitializer
{
public:
    virtual ~IEnum_Item_Initializer() = default;

    virtual std::shared_ptr<const IEnum_Item> get_enum_item() const = 0;
};

}
