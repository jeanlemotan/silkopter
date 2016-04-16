#pragma once

#include <vector>
#include "IMember_Def_Container.h"

namespace ts
{

class Member_Def_Container_EP : virtual public IMember_Def_Container
{
public:

    ~Member_Def_Container_EP();

    auto get_member_def_count() const -> size_t override;

    auto add_member_def(std::unique_ptr<IMember_Def>&& member_def) -> Result<void> override;

    auto get_member_def(size_t idx) const -> IMember_Def const& override;
    auto get_member_def(size_t idx) -> IMember_Def& override;

    auto find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_member_def_by_name(std::string const& name) const -> IMember_Def const* override;
    auto find_member_def_by_name(std::string const& name) -> IMember_Def* override;

private:
    std::vector<std::shared_ptr<IMember_Def>> m_member_defs;
};

}
