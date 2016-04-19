#pragma once

#include <vector>
#include "IMember_Container.h"

namespace ts
{

class Member_Container_EP : virtual public IMember_Container
{
public:

    ~Member_Container_EP();

    size_t get_member_count() const override;

    Result<void> add_member(std::shared_ptr<IMember> member) override;

    IMember const& get_member(size_t idx) const override;
    IMember& get_member(size_t idx) override;

    boost::optional<size_t> find_member_idx_by_name(std::string const& name) const override;

    IMember const* find_member_by_name(std::string const& name) const override;
    IMember* find_member_by_name(std::string const& name) override;

private:
    std::vector<std::shared_ptr<IMember>> m_members;
};

}
