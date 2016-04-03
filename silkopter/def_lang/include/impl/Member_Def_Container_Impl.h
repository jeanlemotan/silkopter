#pragma once

#include <vector>
#include "IMember_Def_Container.h"

namespace ts
{

class Member_Def_Container_Impl : virtual public IMember_Def_Container
{
public:

    auto get_member_def_count() const -> size_t override;

    auto add_member_def(std::unique_ptr<Member_Def>&& member_def) -> bool override;

    auto get_member_def(size_t idx) const -> std::shared_ptr<const Member_Def> override;
    auto get_member_def(size_t idx) -> std::shared_ptr<Member_Def> override;

    auto find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_member_def_by_name(std::string const& name) const -> std::shared_ptr<const Member_Def> override;
    auto find_member_def_by_name(std::string const& name) -> std::shared_ptr<Member_Def> override;

private:
    std::vector<std::shared_ptr<Member_Def>> m_member_defs;
};

}
