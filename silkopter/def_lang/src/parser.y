%{
#include <stdio.h>
#include <string>
#include <iostream>
//    #include "node.h"
//    NBlock *programBlock; /* the top level root node of our final AST */

    extern int yylex();
    void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

/* use newer C++ skeleton file */
//%skeleton "lalr1.cc"

/* verbose error messages */
%error-verbose

/* Represents the many different ways we can access our data */
%union {
    std::string* string;
    int token;
}

%token <token> TIMPORT
%token <token> TALIAS
%token <token> TSTRUCT
%token <token> TSTRING
%token <token> TVECTOR
%token <token> TFLOAT
%token <token> TDOUBLE
%token <token> TBOOL
%token <token> TUINT8_T
%token <token> TINT8_T
%token <token> TUINT16_T
%token <token> TINT16_T
%token <token> TUINT32_T
%token <token> TINT32_T
%token <token> TUINT64_T
%token <token> TINT64_T
%token <token> TVEC2F
%token <token> TVEC2D
%token <token> TVEC2S8
%token <token> TVEC2U8
%token <token> TVEC2S16
%token <token> TVEC2U16
%token <token> TVEC2S32
%token <token> TVEC2U32
%token <token> TVEC3F
%token <token> TVEC3D
%token <token> TVEC3S8
%token <token> TVEC3U8
%token <token> TVEC3S16
%token <token> TVEC3U16
%token <token> TVEC3S32
%token <token> TVEC3U32
%token <token> TVEC4F
%token <token> TVEC4D
%token <token> TVEC4S8
%token <token> TVEC4U8
%token <token> TVEC4S16
%token <token> TVEC4U16
%token <token> TVEC4S32
%token <token> TVEC4U32
%token <token> TQUATF
%token <token> TQUATD
%token <token> TMAT2F
%token <token> TMAT2D
%token <token> TMAT3F
%token <token> TMAT3D
%token <token> TMAT4F
%token <token> TMAT4D
%token <token> TCOLOR_RGB
%token <token> TCOLOR_RGBA
%token <token> TMIN
%token <token> TMAX
%token <token> TPUBLIC
%token <token> TPRIVATE
%token <token> TPROTECTED
%token <token> TCONST
%token <token> TUI_CONTROL
%token <token> TUI_NAME
%token <token> TUI_CONST
%token <token> TNAMESPACE
%token <token> TFALSE;
%token <token> TTRUE;
%token <token> TEQUAL
%token <token> TLPARENTHESIS
%token <token> TRPARENTHESIS
%token <token> TLBRACE
%token <token> TRBRACE
%token <token> TLBRAKET
%token <token> TRBRAKET
%token <token> TLANGLED_BRAKET
%token <token> TRANGLED_BRAKET
%token <token> TNAMESPACE_SEPARATOR
%token <token> TCOLON
%token <token> TSEMICOLON
%token <token> TDOT
%token <token> TCOMMA
%token <string> TIDENTIFIER
%token <string> TFLOAT_CONSTANT
%token <string> TDOUBLE_CONSTANT
%token <string> TINTEGER_CONSTANT
%token <string> TSTRING_LITERAL

%start program

%%

program : import_list
        | declaration_list
        | namespace_list
        ;

import_list : import
            | import_list import

import : TIMPORT TSTRING_LITERAL

alias_declaration   : TALIAS identifier TEQUAL builtin_type TSEMICOLON
                    | TALIAS identifier TEQUAL builtin_type TCOLON attribute_list TSEMICOLON
                    ;

struct_declaration  : TSTRUCT identifier TLBRACE TRBRACE TSEMICOLON
                    | TSTRUCT identifier TLBRACE struct_body TRBRACE TSEMICOLON
                    | TSTRUCT identifier TCOLON inheritance TLBRACE TRBRACE TSEMICOLON
                    | TSTRUCT identifier TCOLON inheritance TLBRACE struct_body TRBRACE TSEMICOLON
                    ;

struct_body : declaration_list
            ;


inheritance : inheritance_type identifier
            ;

inheritance_type    : TPUBLIC
                    | TPRIVATE
                    | TPROTECTED
                    ;

namespace_list  : namespace
                | namespace_list namespace

namespace   : TNAMESPACE identifier TLBRACE TRBRACE
            | TNAMESPACE identifier TLBRACE namespace_body TRBRACE
            ;

namespace_body  : declaration_list
                ;

var_declaration : type identifier TSEMICOLON
                | type identifier TCOLON attribute_list TSEMICOLON
                | type identifier TEQUAL expression TSEMICOLON
                | type identifier TEQUAL expression TCOLON attribute_list TSEMICOLON
                ;

declaration_list    : struct_declaration
                    | alias_declaration
                    | var_declaration
                    ;

identifier  : TIDENTIFIER
            ;

        
attribute_list  : TLBRAKET attribute_body TRBRAKET
                ;

attribute_body  : attribute
                | attribute_body TCOMMA attribute
                ;

attribute   : TMIN TEQUAL expression
            | TMAX TEQUAL expression
            | TUI_NAME TEQUAL TSTRING_LITERAL
            ;
        
expression  : literal
            | initializer_list
            | TLPARENTHESIS expression TRPARENTHESIS
            ;

initializer_list    : TLBRACE TRBRACE
                    | TLBRACE initializer_body TRBRACE
                    ;

initializer_body    : expression
                    | initializer_body TCOMMA expression
                    ;

type    : identifier
        | builtin_type
        ;

builtin_type    : builtin_templated_type
                | TBOOL
                | TFLOAT | TDOUBLE
                | TSTRING
                | TINT8_T | TUINT8_T
                | TINT16_T | TUINT16_T
                | TINT32_T | TUINT32_T
                | TINT64_T | TUINT64_T
                | TVEC2F | TVEC2D | TVEC2U8 | TVEC2S8 | TVEC2U16 | TVEC2S16 | TVEC2U32 | TVEC2S32
                | TVEC3F | TVEC3D | TVEC3U8 | TVEC3S8 | TVEC3U16 | TVEC3S16 | TVEC3U32 | TVEC3S32
                | TVEC4F | TVEC4D | TVEC4U8 | TVEC4S8 | TVEC4U16 | TVEC4S16 | TVEC4U32 | TVEC4S32
                | TQUATF | TQUATD
                | TMAT2F | TMAT2D
                | TMAT3F | TMAT3D
                | TMAT4F | TMAT4D
                | TCOLOR_RGB | TCOLOR_RGBA
                ;
    
builtin_templated_type  : TVECTOR template_argument_list
                        ;
            
template_argument_list  : TLANGLED_BRAKET TRANGLED_BRAKET
                        | TLANGLED_BRAKET template_argument_body TRANGLED_BRAKET
                        ;

template_argument_body  : template_argument
                        | template_argument_body TCOMMA template_argument
                        ;

template_argument   : type
                    | TSTRING_LITERAL
                    | TINTEGER_CONSTANT
                    ;

literal : TFLOAT_CONSTANT
        | TDOUBLE_CONSTANT
        | TINTEGER_CONSTANT
        | TSTRING_LITERAL
        | TFALSE
        | TTRUE
        ;


%%
