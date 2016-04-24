#include "def_lang/ep/Member_Def_Container_EP.h"
#include "def_lang/IMember_Def.h"

namespace ts
{

size_t Member_Def_Container_EP::get_member_def_count() const
{
    return m_member_defs.size();
}

Member_Def_Container_EP::~Member_Def_Container_EP()
{

}

Result<void> Member_Def_Container_EP::add_member_def(std::shared_ptr<const IMember_Def> member_def)
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

std::shared_ptr<IMember_Def const> Member_Def_Container_EP::get_member_def(size_t idx) const
{
    return m_member_defs[idx];
}

boost::optional<size_t> Member_Def_Container_EP::find_member_def_idx_by_name(std::string const& name) const
{
    auto it = std::find_if(m_member_defs.begin(), m_member_defs.end(), [&name](std::shared_ptr<const IMember_Def> const& member_def) { return member_def->get_name() == name; });
    return it != m_member_defs.end() ? boost::optional<size_t>(std::distance(m_member_defs.begin(), it)) : boost::optional<size_t>();
}

std::shared_ptr<IMember_Def const> Member_Def_Container_EP::find_member_def_by_name(std::string const& name) const
{
    auto it = std::find_if(m_member_defs.begin(), m_member_defs.end(), [&name](std::shared_ptr<const IMember_Def> const& member_def) { return member_def->get_name() == name; });
    return it != m_member_defs.end() ? *it : std::shared_ptr<const IMember_Def>();
}


}
