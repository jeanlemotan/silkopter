/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIMPORT = 258,
    TALIAS = 259,
    TSTRUCT = 260,
    TSTRING = 261,
    TVECTOR = 262,
    TFLOAT = 263,
    TDOUBLE = 264,
    TBOOL = 265,
    TUINT8_T = 266,
    TINT8_T = 267,
    TUINT16_T = 268,
    TINT16_T = 269,
    TUINT32_T = 270,
    TINT32_T = 271,
    TUINT64_T = 272,
    TINT64_T = 273,
    TVEC2F = 274,
    TVEC2D = 275,
    TVEC2S8 = 276,
    TVEC2U8 = 277,
    TVEC2S16 = 278,
    TVEC2U16 = 279,
    TVEC2S32 = 280,
    TVEC2U32 = 281,
    TVEC3F = 282,
    TVEC3D = 283,
    TVEC3S8 = 284,
    TVEC3U8 = 285,
    TVEC3S16 = 286,
    TVEC3U16 = 287,
    TVEC3S32 = 288,
    TVEC3U32 = 289,
    TVEC4F = 290,
    TVEC4D = 291,
    TVEC4S8 = 292,
    TVEC4U8 = 293,
    TVEC4S16 = 294,
    TVEC4U16 = 295,
    TVEC4S32 = 296,
    TVEC4U32 = 297,
    TQUATF = 298,
    TQUATD = 299,
    TMAT2F = 300,
    TMAT2D = 301,
    TMAT3F = 302,
    TMAT3D = 303,
    TMAT4F = 304,
    TMAT4D = 305,
    TCOLOR_RGB = 306,
    TCOLOR_RGBA = 307,
    TMIN = 308,
    TMAX = 309,
    TPUBLIC = 310,
    TPRIVATE = 311,
    TPROTECTED = 312,
    TCONST = 313,
    TUI_CONTROL = 314,
    TUI_NAME = 315,
    TUI_CONST = 316,
    TNAMESPACE = 317,
    TFALSE = 318,
    TTRUE = 319,
    TEQUAL = 320,
    TLPARENTHESIS = 321,
    TRPARENTHESIS = 322,
    TLBRACE = 323,
    TRBRACE = 324,
    TLBRAKET = 325,
    TRBRAKET = 326,
    TLANGLED_BRAKET = 327,
    TRANGLED_BRAKET = 328,
    TNAMESPACE_SEPARATOR = 329,
    TCOLON = 330,
    TSEMICOLON = 331,
    TDOT = 332,
    TCOMMA = 333,
    TIDENTIFIER = 334,
    TFLOAT_CONSTANT = 335,
    TDOUBLE_CONSTANT = 336,
    TINTEGER_CONSTANT = 337,
    TSTRING_LITERAL = 338
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 19 "parser.y" /* yacc.c:1909  */

    std::string* string;
    int token;

#line 143 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
