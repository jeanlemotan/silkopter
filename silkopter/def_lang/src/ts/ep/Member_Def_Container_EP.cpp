#include "ep/Member_Def_Container_EP.h"
#include "IMember_Def.h"

namespace ts
{

auto Member_Def_Container_EP::get_member_def_count() const -> size_t
{
    return m_member_defs.size();
}

Member_Def_Container_EP::~Member_Def_Container_EP()
{

}

auto Member_Def_Container_EP::add_member_def(std::unique_ptr<IMember_Def>&& member_def) -> Result<void>
{
    if (!member_def)
    {
        return Error("Cannot add null member");
    }
    if (find_member_def_by_name(member_def->get_name()) != nullptr)
    {
        return Error("Duplicate member name: " + member_def->get_name());
    }

    m_member_defs.push_back(std::move(member_def));

    return success;
}

auto Member_Def_Container_EP::get_member_def(size_t idx) const -> IMember_Def const&
{
    return *m_member_defs[idx];
}
auto Member_Def_Container_EP::get_member_def(size_t idx) -> IMember_Def&
{
    return *m_member_defs[idx];
}

auto Member_Def_Container_EP::find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    return boost::none;
}

auto Member_Def_Container_EP::find_member_def_by_name(std::string const& name) const -> IMember_Def const*
{
    return nullptr;
}
auto Member_Def_Container_EP::find_member_def_by_name(std::string const& name) -> IMember_Def*
{
    return nullptr;
}


}
