%skeleton "lalr1.cc"
%require "2.3"
%debug
%defines
%error-verbose

%code 
{
    #include <stdio.h>
    #include <string>
    #include <iostream>
%}


%union 
{
    std::string* string;
    int token;
    ts::IType* type;
    ts::INamespace* namespace;
    ts::IMember_Def* member_def;
    ts::IAttribute* attribute;
    std::vector<std::unique_ptr<ts::IAttribute>>* attribute_list;
    ts::ILiteral* literal;
    ts::IExpression* expression;
    ts::IInitializer_List* initializer_list;
}

%token <token> TIMPORT
%token <token> TALIAS
%token <token> TSTRUCT
%token <token> TMIN
%token <token> TMAX
%token <token> TDECIMALS
%token <token> TPUBLIC
%token <token> TPRIVATE
%token <token> TPROTECTED
%token <token> TCONST
%token <token> TUI_CONTROL
%token <token> TUI_NAME
%token <token> TUI_CONST
%token <token> TNAMESPACE
%token <token> TFALSE
%token <token> TTRUE
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
%token <string> TFLOAT_LITERAL
%token <string> TDOUBLE_LITERAL
%token <string> TINTEGER_LITERAL
%token <string> TSTRING_LITERAL

%type <string> identifier
%type <string> type_name
%type <type> alias_declaration struct_declaration
%type <namespace> namespace
%type <member_def> member_declaration
%type <attribute> attribute
%type <attribute_list> attribute_list
%type <literal> literal
%type <expression> expression
%type <initializer_list> initializer_list

%destructor { delete $$; } TIDENTIFIER TFLOAT_LITERAL TDOUBLE_LITERAL TINTEGER_LITERAL TSTRING_LITERAL
%destructor { delete $$; } type_name
%destructor { delete $$; } alias_declaration struct_declaration
%destructor { delete $$; } namespace
%destructor { delete $$; } member_declaration
%destructor { delete $$; } attribute
%destructor { delete $$; } attribute_list
%destructor { delete $$; } literal
%destructor { delete $$; } expression
%destructor { delete $$; } initializer_list

%start program

%%

program : import_list
        | declaration_list
        | namespace_list
        ;

import_list : import
            | import_list import

import  : TIMPORT TSTRING_LITERAL
        {
            type_system->import($2)
        }
        ;

alias_declaration   : TALIAS identifier TEQUAL type_name TSEMICOLON
                    {
                        $$ = new ts::Alias($4);
                    }
                    | TALIAS identifier TEQUAL type_name TCOLON attribute_list TSEMICOLON
                    {
                        ts::Alias* alias = new ts::Alias($4);
                        for (auto const& att: $6)
                        {
                            $$->add_attribute(std::move(att));
                        }
                        $$ = alias;
                    }
                    ;

struct_declaration  : TSTRUCT identifier TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = new ts::Struct_Type();
                    }
                    | TSTRUCT identifier TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = new ts::Struct_Type();
                    }
                    | TSTRUCT identifier TCOLON inheritance TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = new ts::Struct_Type();
                    }
                    | TSTRUCT identifier TCOLON inheritance TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = new ts::Struct_Type();
                    }
                    ;

struct_body : struct_declaration
            | alias_declaration
            | member_declaration
            ;


inheritance : inheritance_type identifier
            ;

inheritance_type    : TPUBLIC
                    | TPRIVATE
                    | TPROTECTED
                    ;

namespace_list  : namespace
                | namespace_list namespace
                ;

namespace   : TNAMESPACE identifier TLBRACE TRBRACE
            {
                $$ = new ts::Namespace();
            }
            | TNAMESPACE identifier TLBRACE namespace_body TRBRACE
            {
                $$ = new ts::Namespace();
            }
            ;

namespace_body  : declaration_list
                ;

member_declaration : type_name identifier TSEMICOLON
                {
                    $$ = new ts::Member_Def($1, $2);
                }
                | type_name identifier TEQUAL expression TSEMICOLON
                {
                    $$ = new ts::Member_Def($1, $2, $4);
                }
                ;

declaration_list    : struct_declaration
                    | alias_declaration
                    ;

identifier  : TIDENTIFIER
            ;

        
attribute_list  : TLBRAKET attribute_body TRBRAKET
                {
                    $$ = new std::vector<std::unique_ptr<ts::IAttribute>>();
                }
                ;

attribute_body  : attribute
                | attribute_body TCOMMA attribute
                ;

attribute   : TMIN TEQUAL expression
            {
                $$ = new ts::Min_Attribute($3->Evaluate());
            }
            | TMAX TEQUAL expression
            {
                $$ = new ts::Max_Attribute($3->Evaluate());
            }
            | TDECIMALS TEQUAL TFLOAT_LITERAL
            {
                $$ = new ts::Decimals_Attribute($3);
            }
            | TUI_NAME TEQUAL TSTRING_LITERAL
            {
                $$ = new ts::UI_Name_Attribute($3);
            }
            ;
        
expression  : literal
            {
                $$ = new ts::Literal_Expression($1);
            }
            | initializer_list
            {
                $$ = new ts::Initializer_List_Expression($1);
            }
            | TLPARENTHESIS expression TRPARENTHESIS
            {
                $$ = $2;
            }
            ;

initializer_list    : TLBRACE TRBRACE
                    {
                        $$ = new ts::Initializer_List();
                    }
                    | TLBRACE initializer_body TRBRACE
                    {
                        $$ = new ts::Initializer_List();
                    }
                    ;

initializer_body    : expression
                    | initializer_body TCOMMA expression
                    ;

type_name   : templated_type
            | identifier
            {
                $$ = type_system.find_type_symbol_by_name($1).get();
            }
            ;

templated_type  : identifier TLANGLED_BRAKET TRANGLED_BRAKET
                | identifier TLANGLED_BRAKET template_argument_list TRANGLED_BRAKET
                ;

template_argument_list  : template_argument
                        | template_argument_list TCOMMA template_argument
                        ;

template_argument   : type_name
                    | TSTRING_LITERAL
                    | TINTEGER_LITERAL
                    ;

literal : TFLOAT_LITERAL
        {
            $$ = new ts::Literal($1);
        }
        | TDOUBLE_LITERAL
        {
            $$ = new ts::Literal($1);
        }
        | TINTEGER_LITERAL
        {
            $$ = new ts::Literal($1);
        }
        | TSTRING_LITERAL
        {
            $$ = new ts::Literal($1);
        }
        | TFALSE
        {
            $$ = new ts::Literal($1);
        }
        | TTRUE
        {
            $$ = new ts::Literal($1);
        }
        ;


%%

