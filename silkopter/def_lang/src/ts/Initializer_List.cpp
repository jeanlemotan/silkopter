#include "Initializer_List.h"

namespace ts
{

Initializer_List::Initializer_List(std::vector<std::unique_ptr<IInitializer>> initializers)
    : m_initializers(std::move(initializers))
{
}

auto Initializer_List::get_initializer_count() const -> size_t
{
    return m_initializers.size();
}
auto Initializer_List::get_initializer(size_t idx) const -> IInitializer const&
{
    return *m_initializers[idx];
}
auto Initializer_List::get_initializer_value() const -> IValue const*
{
    if (m_initializers.size() == 1)
    {
        return m_initializers.front()->get_initializer_value();
    }
    return nullptr;
}


}
