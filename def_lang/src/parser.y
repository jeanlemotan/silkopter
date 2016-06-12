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
    #include <sstream>

    #include "def_lang/Result.h"
    #include "def_lang/ast/Builder.h"
    #include "def_lang/ast/Node.h"
    #include "def_lang/ast/Attribute.h"
}

%param { ts::ast::Builder& builder }

%define parse.trace
%define parse.error verbose

%code
{
    #include "ts/ast/Lexer.h"

    // Prototype of the yylex function providing subsequent tokens.
    yy::parser::symbol_type yylex(ts::ast::Builder& builder)
    {
        return builder.get_lexer().astyylex();
    }
}

%token
    TEND 0 "end"
    TIMPORT "import"
    TALIAS "alias"
    TSTRUCT "struct"
    TENUM "enum"
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

%type <ts::ast::Node> top_level_declaration_list
%type <ts::ast::Node> top_level_declaration
%type <ts::ast::Node> type_declaration
%type <ts::ast::Node> enum_declaration
%type <ts::ast::Node> enum_body
%type <ts::ast::Node> enum_body_item
%type <ts::ast::Node> enum_body_item_list
%type <ts::ast::Node> struct_declaration
%type <ts::ast::Node> struct_body
%type <ts::ast::Node> struct_body_declaration
%type <ts::ast::Node> struct_body_declaration_list
%type <ts::ast::Node> inheritance
%type <ts::ast::Node> identifier_path
%type <ts::ast::Node> identifier
%type <ts::ast::Node> type
%type <ts::ast::Node> templated_type
%type <ts::ast::Node> template_argument_list
%type <ts::ast::Node> template_argument
%type <ts::ast::Node> alias_declaration
%type <ts::ast::Node> namespace_declaration
%type <ts::ast::Node> namespace_body
%type <ts::ast::Node> namespace_body_declaration
%type <ts::ast::Node> namespace_body_declaration_list
%type <ts::ast::Node> member_declaration
%type <ts::ast::Node> attribute
%type <ts::ast::Node> attribute_list
%type <ts::ast::Node> attribute_body
%type <ts::ast::Node> literal
%type <ts::ast::Node> initializer_list
%type <ts::ast::Node> initializer
%type <ts::ast::Node> initializer_body

//%printer { yyoutput << $$; } <>;

%%
%start program;

program : import_list
        {
        }
        | top_level_declaration_list
        {
            builder.get_ast_root_node().move_children_from(std::move($1));
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
                                $$ = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
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
                        $$ = ts::ast::Node(ts::ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | TALIAS identifier TEQUAL type TCOLON attribute_list TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.move_children_from(std::move($6));
                    }
                    ;

enum_declaration    : TENUM identifier TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::ENUM_DECLARATION, builder.get_location());
                        $$.add_child($2);
                    }
                    | TENUM identifier TLBRACE enum_body TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::ENUM_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    ;

enum_body   : enum_body_item_list
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::ENUM_BODY, builder.get_location());
                $$.move_children_from(std::move($1));
            }
            ;

enum_body_item_list : enum_body_item
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                        $$.add_child($1);
                    }
                    | enum_body_item_list enum_body_item
                    {
                        $$.move_children_from(std::move($1));
                        $$.add_child($2);
                    }
                    ;


enum_body_item  : identifier TCOMMA
                {
                    $$ = $1;
                }
                | identifier TEQUAL TINTEGER_LITERAL TCOMMA
                {
                    $$ = $1;
                    $$.add_attribute(ts::ast::Attribute("integral_value", $3));
                }
                | identifier TCOLON attribute_list TCOMMA
                {
                    $$ = $1;
                }
                | identifier TEQUAL TINTEGER_LITERAL TCOLON attribute_list TCOMMA
                {
                    $$ = $1;
                    $$.add_attribute(ts::ast::Attribute("integral_value", $3));
                    $$.move_children_from(std::move($5));
                }
                ;


struct_declaration  : TSTRUCT identifier TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                    }
                    | TSTRUCT identifier TCOLON attribute_list TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.move_children_from(std::move($4));
                    }
                    | TSTRUCT identifier TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | TSTRUCT identifier TCOLON attribute_list TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.move_children_from(std::move($4));
                        $$.add_child($6);
                    }
                    | TSTRUCT identifier TCOLON inheritance TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | TSTRUCT identifier TCOLON inheritance TCOLON attribute_list TLBRACE TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.move_children_from(std::move($6));
                    }
                    | TSTRUCT identifier TCOLON inheritance TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.add_child($6);
                    }
                    | TSTRUCT identifier TCOLON inheritance TCOLON attribute_list TLBRACE struct_body TRBRACE TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        $$.add_child($2);
                        $$.add_child($4);
                        $$.move_children_from(std::move($6));
                        $$.add_child($8);
                    }
                    ;

struct_body : struct_body_declaration_list
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::STRUCT_BODY, builder.get_location());
                $$.move_children_from(std::move($1));
            }
            ;

struct_body_declaration_list    : struct_body_declaration
                                {
                                    $$ = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
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

inheritance : TPUBLIC type
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::INHERITANCE, builder.get_location());
                $$.add_child($2);
                $$.add_attribute(ts::ast::Attribute("visibility", "public"));
            }
            ;

namespace_declaration   : TNAMESPACE identifier TLBRACE TRBRACE
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                $$.add_child($2);
            }
            | TNAMESPACE identifier TLBRACE namespace_body TRBRACE
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                $$.add_child($2);
                $$.add_child($4);
            }
            ;

namespace_body  : namespace_body_declaration_list
                {
                    $$ = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_BODY, builder.get_location());
                    $$.move_children_from(std::move($1));
                }
                ;

namespace_body_declaration_list : namespace_body_declaration
                                {
                                    $$ = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
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
                        $$ = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                    }
                    | type identifier TCOLON attribute_list TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                        $$.move_children_from(std::move($4));
                    }
                    | type identifier TEQUAL initializer TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        $$.add_child($1);
                        $$.add_child($2);
                        $$.add_child($4);
                    }
                    | type identifier TEQUAL initializer TCOLON attribute_list TSEMICOLON
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
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
                    | enum_declaration
                    {
                        $$ = $1;
                    }
                    ;

identifier  : TIDENTIFIER
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::IDENTIFIER, builder.get_location());
                $$.add_attribute(ts::ast::Attribute("value", $1));
            }
            ;

identifier_path : TIDENTIFIER_PATH
                {
                    $$ = ts::ast::Node(ts::ast::Node::Type::IDENTIFIER, builder.get_location());
                    $$.add_attribute(ts::ast::Attribute("value", $1));
                }
                ;

        
attribute_list  : TLBRAKET attribute_body TRBRAKET
                {
                    $$ = $2;
                }
                ;

attribute_body  : attribute
                {
                    $$ = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
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
                $$ = ts::ast::Node(ts::ast::Node::Type::ATTRIBUTE, builder.get_location());
                $$.add_child($1);
                $$.add_child($3);
            }
            ;
        
initializer : literal
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                $$.add_child($1);
            }
            | identifier_path
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                $$.add_child($1);
            }
            | identifier
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                $$.add_child($1);
            }
            | initializer_list
            {
                $$ = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                $$.move_children_from(std::move($1));
            }
            ;

initializer_list    : TLBRACE TRBRACE
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                    }
                    | TLBRACE initializer_body TRBRACE
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
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
            $$ = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            $$.add_child($1);
        }
        | identifier
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            $$.add_child($1);
        }
        | templated_type
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            $$.add_child($1);
        }
        ;

templated_type  : identifier TLANGLED_BRAKET TRANGLED_BRAKET
                {
                    $$ = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    $$.add_child($1);
                }
                | identifier TLANGLED_BRAKET template_argument_list TRANGLED_BRAKET
                {
                    $$ = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    $$.add_child($1);
                    $$.move_children_from(std::move($3));
                }
                ;

template_argument_list  : template_argument
                        {
                            $$ = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
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
                        $$ = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child($1);
                    }
                    | TCONST type
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child($2).add_attribute(ts::ast::Attribute("const", true));
                    }
                    | TTRUE
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child(ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ts::ast::Attribute("value", true)));
                    }
                    | TFALSE
                    {
                        $$ = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        $$.add_child(ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ts::ast::Attribute("value", false)));
                    }
                    ;

literal : TFLOAT_LITERAL
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ts::ast::Attribute("value", $1));
        }
        | TDOUBLE_LITERAL
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ts::ast::Attribute("value", $1));
        }
        | TINTEGER_LITERAL
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ts::ast::Attribute("value", $1));
        }
        | TSTRING_LITERAL
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ts::ast::Attribute("value", $1.substr(1, $1.size() - 2)));
        }
        | TFALSE
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ts::ast::Attribute("value", false));
        }
        | TTRUE
        {
            $$ = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            $$.add_attribute(ts::ast::Attribute("value", true));
        }
        ;


%%

namespace yy
{
// Mandatory error function
void parser::error (const parser::location_type& loc, const std::string& msg)
{
    std::stringstream ss;
    ss << loc << ": " << msg;
    builder.report_error(ts::Error(ss.str()));
}

}
