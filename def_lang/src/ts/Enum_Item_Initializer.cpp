#include "def_lang/impl/Enum_Item_Initializer.h"
#include "def_lang/ts_assert.h"

namespace ts
{

Enum_Item_Initializer::Enum_Item_Initializer(std::shared_ptr<const IEnum_Item> item)
    : m_item(item)
{
    TS_ASSERT(m_item);
}

std::shared_ptr<const IEnum_Item> Enum_Item_Initializer::get_enum_item() const
{
    return m_item;
}

}
