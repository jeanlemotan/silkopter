#include "impl/Attribute_Container_Impl.h"
#include "attributes/IAttribute.h"

namespace ts
{

auto Attribute_Container_Impl::get_attribute_count() const -> size_t
{
    return m_attributes.size();
}

auto Attribute_Container_Impl::add_attribute(std::unique_ptr<IAttribute>&& attribute) -> bool
{
    if (!attribute)
    {
        return false;
    }
    if (find_attribute_by_name(attribute->get_name()) != nullptr)
    {
        return false;
    }

    m_attributes.push_back(std::move(attribute));

    return true;
}

auto Attribute_Container_Impl::get_attribute(size_t idx) const -> std::shared_ptr<const IAttribute>
{
    return m_attributes[idx];
}
auto Attribute_Container_Impl::get_attribute(size_t idx) -> std::shared_ptr<IAttribute>
{
    return m_attributes[idx];
}

auto Attribute_Container_Impl::find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    return boost::none;
}

auto Attribute_Container_Impl::find_attribute_by_name(std::string const& name) const -> std::shared_ptr<const IAttribute>
{
    return nullptr;
}
auto Attribute_Container_Impl::find_attribute_by_name(std::string const& name) -> std::shared_ptr<IAttribute>
{
    return nullptr;
}


}
