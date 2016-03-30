#pragma once

#include <memory>

namespace ts
{

class ISymbol;

class IDeclaration_Scope
{
public:
    virtual ~IDeclaration_Scope() = default;

    virtual auto add_symbol(std::unique_ptr<ISymbol> symbol) -> bool = 0;

    virtual auto get_symbol_count() -> const size_t = 0;
    virtual auto get_symbol(size_t idx) const -> std::shared_ptr<const ISymbol> = 0;
    virtual auto get_symbol(size_t idx) -> std::shared_ptr<ISymbol> = 0;
};

}
