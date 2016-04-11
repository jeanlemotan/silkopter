#pragma once

#include <memory>
#include <boost/optional.hpp>
#include "Result.h"

namespace ts
{

class IAttribute;

class IAttribute_Container
{
public:
    virtual ~IAttribute_Container() = default;

    virtual auto add_attribute(std::unique_ptr<IAttribute>&& att) -> Result<void> = 0;

    virtual auto get_attribute_count() const -> size_t = 0;

    virtual auto get_attribute(size_t idx) const -> IAttribute const& = 0;
    virtual auto get_attribute(size_t idx) -> IAttribute& = 0;

    virtual auto find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_attribute_by_name(std::string const& name) const -> IAttribute const* = 0;
    virtual auto find_attribute_by_name(std::string const& name) -> IAttribute* = 0;

    template<typename T>
    auto find_specialized_attribute_by_name() const -> T const*;

    template<typename T>
    auto find_specialized_attribute_by_name() -> T*;
};

}
