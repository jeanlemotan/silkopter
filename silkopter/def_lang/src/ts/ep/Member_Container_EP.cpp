#include "ep/Member_Container_EP.h"
#include "IMember.h"
#include "IMember_Def.h"

namespace ts
{

Member_Container_EP::~Member_Container_EP()
{

}

auto Member_Container_EP::get_member_count() const -> size_t
{
    return m_members.size();
}

auto Member_Container_EP::add_member(std::unique_ptr<IMember>&& member) -> Result<void>
{
    if (!member)
    {
        return Error("Cannot add null member");
    }
    if (find_member_by_name(member->get_member_def().get_name()) != nullptr)
    {
        return Error("Duplicate member name: " + member->get_member_def().get_name());
    }

    m_members.push_back(std::move(member));

    return success;
}

auto Member_Container_EP::get_member(size_t idx) const -> IMember const&
{
    return *m_members[idx];
}
auto Member_Container_EP::get_member(size_t idx) -> IMember&
{
    return *m_members[idx];
}

auto Member_Container_EP::find_member_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    return boost::none;
}

auto Member_Container_EP::find_member_by_name(std::string const& name) const -> IMember const*
{
    return nullptr;
}
auto Member_Container_EP::find_member_by_name(std::string const& name) -> IMember*
{
    return nullptr;
}


}
