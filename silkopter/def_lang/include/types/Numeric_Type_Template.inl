namespace ts
{

template<typename Traits>
Numeric_Type_Template<Traits>::Numeric_Type_Template(std::string const& name)
    : Type_Template_EP<Traits>(name)
{

}

template<typename Traits>
auto Numeric_Type_Template<Traits>::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

}
