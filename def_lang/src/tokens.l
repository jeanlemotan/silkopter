identifier ([A-Za-z]|_)([A-Za-z]|[0-9]|_)*
scope [:][:]
%x BLOCK_COMMENT
%{

#include "yy_parser.hpp"
#include "ts/ast/Lexer.h"

#undef yywrap
#define yywrap() 1

%}

%option noyywrap nounput batch debug noinput

%{
  // Code run each time a pattern is matched.
#define YY_USER_ACTION  m_location.columns (yyleng);

#define yyterminate() return yy::parser::make_TEND(m_location)

// Tell Flex the lexer's prototype ...
#define YY_DECL yy::parser::symbol_type ts::ast::Lexer::astyylex()

// ... and declare it for the parser's sake.
//YY_DECL;
%}

%%

%{
  // Code run each time yylex is called.
  m_location.step ();
%}

[ \t]+                  m_location.step();
[\n]+                   m_location.lines(yyleng); m_location.step();
"import"                return yy::parser::make_TIMPORT(m_location);
"alias"                 return yy::parser::make_TALIAS(m_location);
"struct"                return yy::parser::make_TSTRUCT(m_location);
"enum"                  return yy::parser::make_TENUM(m_location);
"public"                return yy::parser::make_TPUBLIC(m_location);
"private"               return yy::parser::make_TPRIVATE(m_location);
"protected"             return yy::parser::make_TPROTECTED(m_location);
"const"                 return yy::parser::make_TCONST(m_location);
"namespace"             return yy::parser::make_TNAMESPACE(m_location);
"false" 	            return yy::parser::make_TFALSE(m_location);
"true" 	                return yy::parser::make_TTRUE(m_location);
{identifier}            return yy::parser::make_TIDENTIFIER(yytext, m_location);
{scope}?{identifier}({scope}{identifier})+   return yy::parser::make_TIDENTIFIER_PATH(yytext, m_location);
-?[0-9]+           return yy::parser::make_TINTEGER_LITERAL(strtol(yytext, nullptr, 10), m_location);
-?[0-9]+\.[0-9]*   return yy::parser::make_TDOUBLE_LITERAL(strtod(yytext, nullptr), m_location);
-?[0-9]+\.[0-9]*f  return yy::parser::make_TFLOAT_LITERAL(strtof(yytext, nullptr), m_location);
\"(\\.|[^"])*\"         return yy::parser::make_TSTRING_LITERAL(yytext, m_location);
"="                     return yy::parser::make_TEQUAL(m_location);
"("                     return yy::parser::make_TLPARENTHESIS(m_location);
")"                     return yy::parser::make_TRPARENTHESIS(m_location);
"{"                     return yy::parser::make_TLBRACE(m_location);
"}"                     return yy::parser::make_TRBRACE(m_location);
"["                     return yy::parser::make_TLBRAKET(m_location);
"]"                     return yy::parser::make_TRBRAKET(m_location);
"<"                     return yy::parser::make_TLANGLED_BRAKET(m_location);
">"                     return yy::parser::make_TRANGLED_BRAKET(m_location);
{scope}                 return yy::parser::make_TNAMESPACE_SEPARATOR(m_location);
":"                     return yy::parser::make_TCOLON(m_location);
";"                     return yy::parser::make_TSEMICOLON(m_location);
"."                     return yy::parser::make_TDOT(m_location);
","                     return yy::parser::make_TCOMMA(m_location);
\/\*                    {
                            BEGIN(BLOCK_COMMENT);
                        }
<BLOCK_COMMENT>\*\/		{
                            BEGIN(INITIAL);
                        }
<BLOCK_COMMENT>\n		{ }
<BLOCK_COMMENT>.        ;
\/\/([^\n])*            ;
.                       { std::cout << "Unknown token!\n"; yyterminate(); }
<<EOF>>                 {
                            if (!m_builder.end_file())
                            {
                                yyterminate();
                            }
                        }
%%

