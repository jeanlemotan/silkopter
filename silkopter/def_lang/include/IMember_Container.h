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

    virtual size_t get_member_count() const = 0;

    virtual Result<void> add_member(std::unique_ptr<IMember>&& member) = 0;

    virtual IMember const& get_member(size_t idx) const = 0;
    virtual IMember& get_member(size_t idx) = 0;

    virtual boost::optional<size_t> find_member_idx_by_name(std::string const& name) const = 0;

    virtual IMember const* find_member_by_name(std::string const& name) const = 0;
    virtual IMember* find_member_by_name(std::string const& name) = 0;
};

}
