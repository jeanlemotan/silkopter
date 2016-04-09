#include "Initializer_List.h"

namespace ts
{

Initializer_List::Initializer_List(std::vector<std::unique_ptr<IInitializer>> initializers)
{
    m_initializers.reserve(initializers.size());
    for (std::unique_ptr<IInitializer>& initializer: initializers)
    {
        m_initializers.push_back(std::move(initializer));
    }
    initializers.clear();
}

auto Initializer_List::get_initializer_count() const -> size_t
{
    return m_initializers.size();
}
auto Initializer_List::get_initializer(size_t idx) const -> std::shared_ptr<const IInitializer>
{
    return m_initializers[idx];
}



}
