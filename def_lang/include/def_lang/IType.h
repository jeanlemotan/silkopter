#pragma once

#include "ISymbol.h"
#include "IAttribute_Container.h"
#include "ITemplate_Argument.h"

namespace ts
{

class IValue;

class IType : virtual public ISymbol, virtual public IAttribute_Container
{
public:
    IType() = default;
    IType(IType const&) = delete;
    IType& operator=(IType const&) = delete;

    virtual ~IType() = default;

    virtual std::string const& get_ui_name() const = 0;
    virtual Symbol_Path get_native_type() const = 0;

    virtual std::shared_ptr<IType> clone(std::string const& name) const = 0;
    virtual std::shared_ptr<IType> alias(std::string const& name) const = 0;

    virtual std::shared_ptr<const IType> get_aliased_type() const = 0;

    virtual std::shared_ptr<IValue> create_value() const = 0;
};

}
