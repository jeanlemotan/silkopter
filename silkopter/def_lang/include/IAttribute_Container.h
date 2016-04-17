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

    virtual Result<void> add_attribute(std::unique_ptr<IAttribute>&& att) = 0;

    virtual size_t get_attribute_count() const = 0;

    virtual std::shared_ptr<IAttribute const> get_attribute(size_t idx) const = 0;
    virtual std::shared_ptr<IAttribute> get_attribute(size_t idx) = 0;

    virtual boost::optional<size_t> find_attribute_idx_by_name(std::string const& name) const = 0;

    virtual std::shared_ptr<IAttribute const> find_attribute_by_name(std::string const& name) const = 0;
    virtual std::shared_ptr<IAttribute> find_attribute_by_name(std::string const& name) = 0;

    template<typename T>
    std::shared_ptr<T const> find_specialized_attribute_by_name() const;

    template<typename T>
    std::shared_ptr<T> find_specialized_attribute_by_name();
};

}
