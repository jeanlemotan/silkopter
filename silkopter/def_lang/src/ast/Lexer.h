#pragma once

#undef yyFlexLexer
#define yyFlexLexer xxFlexLexer
#include <FlexLexer.h>

namespace ast
{
class Builder;
}

#include "yy_parser.hpp"

namespace ast
{

class Lexer : public xxFlexLexer
{
public:
    Lexer(Builder& builder);

    int yylex()
    {
        //not used
        assert(0);
        return 0;
    }

    auto astyylex() -> yy::parser::symbol_type;

    auto get_location() const -> yy::location const&;

private:
    yy::location m_location;
    Builder& m_builder;
};

}
