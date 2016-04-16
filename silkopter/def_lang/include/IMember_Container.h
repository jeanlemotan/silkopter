#pragma once

#include <stdint.h>
#include <memory>
#include <boost/optional.hpp>
#include "Result.h"

namespace ts
{

class IMember;

class IMember_Container
{
public:
    virtual ~IMember_Container() = default;

    virtual auto get_member_count() const -> size_t = 0;

    virtual auto add_member(std::unique_ptr<IMember>&& member) -> Result<void> = 0;

    virtual auto get_member(size_t idx) const -> IMember const& = 0;
    virtual auto get_member(size_t idx) -> IMember& = 0;

    virtual auto find_member_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_member_by_name(std::string const& name) const -> IMember const* = 0;
    virtual auto find_member_by_name(std::string const& name) -> IMember* = 0;
};

}
