#pragma once

#include "def_lang/IEnum_Item.h"
#include "def_lang/ep/Symbol_EP.h"
#include "def_lang/ep/Attribute_Container_EP.h"

namespace ts
{

class Enum_Item final: virtual public IEnum_Item, public Symbol_EP, public Attribute_Container_EP
{
public:
    Enum_Item(std::string const& name, int64_t integral_value);

    std::shared_ptr<const IValue> get_initializer_value() const override;

    int64_t get_integral_value() const override;
    std::string const& get_ui_name() const override;
    bool is_default() const override;

protected:
    Result<void> validate_attribute(IAttribute const& attribute) override;

private:
    std::string m_ui_name;
    int64_t m_integral_value = 0;
    bool m_is_default = false;
};

}
