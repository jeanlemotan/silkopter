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

    yy::parser::symbol_type astyylex();

private:
    Builder& m_builder;
};

}
