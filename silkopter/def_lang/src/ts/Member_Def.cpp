#include "Member_Def.h"
#include "values/IValue.h"

namespace ts
{

Member_Def::Member_Def(std::string const& name, std::shared_ptr<IType> type, std::unique_ptr<IValue> default_value)
    : Symbol_Impl(name)
    , m_type(type)
    , m_default_value(std::move(default_value))
{

}

Member_Def::~Member_Def()
{

}

auto Member_Def::get_type() const -> std::shared_ptr<IType>
{
    return m_type;
}
auto Member_Def::get_default_value() const -> std::shared_ptr<const IValue>
{
    return m_default_value;
}

}
