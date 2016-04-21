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

std::shared_ptr<const IMember> Member_Container_EP::get_member(size_t idx) const
{
    return m_members[idx];
}
std::shared_ptr<IMember> Member_Container_EP::get_member(size_t idx)
{
    return m_members[idx];
}

boost::optional<size_t> Member_Container_EP::find_member_idx_by_name(std::string const& name) const
{
    auto it = std::find_if(m_members.begin(), m_members.end(), [&name](std::shared_ptr<const IMember> const& member) { return member->get_member_def()->get_name() == name; });
    return it != m_members.end() ? boost::optional<size_t>(std::distance(m_members.begin(), it)) : boost::optional<size_t>();
}

std::shared_ptr<const IMember> Member_Container_EP::find_member_by_name(std::string const& name) const
{
    auto it = std::find_if(m_members.begin(), m_members.end(), [&name](std::shared_ptr<const IMember> const& member) { return member->get_member_def()->get_name() == name; });
    return it != m_members.end() ? *it : std::shared_ptr<const IMember>();
}
std::shared_ptr<IMember> Member_Container_EP::find_member_by_name(std::string const& name)
{
    auto it = std::find_if(m_members.begin(), m_members.end(), [&name](std::shared_ptr<IMember> const& member) { return member->get_member_def()->get_name() == name; });
    return it != m_members.end() ? *it : std::shared_ptr<IMember>();
}


}
