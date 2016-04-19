#include "impl/Initializer_List.h"

namespace ts
{

Initializer_List::Initializer_List(std::vector<std::shared_ptr<IInitializer>> initializers)
    : m_initializers(std::move(initializers))
{
}

size_t Initializer_List::get_initializer_count() const
{
    return m_initializers.size();
}
IInitializer const& Initializer_List::get_initializer(size_t idx) const
{
    return *m_initializers[idx];
}
IValue const* Initializer_List::get_initializer_value() const
{
    if (m_initializers.size() == 1)
    {
        return m_initializers.front()->get_initializer_value();
    }
    return nullptr;
}


}
