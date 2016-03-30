#pragma once

namespace ts
{

class IMember_Def;

class IMember_Def_Container
{
public:
    virtual ~IMember_Def_Container() = default;

    virtual auto get_member_def_count() const -> size_t = 0;

    virtual auto get_member_def(size_t idx) const -> std::shared_ptr<const IMember_Def> = 0;
    virtual auto get_member_def(size_t idx) -> std::shared_ptr<IMember_Def> = 0;

    virtual auto find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_member_def_by_name(std::string const& name) const -> std::shared_ptr<const IMember_Def> = 0;
    virtual auto find_member_def_by_name(std::string const& name) -> std::shared_ptr<IMember_Def> = 0;
};

}
