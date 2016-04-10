#pragma once

#include <memory>
#include "values/IValue.h"
#include "IInitializer.h"


namespace ts
{

template<typename Traits>
class Value_Template_EP : public virtual Traits::value_interface
{
public:
    typedef typename Traits::value_interface value_interface;
    typedef typename Traits::type_interface type_interface;
    typedef typename Traits::fundamental_type fundamental_type;

    Value_Template_EP(type_interface const& type);

    auto is_equal(IValue const& other) const -> Result<bool> override;

    auto copy_assign(IValue const& other) -> Result<void> override;
    //auto copy_assign(IInitializer const& initializer) -> Result<void> override;

    auto clone() const -> std::unique_ptr<IValue> override;

    auto get_type() const -> IType const& override;

    auto parse_from_ui_string(std::string const& str) -> Result<void> override;
    auto get_ui_string() const -> std::string override;

    auto select(Value_Selector const& selector) const -> IValue const* override;
    auto select(Value_Selector const& selector) -> IValue* override;

    auto get_specialized_type() const -> type_interface const& override;

    auto set_value(fundamental_type value) -> Result<void> override;
    auto get_value() const -> fundamental_type override;

private:
    type_interface const& m_type;
    fundamental_type m_value;
};

}

#include "impl/Value_Template_EP.inl"
