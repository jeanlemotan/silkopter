#include "Lexer.h"

int xxFlexLexer::yylex()
{
    assert(0);
    return 0;
}

int xxFlexLexer::yywrap()
{
    return 1;
}


namespace ts
{
namespace ast
{

Lexer::Lexer(Builder& builder)
    : m_builder(builder)
{

}

yy::location const& Lexer::get_location() const
{
    return m_location;
}

}
}
