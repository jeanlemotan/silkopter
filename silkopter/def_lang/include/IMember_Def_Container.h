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

    virtual size_t get_member_def_count() const = 0;

    virtual Result<void> add_member_def(std::unique_ptr<IMember_Def>&& member_def) = 0;

    virtual std::shared_ptr<IMember_Def const> get_member_def(size_t idx) const = 0;
    virtual std::shared_ptr<IMember_Def> get_member_def(size_t idx) = 0;

    virtual boost::optional<size_t> find_member_def_idx_by_name(std::string const& name) const = 0;

    virtual std::shared_ptr<IMember_Def const> find_member_def_by_name(std::string const& name) const = 0;
    virtual std::shared_ptr<IMember_Def> find_member_def_by_name(std::string const& name) = 0;
};

}
