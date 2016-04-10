#pragma once

#include <vector>
#include "attributes/IAttribute_Container.h"

namespace ts
{

class Attribute_Container_EP : virtual public IAttribute_Container
{
public:

    auto get_attribute_count() const -> size_t override;

    auto add_attribute(std::unique_ptr<IAttribute>&& attribute) -> bool override;

    auto get_attribute(size_t idx) const -> std::shared_ptr<const IAttribute> override;
    auto get_attribute(size_t idx) -> std::shared_ptr<IAttribute> override;

    auto find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_attribute_by_name(std::string const& name) const -> std::shared_ptr<const IAttribute> override;
    auto find_attribute_by_name(std::string const& name) -> std::shared_ptr<IAttribute> override;

private:
    std::vector<std::shared_ptr<IAttribute>> m_attributes;
};

}
