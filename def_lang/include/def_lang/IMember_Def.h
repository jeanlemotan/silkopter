#pragma once

#include <memory>
#include "ISymbol.h"
#include "IAttribute_Container.h"

namespace ts
{

class IType;
class IValue;

class IMember_Def : virtual public ISymbol, virtual public IAttribute_Container
{
public:
    virtual ~IMember_Def() = default;

    virtual std::shared_ptr<IType const> get_type() const = 0;
    virtual IValue const& get_default_value() const = 0;

    virtual std::string const& get_ui_name() const = 0;

};

}
