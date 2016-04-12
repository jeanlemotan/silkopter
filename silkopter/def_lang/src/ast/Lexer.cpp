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


namespace ast
{

Lexer::Lexer(Builder& builder)
    : m_builder(builder)
{

}

auto Lexer::get_location() const -> yy::location const&
{
    return m_location;
}

}
