#pragma once

#include <memory>
#include <boost/optional.hpp>

namespace ts
{

class IAttribute;

class IAttribute_Container
{
public:
    virtual ~IAttribute_Container() = default;

    virtual auto add_attribute(std::unique_ptr<IAttribute>&& att) -> bool = 0;

    virtual auto get_attribute_count() const -> size_t = 0;

    virtual auto get_attribute(size_t idx) const -> std::shared_ptr<const IAttribute> = 0;
    virtual auto get_attribute(size_t idx) -> std::shared_ptr<IAttribute> = 0;

    virtual auto find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_attribute_by_name(std::string const& name) const -> std::shared_ptr<const IAttribute> = 0;
    virtual auto find_attribute_by_name(std::string const& name) -> std::shared_ptr<IAttribute> = 0;

    template<typename T>
    auto find_specialized_attribute_by_name() const -> std::shared_ptr<const T>;

    template<typename T>
    auto find_specialized_attribute_by_name() -> std::shared_ptr<T>;
};

}
