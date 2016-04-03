#pragma once

namespace ts
{

class ISymbol
{
public:
    virtual ~ISymbol() = default;

    virtual auto get_name() const -> std::string const& = 0;
    //virtual Declaration_Scope const& get_declaration_scope() const = 0;
};

}
