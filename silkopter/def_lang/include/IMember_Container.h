#pragma once

#include "IMember.h"

namespace ts
{

class IMember_Container
{
public:
    virtual ~IMember_Container() = default;

    virtual auto get_member_count() const -> size_t = 0;

    virtual auto get_member(size_t idx) const -> std::shared_ptr<const IMember> = 0;
    virtual auto get_member(size_t idx) -> std::shared_ptr<IMember> = 0;

    virtual auto find_member_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_member_by_name(std::string const& name) const -> std::shared_ptr<const IMember> = 0;
    virtual auto find_member_by_name(std::string const& name) -> std::shared_ptr<IMember> = 0;
};

}
