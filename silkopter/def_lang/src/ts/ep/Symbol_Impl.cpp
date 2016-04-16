#include "ep/Symbol_EP.h"

namespace ts
{

Symbol_EP::Symbol_EP(std::string const& name)
    : m_name(name)
{
}

auto Symbol_EP::get_name() const -> std::string const&
{
    return m_name;
}

}
