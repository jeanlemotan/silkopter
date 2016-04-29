#include "def_lang/impl/Namespace.h"

namespace ts
{

Namespace::Namespace(std::string const& name)
    : Declaration_Scope_EP(name)
{
}

Result<void> Namespace::validate_symbol(std::shared_ptr<const ISymbol> symbol)
{
    return ts::success;
}

}
