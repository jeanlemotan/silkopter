#include "ep/Attribute_Container_EP.h"
#include "IAttribute.h"

namespace ts
{

auto Attribute_Container_EP::get_attribute_count() const -> size_t
{
    return m_attributes.size();
}

auto Attribute_Container_EP::add_attribute(std::unique_ptr<IAttribute>&& attribute) -> Result<void>
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

auto Attribute_Container_EP::get_attribute(size_t idx) const -> IAttribute const&
{
    return *m_attributes[idx];
}
auto Attribute_Container_EP::get_attribute(size_t idx) -> IAttribute&
{
    return *m_attributes[idx];
}

auto Attribute_Container_EP::find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    return boost::none;
}

auto Attribute_Container_EP::find_attribute_by_name(std::string const& name) const -> IAttribute const*
{
    return nullptr;
}
auto Attribute_Container_EP::find_attribute_by_name(std::string const& name) -> IAttribute*
{
    return nullptr;
}


}
