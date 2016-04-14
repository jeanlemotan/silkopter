#pragma once

namespace ts
{

class Member;

class IMember_Container
{
public:
    virtual ~IMember_Container() = default;

    virtual auto get_member_count() const -> size_t = 0;

    virtual auto get_member(size_t idx) const -> std::shared_ptr<const Member> = 0;
    virtual auto get_member(size_t idx) -> std::shared_ptr<Member> = 0;

    virtual auto find_member_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_member_by_name(std::string const& name) const -> std::shared_ptr<const Member> = 0;
    virtual auto find_member_by_name(std::string const& name) -> std::shared_ptr<Member> = 0;
};

}
