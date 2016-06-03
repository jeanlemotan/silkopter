#include "def_lang/IPoly_Type.h"

#include "def_lang/ep/Symbol_EP.h"
#include "def_lang/ep/Attribute_Container_EP.h"

namespace ts
{

class Poly_Type final : virtual public IPoly_Type, public Symbol_EP, public Attribute_Container_EP, public std::enable_shared_from_this<Poly_Type>
{
public:
    typedef IPoly_Value value_type;

    Poly_Type(std::string const& name);
    Poly_Type(Poly_Type const& other, std::string const& name);

    Result<void> init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) override;

    std::string const& get_ui_name() const override;
    Symbol_Path get_native_type() const override;

    std::shared_ptr<IType> clone(std::string const& name) const override;

    std::string get_template_instantiation_string() const override;

    std::shared_ptr<const IType> get_inner_type() const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<value_type> create_specialized_value() const override;

    std::string generate_serialization_code() const;

protected:
    Result<void> validate_attribute(IAttribute const& attribute) override;

private:
    std::shared_ptr<const IType> m_inner_type;
    std::string m_ui_name;
    Symbol_Path m_native_type;
};


}
