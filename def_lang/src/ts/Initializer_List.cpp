#include "def_lang/impl/Initializer_List.h"

namespace ts
{

Initializer_List::Initializer_List(std::vector<std::shared_ptr<const IInitializer>> initializers)
    : m_initializers(std::move(initializers))
{
}

size_t Initializer_List::get_initializer_count() const
{
    return m_initializers.size();
}
std::shared_ptr<const IInitializer> Initializer_List::get_initializer(size_t idx) const
{
    return m_initializers[idx];
}

}
