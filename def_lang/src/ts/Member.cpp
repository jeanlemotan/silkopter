#include "def_lang/impl/Member.h"
#include "def_lang/IMember_Def.h"
#include "def_lang/IValue.h"
#include "def_lang/IType.h"

namespace ts
{

Member::Member(std::shared_ptr<IMember_Def const> member_def)
    : m_member_def(member_def)
{
    m_value = member_def->get_default_value()->get_type()->create_value();
    auto result = m_value->copy_construct(*member_def->get_default_value());
    TS_ASSERT(result == success);
}

Member::~Member()
{

}

std::shared_ptr<IMember_Def const> Member::get_member_def() const
{
    return m_member_def;
}

std::shared_ptr<const IValue> Member::get_value() const
{
    return m_value;
}

std::shared_ptr<IValue> Member::get_value()
{
    return m_value;
}


}
