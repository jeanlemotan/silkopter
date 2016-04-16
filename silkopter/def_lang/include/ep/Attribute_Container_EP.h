#pragma once

#include <memory>
#include <vector>
#include "IAttribute_Container.h"

namespace ts
{

class IAttribute;

class Attribute_Container_EP : virtual public IAttribute_Container
{
public:

    auto get_attribute_count() const -> size_t override;

    auto add_attribute(std::unique_ptr<IAttribute>&& attribute) -> Result<void> override;

    auto get_attribute(size_t idx) const -> IAttribute const& override;
    auto get_attribute(size_t idx) -> IAttribute& override;

    auto find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_attribute_by_name(std::string const& name) const -> IAttribute const* override;
    auto find_attribute_by_name(std::string const& name) -> IAttribute* override;

protected:

    virtual auto validate_attribute(IAttribute const& attribute) -> Result<void> = 0;

private:
    std::vector<std::shared_ptr<IAttribute>> m_attributes;
};

}
