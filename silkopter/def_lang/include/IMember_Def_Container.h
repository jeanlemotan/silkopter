#pragma once

#include <stdint.h>
#include <memory>
#include <boost/optional.hpp>

namespace ts
{

class Member_Def;

class IMember_Def_Container
{
public:
    virtual ~IMember_Def_Container() = default;

    virtual auto get_member_def_count() const -> size_t = 0;

    virtual auto add_member_def(std::unique_ptr<Member_Def>&& member_def) -> bool = 0;

    virtual auto get_member_def(size_t idx) const -> std::shared_ptr<const Member_Def> = 0;
    virtual auto get_member_def(size_t idx) -> std::shared_ptr<Member_Def> = 0;

    virtual auto find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_member_def_by_name(std::string const& name) const -> std::shared_ptr<const Member_Def> = 0;
    virtual auto find_member_def_by_name(std::string const& name) -> std::shared_ptr<Member_Def> = 0;
};

}
