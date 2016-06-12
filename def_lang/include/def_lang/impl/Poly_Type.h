#include "def_lang/IPoly_Type.h"

#include "def_lang/ep/Symbol_EP.h"
#include "def_lang/ep/Attribute_Container_EP.h"
#include "def_lang/Qualified_Type.h"

namespace ts
{

class IStruct_Type;

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
    std::shared_ptr<IType> alias(std::string const& name) const override;
    std::shared_ptr<const IType> get_aliased_type() const override;

    std::shared_ptr<const Qualified_Type> get_inner_qualified_type() const override;
    std::vector<std::shared_ptr<const Qualified_Type>> get_all_inner_qualified_types() const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<value_type> create_specialized_value() const override;

protected:
    Result<void> validate_attribute(IAttribute const& attribute) override;

private:
    std::shared_ptr<const Qualified_Type> m_inner_qualified_type;
    std::shared_ptr<const IStruct_Type> m_inner_type;
    std::shared_ptr<const IType> m_aliased_type;
    std::string m_ui_name;
    Symbol_Path m_native_type;
};


}
