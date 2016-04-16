#pragma once

#include <vector>
#include "IMember_Container.h"

namespace ts
{

class Member_Container_EP : virtual public IMember_Container
{
public:

    ~Member_Container_EP();

    auto get_member_count() const -> size_t override;

    auto add_member(std::unique_ptr<IMember>&& member) -> Result<void> override;

    auto get_member(size_t idx) const -> IMember const& override;
    auto get_member(size_t idx) -> IMember& override;

    auto find_member_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_member_by_name(std::string const& name) const -> IMember const* override;
    auto find_member_by_name(std::string const& name) -> IMember* override;

private:
    std::vector<std::shared_ptr<IMember>> m_members;
};

}
