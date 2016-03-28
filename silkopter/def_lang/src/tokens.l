%{
#include <stdio.h>
#include <iostream>
#include <string>
//#include "node.h"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%%

[ tn]                 ;
"import"                return TOKEN(TIMPORT);
"alias"                 return TOKEN(TALIAS);
"struct"                return TOKEN(TSTRUCT);
"string"                return TOKEN(TSTRING);
"vector"                return TOKEN(TVECTOR);
"float"                 return TOKEN(TFLOAT);
"double"                return TOKEN(TDOUBLE);
"bool"                  return TOKEN(TBOOL);
"uint8_t"               return TOKEN(TUINT8_T);
"int8_t"                return TOKEN(TINT8_T);
"uint16_t"              return TOKEN(TUINT16_T);
"int16_t"               return TOKEN(TINT16_T);
"uint32_t"              return TOKEN(TUINT32_T);
"int32_t"               return TOKEN(TINT32_T);
"uint64_t"              return TOKEN(TUINT64_T);
"int64_t"               return TOKEN(TINT64_T);
"vec2f"                 return TOKEN(TVEC2F);
"vec2d"                 return TOKEN(TVEC2D);
"vec2s8"                return TOKEN(TVEC2S8);
"vec2u8"                return TOKEN(TVEC2U8);
"vec2s16"               return TOKEN(TVEC2S16);
"vec2u16"               return TOKEN(TVEC2U16);
"vec2s32"               return TOKEN(TVEC2S32);
"vec2u32"               return TOKEN(TVEC2U32);
"vec3f"                 return TOKEN(TVEC3F);
"vec3d"                 return TOKEN(TVEC3D);
"vec3s8"                return TOKEN(TVEC3S8);
"vec3u8"                return TOKEN(TVEC3U8);
"vec3s16"               return TOKEN(TVEC3S16);
"vec3u16"               return TOKEN(TVEC3U16);
"vec3s32"               return TOKEN(TVEC3S32);
"vec3u32"               return TOKEN(TVEC3U32);
"vec4f"                 return TOKEN(TVEC4F);
"vec4d"                 return TOKEN(TVEC4D);
"vec4s8"                return TOKEN(TVEC4S8);
"vec4u8"                return TOKEN(TVEC4U8);
"vec4s16"               return TOKEN(TVEC4S16);
"vec4u16"               return TOKEN(TVEC4U16);
"vec4s32"               return TOKEN(TVEC4S32);
"vec4u32"               return TOKEN(TVEC4U32);
"quatf"                 return TOKEN(TQUATF);
"quatd"                 return TOKEN(TQUATD);
"mat2f"                 return TOKEN(TMAT2F);
"mat2d"                 return TOKEN(TMAT2D);
"mat3f"                 return TOKEN(TMAT3F);
"mat3d"                 return TOKEN(TMAT3D);
"mat4f"                 return TOKEN(TMAT4F);
"mat4d"                 return TOKEN(TMAT4D);
"color_rgb"             return TOKEN(TCOLOR_RGB);
"color_rgba"            return TOKEN(TCOLOR_RGBA);
"min"                   return TOKEN(TMIN);
"max"                   return TOKEN(TMAX);
"public"                return TOKEN(TPUBLIC);
"private"               return TOKEN(TPRIVATE);
"protected"             return TOKEN(TPROTECTED);
"const"                 return TOKEN(TCONST);
"ui_control"            return TOKEN(TUI_CONTROL);
"ui_name"               return TOKEN(TUI_NAME);
"ui_const"              return TOKEN(TUI_CONST);
"namespace"             return TOKEN(TNAMESPACE);
"false" 	            return TOKEN(TFALSE);
"true" 	                return TOKEN(TTRUE);
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
[0-9]+                  SAVE_TOKEN; return TINTEGER_CONSTANT;
[0-9]+.[0-9]*           SAVE_TOKEN; return TDOUBLE_CONSTANT;
[0-9]+.[0-9]*f          SAVE_TOKEN; return TFLOAT_CONSTANT;
\"([^\\\"]|\\.)*\" 	    SAVE_TOKEN; return TSTRING_LITERAL;
"="                     return TOKEN(TEQUAL);
"("                     return TOKEN(TLPARENTHESIS);
")"                     return TOKEN(TRPARENTHESIS);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"["                     return TOKEN(TLBRAKET);
"]"                     return TOKEN(TRBRAKET);
"<"                     return TOKEN(TLANGLED_BRAKET);
">"                     return TOKEN(TRANGLED_BRAKET);
"::"                    return TOKEN(TNAMESPACE_SEPARATOR);
":"                     return TOKEN(TCOLON);
";"                     return TOKEN(TSEMICOLON);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
.                       std::cout << "Unknown token!\n"; yyterminate();

%%

