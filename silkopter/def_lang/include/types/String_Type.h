#include "IString_Type.h"

#include "impl/Symbol_Impl.h"
#include "impl/Attribute_Container_Impl.h"

namespace ts
{

class String_Type final : virtual public IString_Type, public Symbol_Impl, public Attribute_Container_Impl
{
public:
    typedef IString_Value value_type;
    typedef std::string fundamental_type;

    String_Type(std::string const& name);

    auto clone(std::string const& name) const -> std::unique_ptr<IType> override;

    auto get_template_instantiation_string() const -> std::string const& override;

    auto get_default_value() const -> std::shared_ptr<const IValue> override;
    auto create_value() const -> std::unique_ptr<IValue> override;

    auto get_specialized_default_value() const -> std::shared_ptr<const value_type> override;
    auto create_specialized_value() const -> std::unique_ptr<value_type> override;
};


}
