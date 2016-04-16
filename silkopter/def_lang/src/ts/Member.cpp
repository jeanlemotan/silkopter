#include "Member.h"
#include "IMember_Def.h"
#include "values/IValue.h"

namespace ts
{

Member::Member(IMember_Def const& member_def)
    : m_member_def(member_def)
{
    m_value = member_def.get_default_value().clone();
}

Member::~Member()
{

}

auto Member::get_member_def() const -> IMember_Def const&
{
    return m_member_def;
}

auto Member::get_value() const -> IValue const&
{
    return *m_value;
}

auto Member::get_value() -> IValue&
{
    return *m_value;
}


}
