#pragma once

#include <memory>
#include "IValue.h"
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

    Value_Template_EP(std::shared_ptr<type_interface const> type);

    Result<bool> is_equal(IValue const& other) const override;

    Result<void> copy_assign(IValue const& other) override;
    //Result<void> copy_assign(IInitializer const& initializer) override;

    //std::shared_ptr<IValue> clone() const override;

    std::shared_ptr<IType const> get_type() const override;

    Result<void> parse_from_ui_string(std::string const& str) override;
    Result<std::string> get_ui_string() const override;

    std::shared_ptr<const IValue> select(Value_Selector const& selector) const override;
    std::shared_ptr<IValue> select(Value_Selector const& selector) override;

    std::shared_ptr<type_interface const> get_specialized_type() const override;

    Result<void> set_value(fundamental_type value) override;
    fundamental_type get_value() const override;

private:
    std::shared_ptr<type_interface const> m_type;
    fundamental_type m_value;
};

}

#include "ep/Value_Template_EP.inl"
