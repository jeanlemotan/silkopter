#include "IVector_Type.h"

#include "ep/Symbol_EP.h"
#include "ep/Attribute_Container_EP.h"

namespace ts
{

class Vector_Type final : virtual public IVector_Type, public Symbol_EP, public Attribute_Container_EP, public std::enable_shared_from_this<Vector_Type>
{
public:
    typedef IVector_Value value_type;

    Vector_Type(std::string const& name);

    Result<void> init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) override;

    std::unique_ptr<IType> clone(std::string const& name) const override;

    std::string get_template_instantiation_string() const override;

    std::unique_ptr<IValue> create_value() const override;
    std::unique_ptr<value_type> create_specialized_value() const override;

protected:
    Result<void> validate_attribute(IAttribute const& attribute) override;

public:
    std::shared_ptr<const IType> m_inner_type;
};


}
