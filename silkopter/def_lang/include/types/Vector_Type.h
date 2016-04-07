#include "IVector_Type.h"

#include "impl/Symbol_Impl.h"
#include "impl/Attribute_Container_Impl.h"

namespace ts
{

class Vector_Type final : virtual public IVector_Type, public Symbol_Impl, public Attribute_Container_Impl
{
public:
    typedef IVector_Value value_type;

    Vector_Type(std::string const& name);

    auto init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) -> bool override;

    auto clone(std::string const& name) const -> std::unique_ptr<IType> override;

    auto get_template_instantiation_string() const -> std::string const& override;

    auto get_default_value() const -> std::shared_ptr<const IValue> override;
    auto create_value() const -> std::unique_ptr<IValue> override;

    auto get_specialized_default_value() const -> std::shared_ptr<const value_type> override;
    auto create_specialized_value() const -> std::unique_ptr<value_type> override;

public:
    std::shared_ptr<const IType> m_inner_type;
};


}
