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

%type <type> builtin_type
%type <type> alias_declaration struct_declaration
%type <namespace> namespace
%type <member_def> member_declaration
%type <attribute> attribute
%type <attribute_list> attribute_list
%type <literal> literal
%type <expression> expression
%type <initializer_list> initializer_list

%destructor { delete $$; } TIDENTIFIER TFLOAT_LITERAL TDOUBLE_LITERAL TINTEGER_LITERAL TSTRING_LITERAL
%destructor { delete $$; } builtin_type
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

alias_declaration   : TALIAS identifier TEQUAL builtin_type TSEMICOLON
                    {
                        $$ = new ts::Alias();
                    }
                    | TALIAS identifier TEQUAL builtin_type TCOLON attribute_list TSEMICOLON
                    {
                        $$ = new ts::Alias();
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

member_declaration : type identifier TSEMICOLON
                {
                    $$ = new ts::Member_Def();
                }
                | type identifier TCOLON attribute_list TSEMICOLON
                {
                    $$ = new ts::Member_Def();
                }
                | type identifier TEQUAL expression TSEMICOLON
                {
                    $$ = new ts::Member_Def();
                }
                | type identifier TEQUAL expression TCOLON attribute_list TSEMICOLON
                {
                    $$ = new ts::Member_Def();
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

type    : identifier
        | builtin_type
        ;

builtin_type    : builtin_templated_type { $$ = nullptr; }
                | TBOOL         { $$ = type_system->get_bool_type().get(); }
                | TFLOAT        { $$ = type_system->get_float_type().get(); }
                | TDOUBLE       { $$ = type_system->get_double_type().get(); }
                | TSTRING       { $$ = type_system->get_string_type().get(); }
                | TINT8_T       { $$ = type_system->get_int8_type().get(); }
                | TUINT8_T      { $$ = type_system->get_uint8_type().get(); }
                | TINT16_T      { $$ = type_system->get_int16_type().get(); }
                | TUINT16_T     { $$ = type_system->get_uint16_type().get(); }
                | TINT32_T      { $$ = type_system->get_int32_type().get(); }
                | TUINT32_T     { $$ = type_system->get_uint32_type().get(); }
                | TINT64_T      { $$ = type_system->get_int64_type().get(); }
                | TUINT64_T     { $$ = type_system->get_uint64_type().get(); }
                | TVEC2F        { $$ = type_system->get_vec2f_type().get(); }
                | TVEC2D        { $$ = type_system->get_vec2d_type().get(); } 
                | TVEC2U8       { $$ = type_system->get_vec2u8_type().get(); }
                | TVEC2S8       { $$ = type_system->get_vec2s8_type().get(); } 
                | TVEC2U16      { $$ = type_system->get_vec2u16_type().get(); }
                | TVEC2S16      { $$ = type_system->get_vec2s16_type().get(); }
                | TVEC2U32      { $$ = type_system->get_vec2u32_type().get(); }
                | TVEC2S32      { $$ = type_system->get_vec2s32_type().get(); }
                | TVEC3F        { $$ = type_system->get_vec3f_type().get(); }
                | TVEC3D        { $$ = type_system->get_vec3d_type().get(); } 
                | TVEC3U8       { $$ = type_system->get_vec3u8_type().get(); }
                | TVEC3S8       { $$ = type_system->get_vec3s8_type().get(); } 
                | TVEC3U16      { $$ = type_system->get_vec3u16_type().get(); }
                | TVEC3S16      { $$ = type_system->get_vec3s16_type().get(); }
                | TVEC3U32      { $$ = type_system->get_vec3u32_type().get(); }
                | TVEC3S32      { $$ = type_system->get_vec3s32_type().get(); }
                | TVEC4F        { $$ = type_system->get_vec4f_type().get(); }
                | TVEC4D        { $$ = type_system->get_vec4d_type().get(); } 
                | TVEC4U8       { $$ = type_system->get_vec4u8_type().get(); }
                | TVEC4S8       { $$ = type_system->get_vec4s8_type().get(); } 
                | TVEC4U16      { $$ = type_system->get_vec4u16_type().get(); }
                | TVEC4S16      { $$ = type_system->get_vec4s16_type().get(); }
                | TVEC4U32      { $$ = type_system->get_vec4u32_type().get(); }
                | TVEC4S32      { $$ = type_system->get_vec4s32_type().get(); }
                | TQUATF        { $$ = type_system->get_quatf_type().get(); }
                | TQUATD        { $$ = type_system->get_quafd_type().get(); }
                | TMAT2F        { $$ = type_system->get_mat2f_type().get(); }
                | TMAT2D        { $$ = type_system->get_mat2d_type().get(); }
                | TMAT3F        { $$ = type_system->get_mat3f_type().get(); }
                | TMAT3D        { $$ = type_system->get_mat3d_type().get(); }
                | TMAT4F        { $$ = type_system->get_mat4f_type().get(); }
                | TMAT4D        { $$ = type_system->get_mat4d_type().get(); }
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

