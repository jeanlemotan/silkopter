#pragma once

#include <memory>
#include <vector>
#include "def_lang/IAttribute_Container.h"

namespace ts
{

class IAttribute;

class Attribute_Container_EP : virtual public IAttribute_Container
{
public:

    size_t get_attribute_count() const override;

    Result<void> add_attribute(std::shared_ptr<const IAttribute> attribute) override;
    std::shared_ptr<IAttribute const> get_attribute(size_t idx) const override;
    boost::optional<size_t> find_attribute_idx_by_name(std::string const& name) const override;
    std::shared_ptr<IAttribute const> find_attribute_by_name(std::string const& name) const override;

protected:

    virtual Result<void> validate_attribute(IAttribute const& attribute) = 0;

private:
    std::vector<std::shared_ptr<const IAttribute>> m_attributes;
};

}
