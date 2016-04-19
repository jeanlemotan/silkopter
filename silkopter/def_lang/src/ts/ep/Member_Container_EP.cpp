#include "ep/Member_Container_EP.h"
#include "IMember.h"
#include "IMember_Def.h"

namespace ts
{

Member_Container_EP::~Member_Container_EP()
{

}

size_t Member_Container_EP::get_member_count() const
{
    return m_members.size();
}

Result<void> Member_Container_EP::add_member(std::shared_ptr<IMember> member)
{
    if (!member)
    {
        return Error("Cannot add null member");
    }
    if (find_member_by_name(member->get_member_def()->get_name()) != nullptr)
    {
        return Error("Duplicate member name: " + member->get_member_def()->get_name());
    }

    m_members.push_back(std::move(member));

    return success;
}

IMember const& Member_Container_EP::get_member(size_t idx) const
{
    return *m_members[idx];
}
IMember& Member_Container_EP::get_member(size_t idx)
{
    return *m_members[idx];
}

boost::optional<size_t> Member_Container_EP::find_member_idx_by_name(std::string const& name) const
{
    return boost::none;
}

IMember const* Member_Container_EP::find_member_by_name(std::string const& name) const
{
    return nullptr;
}
IMember* Member_Container_EP::find_member_by_name(std::string const& name)
{
    return nullptr;
}


}
