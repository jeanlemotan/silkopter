#include "def_lang/ep/Attribute_Container_EP.h"
#include "def_lang/IAttribute.h"

namespace ts
{

size_t Attribute_Container_EP::get_attribute_count() const
{
    return m_attributes.size();
}

Result<void> Attribute_Container_EP::add_attribute(std::shared_ptr<const IAttribute> attribute)
{
    if (!attribute)
    {
        return Error("Cannot add null attribute");
    }
    if (find_attribute_by_name(attribute->get_name()) != nullptr)
    {
        return Error("Duplicated attribute " + attribute->get_name());
    }

    auto result = validate_attribute(*attribute);
    if (result != success)
    {
        return result.error();
    }

    m_attributes.push_back(std::move(attribute));

    return success;
}

std::shared_ptr<IAttribute const> Attribute_Container_EP::get_attribute(size_t idx) const
{
    return m_attributes[idx];
}
boost::optional<size_t> Attribute_Container_EP::find_attribute_idx_by_name(std::string const& name) const
{
    return boost::none;
}
std::shared_ptr<IAttribute const> Attribute_Container_EP::find_attribute_by_name(std::string const& name) const
{
    return nullptr;
}


}
