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

    size_t get_attribute_count() const override;

    Result<void> add_attribute(std::unique_ptr<IAttribute>&& attribute) override;

    std::shared_ptr<IAttribute const> get_attribute(size_t idx) const override;
    std::shared_ptr<IAttribute> get_attribute(size_t idx) override;

    boost::optional<size_t> find_attribute_idx_by_name(std::string const& name) const override;

    std::shared_ptr<IAttribute const> find_attribute_by_name(std::string const& name) const override;
    std::shared_ptr<IAttribute> find_attribute_by_name(std::string const& name) override;

protected:

    virtual Result<void> validate_attribute(IAttribute const& attribute) = 0;

private:
    std::vector<std::shared_ptr<IAttribute>> m_attributes;
};

}
