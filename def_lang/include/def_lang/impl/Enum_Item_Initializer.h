#pragma once

#include "def_lang/IEnum_Item_Initializer.h"

namespace ts
{

class Enum_Item_Initializer : public virtual IEnum_Item_Initializer
{
public:
    Enum_Item_Initializer(std::shared_ptr<const IEnum_Item> item);

    std::shared_ptr<const IEnum_Item> get_enum_item() const override;

private:
    std::shared_ptr<const IEnum_Item> m_item;
};

}
