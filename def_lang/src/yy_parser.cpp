// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "yy_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "yy_parser.hpp"

// User implementation prologue.

#line 51 "yy_parser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 34 "parser.y" // lalr1.cc:413

    #include "ts/ast/Lexer.h"

    // Prototype of the yylex function providing subsequent tokens.
    yy::parser::symbol_type yylex(ts::ast::Builder& builder)
    {
        return builder.get_lexer().astyylex();
    }

#line 63 "yy_parser.cpp" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 149 "yy_parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
   parser :: parser  (ts::ast::Builder& builder_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      builder (builder_yyarg)
  {}

   parser ::~ parser  ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
   parser ::by_state::by_state ()
    : state (empty_state)
  {}

  inline
   parser ::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
   parser ::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
   parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
   parser ::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
   parser ::symbol_number_type
   parser ::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
   parser ::stack_symbol_type::stack_symbol_type ()
  {}


  inline
   parser ::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 31: // "double literal"
        value.move< double > (that.value);
        break;

      case 30: // "float literal"
        value.move< float > (that.value);
        break;

      case 32: // "integer literal"
      case 33: // "hex literal"
        value.move< int64_t > (that.value);
        break;

      case 28: // "identifier path"
      case 29: // "identifier"
      case 34: // "string literal"
        value.move< std::string > (that.value);
        break;

      case 39: // top_level_declaration_list
      case 40: // top_level_declaration
      case 41: // alias_declaration
      case 42: // enum_declaration
      case 43: // enum_body
      case 44: // enum_body_item_list
      case 45: // enum_body_item
      case 46: // struct_declaration
      case 47: // struct_body
      case 48: // struct_body_declaration_list
      case 49: // struct_body_declaration
      case 50: // inheritance
      case 51: // namespace_declaration
      case 52: // namespace_body
      case 53: // namespace_body_declaration_list
      case 54: // namespace_body_declaration
      case 55: // member_declaration
      case 56: // type_declaration
      case 57: // identifier
      case 58: // identifier_path
      case 59: // attribute_list
      case 60: // attribute_body
      case 61: // attribute
      case 62: // initializer
      case 63: // initializer_list
      case 64: // initializer_body
      case 65: // type
      case 66: // templated_type
      case 67: // template_argument_list
      case 68: // template_argument
      case 69: // literal
        value.move< ts::ast::Node > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
   parser ::stack_symbol_type&
   parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 31: // "double literal"
        value.copy< double > (that.value);
        break;

      case 30: // "float literal"
        value.copy< float > (that.value);
        break;

      case 32: // "integer literal"
      case 33: // "hex literal"
        value.copy< int64_t > (that.value);
        break;

      case 28: // "identifier path"
      case 29: // "identifier"
      case 34: // "string literal"
        value.copy< std::string > (that.value);
        break;

      case 39: // top_level_declaration_list
      case 40: // top_level_declaration
      case 41: // alias_declaration
      case 42: // enum_declaration
      case 43: // enum_body
      case 44: // enum_body_item_list
      case 45: // enum_body_item
      case 46: // struct_declaration
      case 47: // struct_body
      case 48: // struct_body_declaration_list
      case 49: // struct_body_declaration
      case 50: // inheritance
      case 51: // namespace_declaration
      case 52: // namespace_body
      case 53: // namespace_body_declaration_list
      case 54: // namespace_body_declaration
      case 55: // member_declaration
      case 56: // type_declaration
      case 57: // identifier
      case 58: // identifier_path
      case 59: // attribute_list
      case 60: // attribute_body
      case 61: // attribute
      case 62: // initializer
      case 63: // initializer_list
      case 64: // initializer_body
      case 65: // type
      case 66: // templated_type
      case 67: // template_argument_list
      case 68: // template_argument
      case 69: // literal
        value.copy< ts::ast::Node > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
   parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   parser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
   parser ::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
   parser ::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
   parser ::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   parser ::debug_level_type
   parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline  parser ::state_type
   parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
   parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
   parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   parser ::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 12 "parser.y" // lalr1.cc:741
{
  // Initialize the initial location.
  //@$.begin.filename = @$.end.filename = &p.get_filename();
}

#line 529 "yy_parser.cpp" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (builder));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 31: // "double literal"
        yylhs.value.build< double > ();
        break;

      case 30: // "float literal"
        yylhs.value.build< float > ();
        break;

      case 32: // "integer literal"
      case 33: // "hex literal"
        yylhs.value.build< int64_t > ();
        break;

      case 28: // "identifier path"
      case 29: // "identifier"
      case 34: // "string literal"
        yylhs.value.build< std::string > ();
        break;

      case 39: // top_level_declaration_list
      case 40: // top_level_declaration
      case 41: // alias_declaration
      case 42: // enum_declaration
      case 43: // enum_body
      case 44: // enum_body_item_list
      case 45: // enum_body_item
      case 46: // struct_declaration
      case 47: // struct_body
      case 48: // struct_body_declaration_list
      case 49: // struct_body_declaration
      case 50: // inheritance
      case 51: // namespace_declaration
      case 52: // namespace_body
      case 53: // namespace_body_declaration_list
      case 54: // namespace_body_declaration
      case 55: // member_declaration
      case 56: // type_declaration
      case 57: // identifier
      case 58: // identifier_path
      case 59: // attribute_list
      case 60: // attribute_body
      case 61: // attribute
      case 62: // initializer
      case 63: // initializer_list
      case 64: // initializer_body
      case 65: // type
      case 66: // templated_type
      case 67: // template_argument_list
      case 68: // template_argument
      case 69: // literal
        yylhs.value.build< ts::ast::Node > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 119 "parser.y" // lalr1.cc:859
    {
        }
#line 693 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 122 "parser.y" // lalr1.cc:859
    {
            builder.get_ast_root_node().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
        }
#line 701 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 128 "parser.y" // lalr1.cc:859
    {
            }
#line 708 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 131 "parser.y" // lalr1.cc:859
    {
            }
#line 715 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 136 "parser.y" // lalr1.cc:859
    {
            builder.start_file(yystack_[0].value.as< std::string > ().substr(1, yystack_[0].value.as< std::string > ().size() - 2));
        }
#line 723 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 143 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                            }
#line 732 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 148 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                            }
#line 742 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 156 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 750 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 160 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 758 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 167 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
                    }
#line 768 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 173 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 779 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 182 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ENUM_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    }
#line 788 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 187 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ENUM_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 798 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 195 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ENUM_BODY, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
            }
#line 807 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 202 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 816 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 207 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 826 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 216 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[1].value.as< ts::ast::Node > ();
                }
#line 834 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 220 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[3].value.as< ts::ast::Node > ();
                    yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("integral_value", yystack_[1].value.as< int64_t > ()));
                }
#line 843 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 225 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[3].value.as< ts::ast::Node > ();
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                }
#line 852 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 230 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[5].value.as< ts::ast::Node > ();
                    yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("integral_value", yystack_[3].value.as< int64_t > ()));
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                }
#line 862 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 239 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    }
#line 871 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 244 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[3].value.as< ts::ast::Node > ()));
                    }
#line 881 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 250 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 891 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 256 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[4].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 902 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 263 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    }
#line 912 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 269 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[7].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[3].value.as< ts::ast::Node > ()));
                    }
#line 923 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 276 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 934 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 283 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[8].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[4].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 946 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 293 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_BODY, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
            }
#line 955 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 300 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 964 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 305 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 974 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 314 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 982 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 318 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 990 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 324 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INHERITANCE, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("visibility", "public"));
            }
#line 1000 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 332 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
            }
#line 1009 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 337 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
            }
#line 1019 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 345 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_BODY, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
                }
#line 1028 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 352 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 1037 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 357 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 1047 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 365 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                            }
#line 1055 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 369 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                            }
#line 1063 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 376 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
                    }
#line 1073 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 382 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 1084 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 389 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
                    }
#line 1095 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 396 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 1107 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 408 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                    }
#line 1115 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 412 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                    }
#line 1123 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 416 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                    }
#line 1131 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 422 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::IDENTIFIER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< std::string > ()));
            }
#line 1140 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 429 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::IDENTIFIER, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< std::string > ()));
                }
#line 1149 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 437 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[1].value.as< ts::ast::Node > ();
                }
#line 1157 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 443 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                }
#line 1166 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 448 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ts::ast::Node > ()));
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                }
#line 1176 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 456 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ATTRIBUTE, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1185 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 461 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ATTRIBUTE, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1195 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 469 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1204 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 474 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1213 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 479 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1222 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 484 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
            }
#line 1231 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 491 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                    }
#line 1239 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 495 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 1248 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 502 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > ().add_child(std::move(yystack_[0].value.as< ts::ast::Node > ()));
                    }
#line 1256 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 506 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 1265 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 513 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
        }
#line 1274 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 518 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
        }
#line 1283 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 523 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
        }
#line 1292 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 530 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                }
#line 1301 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 535 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                }
#line 1311 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 543 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                        }
#line 1320 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 548 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                            yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ts::ast::Node > ()));
                            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                        }
#line 1330 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 556 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 1339 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 561 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ()).add_attribute(ts::ast::Attribute("const", true));
                    }
#line 1348 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 566 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ts::ast::Attribute("value", true)));
                    }
#line 1357 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 571 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ts::ast::Attribute("value", false)));
                    }
#line 1366 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 578 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< float > ()));
        }
#line 1375 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 583 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< double > ()));
        }
#line 1384 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 588 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< int64_t > ()));
        }
#line 1393 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 593 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< int64_t > ()));
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("hex", true));
        }
#line 1403 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 599 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< std::string > ().substr(1, yystack_[0].value.as< std::string > ().size() - 2)));
        }
#line 1412 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 604 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", false));
        }
#line 1421 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 609 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", true));
        }
#line 1430 "yy_parser.cpp" // lalr1.cc:859
    break;


#line 1434 "yy_parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
   parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
   parser ::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  parser ::yypact_ninf_ = -82;

  const signed char  parser ::yytable_ninf_ = -1;

  const short int
   parser ::yypact_[] =
  {
     146,   -18,    11,    11,    11,    11,    68,    61,   -82,   129,
     -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,    52,    35,
      70,    79,   -82,   -82,   -82,   -14,    25,     5,    15,    14,
     -82,    78,   -82,   130,   -82,    60,    86,    45,   -82,   -82,
     -82,    11,   -14,    11,    64,   100,   101,   124,    11,   -82,
      21,   -82,   -82,   128,   129,   -82,   -82,   115,   111,   -82,
     -82,   131,   -82,    91,   -82,   139,   118,   -82,    51,   111,
      66,   -82,   135,   -82,   132,   111,   -82,   -82,   -82,   -14,
     -82,   -82,   -82,   -82,    38,   -82,   136,   -82,    90,   111,
     -82,    90,   -82,    11,   137,   145,   148,   141,   149,   -82,
     112,   142,   -82,   -82,   119,   -82,   -82,   -82,    80,   -82,
     -82,   -82,   -82,   -82,   -82,   -82,   134,   -82,   -82,   143,
     -82,   -82,   -82,   147,    72,   -82,   150,   111,   -82,   -82,
     -82,   -82,   -82,    20,   111,   -82,   -82,   -82,   151,   152,
     -82,   144,   -82,    90,   153,   -82,   154,   -82,   -82,   -82,
     -82
  };

  const unsigned char
   parser ::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,     2,     4,     3,
       7,    48,    49,    47,    10,     9,     6,    50,     0,     0,
       0,     0,     1,     5,     8,     0,     0,     0,     0,     0,
      51,    66,    65,     0,    67,     0,     0,    30,    31,    34,
      33,     0,     0,     0,     0,     0,     0,     0,    15,    16,
       0,    36,    42,     0,    38,    39,    41,     0,     0,    11,
      22,     0,    32,     0,    35,    55,     0,    53,     0,     0,
       0,    13,     0,    17,     0,     0,    18,    37,    40,     0,
      75,    74,    68,    72,     0,    70,     0,    24,     0,     0,
      43,     0,    52,     0,     0,     0,     0,     0,     0,    14,
       0,     0,    73,    69,     0,    12,    81,    82,     0,    76,
      77,    78,    79,    80,    59,    58,     0,    60,    57,     0,
      56,    54,    26,     0,     0,    23,     0,     0,    19,    20,
      71,    61,    63,     0,     0,    45,    44,    28,     0,     0,
      25,     0,    62,     0,     0,    27,     0,    21,    64,    46,
      29
  };

  const short int
   parser ::yypgoto_[] =
  {
     -82,   -82,   -82,   166,   -82,   165,   -82,   -82,   -82,   -82,
     133,   -82,   -63,   -82,   140,   -82,   -20,   -82,   -82,   126,
     -82,    13,    -2,   -80,   -52,   -82,    89,   -81,   -82,   -82,
     -21,   -82,   -82,    81,   -82
  };

  const short int
   parser ::yydefgoto_[] =
  {
      -1,     6,     7,     8,     9,    10,    11,    12,    47,    48,
      49,    13,    36,    37,    38,    44,    14,    53,    54,    55,
      39,    40,    31,    32,    45,    66,    67,   116,   117,   133,
      41,    34,    84,    85,   118
  };

  const unsigned char
   parser ::yytable_[] =
  {
      18,    19,    20,    21,    33,    95,    86,    98,   115,    52,
     120,   115,    42,    15,    30,    17,    16,    96,     2,     3,
       4,    64,    15,   101,    43,     5,    50,   132,   115,     2,
       3,     4,    51,    46,    52,    74,    83,   119,   142,    63,
      17,    65,    56,    35,    17,    75,    50,   143,    76,     2,
       3,     4,    26,    30,    17,     2,     3,     4,   102,    27,
     103,   139,   148,   115,     1,   104,    25,    56,    22,    94,
       2,     3,     4,    30,    17,   141,     2,     3,     4,    30,
      17,    68,   144,    83,    97,    60,   114,    28,    69,   114,
     138,    65,   106,   107,    30,    17,    29,   108,   131,    57,
      30,    17,   106,   107,    61,    88,   114,   108,    30,    17,
     109,   110,   111,   112,   113,    89,    90,    70,    30,    17,
     109,   110,   111,   112,   113,    79,    71,    80,    81,    79,
      43,    80,    81,     2,     3,     4,   127,    82,    92,   128,
       5,   114,    72,    30,    17,    93,    77,    30,    17,     1,
       2,     3,     4,    91,    58,    59,    87,     5,   134,   135,
      99,   105,   122,   123,   100,   124,   125,   126,   136,   129,
     146,   147,   137,    23,    24,   140,   145,    62,   149,   150,
      78,    73,   121,     0,     0,   130
  };

  const short int
   parser ::yycheck_[] =
  {
       2,     3,     4,     5,    25,    68,    58,    70,    88,    29,
      91,    91,     7,     0,    28,    29,    34,    69,     4,     5,
       6,    42,     9,    75,    19,    11,    28,   108,   108,     4,
       5,     6,    18,    18,    54,    14,    57,    89,    18,    41,
      29,    43,    29,    18,    29,    24,    48,    27,    27,     4,
       5,     6,    17,    28,    29,     4,     5,     6,    79,    24,
      22,   124,   143,   143,     3,    27,    14,    54,     0,    18,
       4,     5,     6,    28,    29,   127,     4,     5,     6,    28,
      29,    17,   134,   104,    18,    25,    88,    17,    24,    91,
      18,    93,    12,    13,    28,    29,    17,    17,    18,    21,
      28,    29,    12,    13,    18,    14,   108,    17,    28,    29,
      30,    31,    32,    33,    34,    24,    25,    17,    28,    29,
      30,    31,    32,    33,    34,    10,    25,    12,    13,    10,
      19,    12,    13,     4,     5,     6,    24,    22,    20,    27,
      11,   143,    18,    28,    29,    27,    18,    28,    29,     3,
       4,     5,     6,    14,    24,    25,    25,    11,    24,    25,
      25,    25,    25,    18,    32,    17,    25,    18,    25,    27,
      18,    27,    25,     7,     9,    25,    25,    37,    25,    25,
      54,    48,    93,    -1,    -1,   104
  };

  const unsigned char
   parser ::yystos_[] =
  {
       0,     3,     4,     5,     6,    11,    36,    37,    38,    39,
      40,    41,    42,    46,    51,    56,    34,    29,    57,    57,
      57,    57,     0,    38,    40,    14,    17,    24,    17,    17,
      28,    57,    58,    65,    66,    18,    47,    48,    49,    55,
      56,    65,     7,    19,    50,    59,    18,    43,    44,    45,
      57,    18,    51,    52,    53,    54,    56,    21,    24,    25,
      25,    18,    49,    57,    65,    57,    60,    61,    17,    24,
      17,    25,    18,    45,    14,    24,    27,    18,    54,    10,
      12,    13,    22,    65,    67,    68,    59,    25,    14,    24,
      25,    14,    20,    27,    18,    47,    59,    18,    47,    25,
      32,    59,    65,    22,    27,    25,    12,    13,    17,    30,
      31,    32,    33,    34,    57,    58,    62,    63,    69,    59,
      62,    61,    25,    18,    17,    25,    18,    24,    27,    27,
      68,    18,    62,    64,    24,    25,    25,    25,    18,    47,
      25,    59,    18,    27,    59,    25,    18,    27,    62,    25,
      25
  };

  const unsigned char
   parser ::yyr1_[] =
  {
       0,    35,    36,    36,    37,    37,    38,    39,    39,    40,
      40,    41,    41,    42,    42,    43,    44,    44,    45,    45,
      45,    45,    46,    46,    46,    46,    46,    46,    46,    46,
      47,    48,    48,    49,    49,    50,    51,    51,    52,    53,
      53,    54,    54,    55,    55,    55,    55,    56,    56,    56,
      57,    58,    59,    60,    60,    61,    61,    62,    62,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      67,    67,    68,    68,    68,    68,    69,    69,    69,    69,
      69,    69,    69
  };

  const unsigned char
   parser ::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     2,     1,     2,     1,
       1,     5,     7,     5,     6,     1,     1,     2,     2,     4,
       4,     6,     5,     7,     6,     8,     7,     9,     8,    10,
       1,     1,     2,     1,     1,     2,     4,     5,     1,     1,
       2,     1,     1,     3,     5,     5,     7,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       1,     2,     3,     1,     3,     1,     1,     1,     3,     4,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  parser ::yytname_[] =
  {
  "\"end\"", "error", "$undefined", "\"import\"", "\"alias\"",
  "\"struct\"", "\"enum\"", "\"public\"", "\"private\"", "\"protected\"",
  "\"const\"", "\"namespace\"", "\"false\"", "\"true\"", "\"=\"", "\"(\"",
  "\")\"", "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\"<\"", "\">\"", "\"::\"",
  "\":\"", "\";\"", "\".\"", "\",\"", "\"identifier path\"",
  "\"identifier\"", "\"float literal\"", "\"double literal\"",
  "\"integer literal\"", "\"hex literal\"", "\"string literal\"",
  "$accept", "program", "import_list", "import",
  "top_level_declaration_list", "top_level_declaration",
  "alias_declaration", "enum_declaration", "enum_body",
  "enum_body_item_list", "enum_body_item", "struct_declaration",
  "struct_body", "struct_body_declaration_list", "struct_body_declaration",
  "inheritance", "namespace_declaration", "namespace_body",
  "namespace_body_declaration_list", "namespace_body_declaration",
  "member_declaration", "type_declaration", "identifier",
  "identifier_path", "attribute_list", "attribute_body", "attribute",
  "initializer", "initializer_list", "initializer_body", "type",
  "templated_type", "template_argument_list", "template_argument",
  "literal", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   parser ::yyrline_[] =
  {
       0,   118,   118,   121,   127,   130,   135,   142,   147,   155,
     159,   166,   172,   181,   186,   194,   201,   206,   215,   219,
     224,   229,   238,   243,   249,   255,   262,   268,   275,   282,
     292,   299,   304,   313,   317,   323,   331,   336,   344,   351,
     356,   364,   368,   375,   381,   388,   395,   407,   411,   415,
     421,   428,   436,   442,   447,   455,   460,   468,   473,   478,
     483,   490,   494,   501,   505,   512,   517,   522,   529,   534,
     542,   547,   555,   560,   565,   570,   577,   582,   587,   592,
     598,   603,   608
  };

  // Print the state stack on the debug stream.
  void
   parser ::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
   parser ::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 1925 "yy_parser.cpp" // lalr1.cc:1167
#line 616 "parser.y" // lalr1.cc:1168


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
