#pragma once

#include <memory>
#include "IMember_Def.h"
#include "ep/Symbol_EP.h"
#include "ep/Attribute_Container_EP.h"
#include "impl/UI_Name_Attribute.h"

namespace ts
{

class IType;
class IValue;

class Member_Def final : virtual public IMember_Def, public Symbol_EP, public Attribute_Container_EP
{
public:

    Member_Def(std::string const& name, IType const& type, std::unique_ptr<const IValue> default_value);
    ~Member_Def();

    auto get_type() const -> IType const&;
    auto get_default_value() const -> IValue const&;

    auto get_ui_name() const -> std::string const&;

protected:
    auto validate_attribute(IAttribute const& attribute) -> Result<void>;

private:
    IType const& m_type;
    std::unique_ptr<const IValue> m_default_value;
    std::string m_ui_name;
};

}
