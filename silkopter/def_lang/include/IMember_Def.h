#pragma once

#include <memory>
#include "ISymbol.h"
#include "attributes/IAttribute_Container.h"

namespace ts
{

class IType;
class IValue;

class IMember_Def : virtual public ISymbol, virtual public IAttribute_Container
{
public:
    virtual ~IMember_Def() = default;

    virtual auto get_type() const -> IType const& = 0;
    virtual auto get_default_value() const -> IValue const& = 0;

    virtual auto get_ui_name() const -> std::string const& = 0;

};

}
