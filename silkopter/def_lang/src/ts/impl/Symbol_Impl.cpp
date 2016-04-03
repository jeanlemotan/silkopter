#include "impl/Symbol_Impl.h"

namespace ts
{

Symbol_Impl::Symbol_Impl(std::string const& name)
    : m_name(name)
{
}

auto Symbol_Impl::get_name() const -> std::string const&
{
    return m_name;
}

}
