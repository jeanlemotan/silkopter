%{

#include "yy_parser.hpp"
#include "ast/Lexer.h"

#undef yywrap
#define yywrap() 1

yy::location loc;
%}

%option noyywrap nounput batch debug noinput

%{
  // Code run each time a pattern is matched.
#define YY_USER_ACTION  loc.columns (yyleng);

#define yyterminate() return yy::parser::make_TEND(loc)

// Tell Flex the lexer's prototype ...
#define YY_DECL yy::parser::symbol_type ast::Lexer::astyylex()

// ... and declare it for the parser's sake.
//YY_DECL;
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

[ \t]+                  loc.step();
[\n]+                   loc.lines(yyleng); loc.step();
"import"                return yy::parser::make_TIMPORT(loc);
"alias"                 return yy::parser::make_TALIAS(loc);
"struct"                return yy::parser::make_TSTRUCT(loc);
"public"                return yy::parser::make_TPUBLIC(loc);
"private"               return yy::parser::make_TPRIVATE(loc);
"protected"             return yy::parser::make_TPROTECTED(loc);
"const"                 return yy::parser::make_TCONST(loc);
"namespace"             return yy::parser::make_TNAMESPACE(loc);
"false" 	            return yy::parser::make_TFALSE(loc);
"true" 	                return yy::parser::make_TTRUE(loc);
[a-zA-Z_][a-zA-Z0-9_]*  return yy::parser::make_TIDENTIFIER(yytext, loc);
[0-9]+                  return yy::parser::make_TINTEGER_LITERAL(strtol(yytext, nullptr, 10), loc);
[0-9]+\.[0-9]*          return yy::parser::make_TDOUBLE_LITERAL(strtod(yytext, nullptr), loc);
[0-9]+\.[0-9]*f         return yy::parser::make_TFLOAT_LITERAL(strtof(yytext, nullptr), loc);
\"(\\.|[^"])*\"         return yy::parser::make_TSTRING_LITERAL(yytext, loc);
"="                     return yy::parser::make_TEQUAL(loc);
"("                     return yy::parser::make_TLPARENTHESIS(loc);
")"                     return yy::parser::make_TRPARENTHESIS(loc);
"{"                     return yy::parser::make_TLBRACE(loc);
"}"                     return yy::parser::make_TRBRACE(loc);
"["                     return yy::parser::make_TLBRAKET(loc);
"]"                     return yy::parser::make_TRBRAKET(loc);
"<"                     return yy::parser::make_TLANGLED_BRAKET(loc);
">"                     return yy::parser::make_TRANGLED_BRAKET(loc);
"::"                    return yy::parser::make_TNAMESPACE_SEPARATOR(loc);
":"                     return yy::parser::make_TCOLON(loc);
";"                     return yy::parser::make_TSEMICOLON(loc);
"."                     return yy::parser::make_TDOT(loc);
","                     return yy::parser::make_TCOMMA(loc);
.                       { std::cout << "Unknown token!\n"; yyterminate(); }
<<EOF>>                 {
                            if (!m_builder.end_file())
                            {
                                yyterminate();
                            }
                        }
%%

