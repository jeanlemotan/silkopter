%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define parser_class_name { parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%locations
%initial-action
{
  // Initialize the initial location.
  //@$.begin.filename = @$.end.filename = &p.get_filename();
};

%code requires
{
    #include <string>

    #include "ast/Builder.h"
    #include "ast/Node.h"
    #include "ast/Attribute.h"
}

%param { ast::Builder& builder }

%define parse.trace
%define parse.error verbose

%code
{
    #include "ast/Lexer.h"

    // Prototype of the yylex function providing subsequent tokens.
    yy::parser::symbol_type yylex(ast::Builder& builder)
    {
        return builder.get_lexer().astyylex();
    }
}

%token
    TEND 0 "end"
    TIMPORT "import"
    TALIAS "alias"
    TSTRUCT "struct"
    TPUBLIC "public"
    TPRIVATE "private"
    TPROTECTED "protected"
    TCONST "const"
    TNAMESPACE "namespace"
    TFALSE "false"
    TTRUE "true"
    TEQUAL "="
    TLPARENTHESIS "("
    TRPARENTHESIS ")"
    TLBRACE "{"
    TRBRACE "}"
    TLBRAKET "["
    TRBRAKET "]"
    TLANGLED_BRAKET "<"
    TRANGLED_BRAKET ">"
    TNAMESPACE_SEPARATOR "::"
    TCOLON ":"
    TSEMICOLON ";"
    TDOT "."
    TCOMMA ","
    ;

%token <std::string> TIDENTIFIER_PATH "identifier path"
%token <std::string> TIDENTIFIER "identifier"
%token <float> TFLOAT_LITERAL "float literal"
%token <double> TDOUBLE_LITERAL "double literal"
%token <int64_t> TINTEGER_LITERAL "integer literal"
%token <std::string> TSTRING_LITERAL "string literal"

%type <::ast::Node> top_level_declaration_list
%type <::ast::Node> top_level_declaration
%type <::ast::Node> type_declaration
%type <::ast::Node> struct_declaration
%type <::ast::Node> struct_body
%type <::ast::Node> struct_body_declaration
%type <::ast::Node> struct_body_declaration_list
%type <::ast::Node> inheritance
%type <::ast::Node> identifier_path
%type <::ast::Node> identifier
%type <::ast::Node> type
%type <::ast::Node> templated_type
%type <::ast::Node> template_argument_list
%type <::ast::Node> template_argument
%type <::ast::Node> alias_declaration
%type <::ast::Node> namespace_declaration
%type <::ast::Node> namespace_body
%type <::ast::Node> namespace_body_declaration
%type <::ast::Node> namespace_body_declaration_list
%type <::ast::Node> member_declaration
%type <::ast::Node> attribute
%type <::ast::Node> attribute_list
%type <::ast::Node> attribute_body
%type <::ast::Node> literal
%type <::ast::Node> initializer_list
%type <::ast::Node> initializer
%type <::ast::Node> initializer_body

//%printer { yyoutput << $$; } <>;

%%
%start program;

program : import_list
        {
        }
        | top_level_declaration_list
        {
            builder.get_root_node().move_children_from(std::move($1));
        }
        ;

import_list : import
            {
            }
            | import_list import
            {
            }


import  : TIMPORT TSTRING_LITERAL
        {
            builder.start_file($2.substr(1, $2.size() - 2));
        }
        ;


top_level_declaration_list  : top_level_declaration
                            {
                                $$ = ast::Node(ast::Node::Type::LIST, builder.get_location());
                                $$.add_child($1);
                            }
                            | top_level_declaration_list top_level_declaration
                            {
                                $$.move_children_from(std::move($1));
                                $$.add_child($2);
                            }
                            ;

top_level_declaration   : type_declaration
                        {
                            $$ = $1;
                        }
                        | namespace_declaration
                        {
                            $$ = $1;
                        }
                        ;


alias_declaration   : TALIAS identifier TEQUAL type TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | TALIAS identifier TEQUAL type TCOLON attribute_list TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.move_children_from(std::move($6));
                    }
                    ;

struct_declaration  : TSTRUCT identifier TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                    }
                    | TSTRUCT identifier TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | TSTRUCT identifier TCOLON inheritance TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | TSTRUCT identifier TCOLON inheritance TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.add_child($6);
                    }
                    ;

struct_body : struct_body_declaration_list
            {
                $$ = ast::Node(ast::Node::Type::STRUCT_BODY, builder.get_location());
                $$.move_children_from(std::move($1));
            }
            ;

struct_body_declaration_list    : struct_body_declaration
                                {
                                    $$ = ast::Node(ast::Node::Type::LIST, builder.get_location());
                                    $$.add_child($1);
                                }
                                | struct_body_declaration_list struct_body_declaration
                                {
                                    $$.move_children_from(std::move($1));
                                    $$.add_child($2);
                                }
                                ;


struct_body_declaration : type_declaration
                        {
                            $$ = $1;
                        }
                        | member_declaration
                        {
                            $$ = $1;
                        }
                        ;

inheritance : inheritance_type identifier_path
            ;

inheritance_type    : TPUBLIC
                    | TPRIVATE
                    | TPROTECTED
                    ;

namespace_declaration   : TNAMESPACE identifier TLBRACE TRBRACE
            {
                $$ = ast::Node(ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                $$.add_child($2);
            }
            | TNAMESPACE identifier TLBRACE namespace_body TRBRACE
            {
                $$ = ast::Node(ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                $$.add_child($2);
                $$.add_child($4);
            }
            ;

namespace_body  : namespace_body_declaration_list
                {
                    $$ = ast::Node(ast::Node::Type::NAMESPACE_BODY, builder.get_location());
                    $$.move_children_from(std::move($1));
                }
                ;

namespace_body_declaration_list : namespace_body_declaration
                                {
                                    $$ = ast::Node(ast::Node::Type::LIST, builder.get_location());
                                    $$.add_child($1);
                                }
                                | namespace_body_declaration_list namespace_body_declaration
                                {
                                    $$.move_children_from(std::move($1));
                                    $$.add_child($2);
                                }
                                ;

namespace_body_declaration  : type_declaration
                            {
                                $$ = $1;
                            }
                            | namespace_declaration
                            {
                                $$ = $1;
                            }
                            ;


member_declaration  : type identifier TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                    }
                    | type identifier TCOLON attribute_list TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                        $$.move_children_from(std::move($4));
                    }
                    | type identifier TEQUAL initializer TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | type identifier TEQUAL initializer TCOLON attribute_list TSEMICOLON
                    {
                        $$ = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.move_children_from(std::move($6));
                    }
                    ;



type_declaration    : struct_declaration
                    {
                        $$ = $1;
                    }
                    | alias_declaration
                    {
                        $$ = $1;
                    }
                    ;

identifier  : TIDENTIFIER
            {
                $$ = ast::Node(ast::Node::Type::IDENTIFIER, builder.get_location());
                $$.add_attribute(ast::Attribute("value", $1));
            }
            ;

identifier_path : TIDENTIFIER_PATH
                {
                    $$ = ast::Node(ast::Node::Type::IDENTIFIER, builder.get_location());
                    $$.add_attribute(ast::Attribute("value", $1));
                }
                ;

        
attribute_list  : TLBRAKET attribute_body TRBRAKET
                {
                    $$ = $2;
                }
                ;

attribute_body  : attribute
                {
                    $$ = ast::Node(ast::Node::Type::LIST, builder.get_location());
                    $$.add_child($1);
                }
                | attribute_body TCOMMA attribute
                {
                    $$.move_children_from(std::move($1));
                    $$.add_child($3);
                }
                ;

attribute   : identifier TEQUAL initializer
            {
                $$ = ast::Node(ast::Node::Type::ATTRIBUTE, builder.get_location());
                $$.add_child($1);
                $$.add_child($3);
            }
            ;
        
initializer : literal
            {
                $$ = ast::Node(ast::Node::Type::INITIALIZER, builder.get_location());
                $$.add_child($1);
            }
            | initializer_list
            {
                $$ = ast::Node(ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                $$.move_children_from(std::move($1));
            }
            ;

initializer_list    : TLBRACE TRBRACE
                    {
                        $$ = ast::Node(ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                    }
                    | TLBRACE initializer_body TRBRACE
                    {
                        $$ = ast::Node(ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                        $$.move_children_from(std::move($2));
                    }
                    ;

initializer_body    : initializer
                    {
                        $$.add_child(std::move($1));
                    }
                    | initializer_body TCOMMA initializer
                    {
                        $$.move_children_from(std::move($1));
                        $$.add_child($3);
                    }
                    ;

type    : identifier_path
        {
            $$ = ast::Node(ast::Node::Type::TYPE, builder.get_location());
            $$.add_child($1);
        }
        | identifier
        {
            $$ = ast::Node(ast::Node::Type::TYPE, builder.get_location());
            $$.add_child($1);
        }
        | templated_type
        {
            $$ = ast::Node(ast::Node::Type::TYPE, builder.get_location());
            $$.add_child($1);
        }
        ;

templated_type  : identifier TLANGLED_BRAKET TRANGLED_BRAKET
                {
                    $$ = ast::Node(ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    $$.add_child($1);
                }
                | identifier TLANGLED_BRAKET template_argument_list TRANGLED_BRAKET
                {
                    $$ = ast::Node(ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    $$.add_child($1);
                    $$.move_children_from(std::move($3));
                }
                ;

template_argument_list  : template_argument
                        {
                            $$ = ast::Node(ast::Node::Type::LIST, builder.get_location());
                            $$.add_child($1);
                        }
                        | template_argument_list TCOMMA template_argument
                        {
                            $$.move_children_from(std::move($1));
                            $$.add_child($3);
                        }
                        ;

template_argument   : type
                    {
                        $$ = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child($1);
                    }
                    | TTRUE
                    {
                        $$ = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child(ast::Node(ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ast::Attribute("value", true)));
                    }
                    | TFALSE
                    {
                        $$ = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child(ast::Node(ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ast::Attribute("value", false)));
                    }
                    ;

literal : TFLOAT_LITERAL
        {
            $$ = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ast::Attribute("value", $1));
        }
        | TDOUBLE_LITERAL
        {
            $$ = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ast::Attribute("value", $1));
        }
        | TINTEGER_LITERAL
        {
            $$ = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ast::Attribute("value", $1));
        }
        | TSTRING_LITERAL
        {
            $$ = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ast::Attribute("value", $1.substr(1, $1.size() - 2)));
        }
        | TFALSE
        {
            $$ = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ast::Attribute("value", false));
        }
        | TTRUE
        {
            $$ = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ast::Attribute("value", true));
        }
        ;


%%

namespace yy
{
// Mandatory error function
void parser::error (const parser::location_type& loc, const std::string& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

}
