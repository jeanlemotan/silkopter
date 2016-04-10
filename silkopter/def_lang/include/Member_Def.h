#pragma once

#include <memory>
#include "impl/Symbol_EP.h"

namespace ts
{

class IType;
class IValue;

class Member_Def final : public Symbol_EP
{
public:

    Member_Def(std::string const& name, IType const& type, std::unique_ptr<const IValue> default_value);
    ~Member_Def();

    auto get_type() const -> IType const&;
    auto get_default_value() const -> IValue const&;

private:
    IType const& m_type;
    std::unique_ptr<const IValue> m_default_value;
};

}
