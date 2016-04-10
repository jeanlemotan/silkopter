#include "Member_Def.h"
#include "values/IValue.h"

namespace ts
{

Member_Def::Member_Def(std::string const& name, IType const& type, std::unique_ptr<const IValue> default_value)
    : Symbol_EP(name)
    , m_type(type)
    , m_default_value(std::move(default_value))
{

}

Member_Def::~Member_Def()
{

}

auto Member_Def::get_type() const -> IType const&
{
    return m_type;
}
auto Member_Def::get_default_value() const -> IValue const&
{
    return *m_default_value;
}

}
