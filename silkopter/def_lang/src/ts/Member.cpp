#include "impl/Member.h"
#include "IMember_Def.h"
#include "IValue.h"

namespace ts
{

Member::Member(std::shared_ptr<IMember_Def const> member_def)
    : m_member_def(member_def)
{
    m_value = member_def->get_default_value().clone();
}

Member::~Member()
{

}

std::shared_ptr<IMember_Def const> Member::get_member_def() const
{
    return m_member_def;
}

IValue const& Member::get_value() const
{
    return *m_value;
}

IValue& Member::get_value()
{
    return *m_value;
}


}
