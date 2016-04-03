#include "impl/Member_Def_Container_Impl.h"
#include "Member_Def.h"

namespace ts
{

auto Member_Def_Container_Impl::get_member_def_count() const -> size_t
{
    return m_member_defs.size();
}

auto Member_Def_Container_Impl::add_member_def(std::unique_ptr<Member_Def>&& member_def) -> bool
{
    if (!member_def)
    {
        return false;
    }
    if (find_member_def_by_name(member_def->get_name()) != nullptr)
    {
        return false;
    }

    m_member_defs.push_back(std::move(member_def));

    return true;
}

auto Member_Def_Container_Impl::get_member_def(size_t idx) const -> std::shared_ptr<const Member_Def>
{
    return m_member_defs[idx];
}
auto Member_Def_Container_Impl::get_member_def(size_t idx) -> std::shared_ptr<Member_Def>
{
    return m_member_defs[idx];
}

auto Member_Def_Container_Impl::find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    return boost::none;
}

auto Member_Def_Container_Impl::find_member_def_by_name(std::string const& name) const -> std::shared_ptr<const Member_Def>
{
    return nullptr;
}
auto Member_Def_Container_Impl::find_member_def_by_name(std::string const& name) -> std::shared_ptr<Member_Def>
{
    return nullptr;
}


}
