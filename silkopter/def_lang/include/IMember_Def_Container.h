#pragma once

#include <stdint.h>
#include <memory>
#include <boost/optional.hpp>
#include "Result.h"

namespace ts
{

class IMember_Def;

class IMember_Def_Container
{
public:
    virtual ~IMember_Def_Container() = default;

    virtual auto get_member_def_count() const -> size_t = 0;

    virtual auto add_member_def(std::unique_ptr<IMember_Def>&& member_def) -> Result<void> = 0;

    virtual auto get_member_def(size_t idx) const -> IMember_Def const& = 0;
    virtual auto get_member_def(size_t idx) -> IMember_Def& = 0;

    virtual auto find_member_def_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_member_def_by_name(std::string const& name) const -> IMember_Def const* = 0;
    virtual auto find_member_def_by_name(std::string const& name) -> IMember_Def* = 0;
};

}
