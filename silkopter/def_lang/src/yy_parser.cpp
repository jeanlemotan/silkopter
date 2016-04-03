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
#line 32 "parser.y" // lalr1.cc:413

    #include "ast/Lexer.h"

    // Prototype of the yylex function providing subsequent tokens.
    yy::parser::symbol_type yylex(ast::Builder& builder)
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
   parser :: parser  (ast::Builder& builder_yyarg)
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
      case 36: // top_level_declaration_list
      case 37: // top_level_declaration
      case 38: // alias_declaration
      case 39: // struct_declaration
      case 40: // struct_body
      case 41: // struct_body_declaration_list
      case 42: // struct_body_declaration
      case 43: // inheritance
      case 45: // namespace_declaration
      case 46: // namespace_body
      case 47: // namespace_body_declaration_list
      case 48: // namespace_body_declaration
      case 49: // member_declaration
      case 50: // type_declaration
      case 51: // identifier
      case 52: // attribute_list
      case 53: // attribute_body
      case 54: // attribute
      case 55: // expression
      case 56: // initializer_list
      case 57: // initializer_body
      case 58: // type
      case 59: // templated_type
      case 60: // template_argument_list
      case 61: // template_argument
      case 62: // literal
        value.move< ::ast::Node > (that.value);
        break;

      case 29: // "double literal"
        value.move< double > (that.value);
        break;

      case 28: // "float literal"
        value.move< float > (that.value);
        break;

      case 30: // "integer literal"
        value.move< int64_t > (that.value);
        break;

      case 27: // "identifier"
      case 31: // "string literal"
        value.move< std::string > (that.value);
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
      case 36: // top_level_declaration_list
      case 37: // top_level_declaration
      case 38: // alias_declaration
      case 39: // struct_declaration
      case 40: // struct_body
      case 41: // struct_body_declaration_list
      case 42: // struct_body_declaration
      case 43: // inheritance
      case 45: // namespace_declaration
      case 46: // namespace_body
      case 47: // namespace_body_declaration_list
      case 48: // namespace_body_declaration
      case 49: // member_declaration
      case 50: // type_declaration
      case 51: // identifier
      case 52: // attribute_list
      case 53: // attribute_body
      case 54: // attribute
      case 55: // expression
      case 56: // initializer_list
      case 57: // initializer_body
      case 58: // type
      case 59: // templated_type
      case 60: // template_argument_list
      case 61: // template_argument
      case 62: // literal
        value.copy< ::ast::Node > (that.value);
        break;

      case 29: // "double literal"
        value.copy< double > (that.value);
        break;

      case 28: // "float literal"
        value.copy< float > (that.value);
        break;

      case 30: // "integer literal"
        value.copy< int64_t > (that.value);
        break;

      case 27: // "identifier"
      case 31: // "string literal"
        value.copy< std::string > (that.value);
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

#line 515 "yy_parser.cpp" // lalr1.cc:741

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
      case 36: // top_level_declaration_list
      case 37: // top_level_declaration
      case 38: // alias_declaration
      case 39: // struct_declaration
      case 40: // struct_body
      case 41: // struct_body_declaration_list
      case 42: // struct_body_declaration
      case 43: // inheritance
      case 45: // namespace_declaration
      case 46: // namespace_body
      case 47: // namespace_body_declaration_list
      case 48: // namespace_body_declaration
      case 49: // member_declaration
      case 50: // type_declaration
      case 51: // identifier
      case 52: // attribute_list
      case 53: // attribute_body
      case 54: // attribute
      case 55: // expression
      case 56: // initializer_list
      case 57: // initializer_body
      case 58: // type
      case 59: // templated_type
      case 60: // template_argument_list
      case 61: // template_argument
      case 62: // literal
        yylhs.value.build< ::ast::Node > ();
        break;

      case 29: // "double literal"
        yylhs.value.build< double > ();
        break;

      case 28: // "float literal"
        yylhs.value.build< float > ();
        break;

      case 30: // "integer literal"
        yylhs.value.build< int64_t > ();
        break;

      case 27: // "identifier"
      case 31: // "string literal"
        yylhs.value.build< std::string > ();
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
#line 109 "parser.y" // lalr1.cc:859
    {
        }
#line 672 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 112 "parser.y" // lalr1.cc:859
    {
            builder.get_root_node().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
        }
#line 680 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 118 "parser.y" // lalr1.cc:859
    {
            }
#line 687 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 121 "parser.y" // lalr1.cc:859
    {
            }
#line 694 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 126 "parser.y" // lalr1.cc:859
    {
            builder.start_file(yystack_[0].value.as< std::string > ());
        }
#line 702 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 133 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST);
                                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                            }
#line 711 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 138 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                            }
#line 720 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 145 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 728 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 149 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 736 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 156 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::ALIAS_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
                    }
#line 746 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 162 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::ALIAS_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[5].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 757 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 171 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                    }
#line 766 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 176 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                    }
#line 776 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 182 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[5].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                    }
#line 786 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 188 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[6].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                    }
#line 797 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 197 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_BODY);
                yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
            }
#line 806 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 204 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST);
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 815 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 209 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 824 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 217 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 832 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 221 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 840 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 235 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::NAMESPACE_DECLARATION);
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
            }
#line 849 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 240 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::NAMESPACE_DECLARATION);
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
            }
#line 859 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 248 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::NAMESPACE_BODY);
                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
                }
#line 868 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 255 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST);
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 877 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 260 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 886 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 267 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                            }
#line 894 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 271 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                            }
#line 902 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 278 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
                    }
#line 912 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 284 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 923 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 291 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
                    }
#line 934 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 298 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[6].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[5].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 946 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 310 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                    }
#line 954 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 314 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                    }
#line 962 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 320 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::IDENTIFIER);
                yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< std::string > ()));
            }
#line 971 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 328 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = yystack_[1].value.as< ::ast::Node > ();
                }
#line 979 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 334 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST);
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                }
#line 988 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 339 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ::ast::Node > ()));
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                }
#line 997 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 346 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::ATTRIBUTE);
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
            }
#line 1007 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 354 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::EXPRESSION);
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
            }
#line 1016 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 359 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::EXPRESSION);
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
            }
#line 1025 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 364 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = yystack_[1].value.as< ::ast::Node > ();
            }
#line 1033 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 370 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER_LIST);
                    }
#line 1041 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 374 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER_LIST);
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 1050 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 381 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER_LIST);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                    }
#line 1059 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 386 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ::ast::Node > ()));
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                    }
#line 1068 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 393 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TYPE);
            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
        }
#line 1077 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 398 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TYPE);
            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
        }
#line 1086 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 405 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_INSTANTIATION);
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                }
#line 1095 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 410 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_INSTANTIATION);
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                }
#line 1105 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 418 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST);
                            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                        }
#line 1114 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 423 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ::ast::Node > ()));
                            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                        }
#line 1123 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 430 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT);
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                    }
#line 1132 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 435 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT);
                        yylhs.value.as< ::ast::Node > ().add_child(ast::Node(ast::Node::Type::LITERAL).add_attribute(ast::Attribute("value", yystack_[0].value.as< std::string > ())));
                    }
#line 1141 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 440 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT);
                        yylhs.value.as< ::ast::Node > ().add_child(ast::Node(ast::Node::Type::LITERAL).add_attribute(ast::Attribute("value", yystack_[0].value.as< int64_t > ())));
                    }
#line 1150 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 445 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT);
                        yylhs.value.as< ::ast::Node > ().add_child(ast::Node(ast::Node::Type::LITERAL).add_attribute(ast::Attribute("value", true)));
                    }
#line 1159 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 450 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT);
                        yylhs.value.as< ::ast::Node > ().add_child(ast::Node(ast::Node::Type::LITERAL).add_attribute(ast::Attribute("value", false)));
                    }
#line 1168 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 457 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL);
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< float > ()));
        }
#line 1177 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 462 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL);
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< double > ()));
        }
#line 1186 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 467 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL);
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< int64_t > ()));
        }
#line 1195 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 472 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL);
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< std::string > ()));
        }
#line 1204 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 477 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL);
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", false));
        }
#line 1213 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 482 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL);
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", true));
        }
#line 1222 "yy_parser.cpp" // lalr1.cc:859
    break;


#line 1226 "yy_parser.cpp" // lalr1.cc:859
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


  const signed char  parser ::yypact_ninf_ = -77;

  const signed char  parser ::yytable_ninf_ = -1;

  const signed char
   parser ::yypact_[] =
  {
      94,   -16,    -6,    -6,    -6,    53,    48,   -77,    96,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,    42,    -5,    49,   -77,
     -77,   -77,    -6,     5,    17,    79,    56,    51,   -77,    58,
      68,    12,   -77,   -77,   -77,    -6,   -77,   -77,   -77,    70,
      -6,   -77,   -77,    73,    96,   -77,   -77,    50,    69,   -77,
     -77,    78,   -77,     7,     9,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,    21,   -77,    -6,    81,   -77,    38,
      69,   -77,    83,    76,   -77,    61,    90,    44,   -77,   -77,
     -77,   -77,    38,    29,   -77,   -77,   -77,   -77,    55,   -77,
     -77,    84,   -77,    85,   -77,    38,   -77,    -6,    95,   -77,
     -77,    18,    69,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
      38,    87,   -77,   -77
  };

  const unsigned char
   parser ::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     4,     3,     7,
      38,    37,    10,     9,     6,    39,     0,     0,     0,     1,
       5,     8,     0,     0,     0,     0,    51,     0,    52,     0,
       0,    17,    18,    21,    20,     0,    23,    24,    25,     0,
       0,    26,    32,     0,    28,    29,    31,     0,     0,    11,
      13,     0,    19,     0,     0,    22,    27,    30,    61,    60,
      53,    59,    58,    57,     0,    55,     0,     0,    14,     0,
       0,    33,     0,     0,    54,     0,     0,     0,    41,    12,
      66,    67,     0,     0,    62,    63,    64,    65,     0,    45,
      44,     0,    15,     0,    56,     0,    40,     0,     0,    47,
      49,     0,     0,    35,    34,    16,    43,    42,    46,    48,
       0,     0,    50,    36
  };

  const signed char
   parser ::yypgoto_[] =
  {
     -77,   -77,   -77,    88,   -77,   104,   -77,   -77,    59,   -77,
      86,   -77,   -77,   -17,   -77,   -77,    71,   -77,     4,    -2,
     -65,   -77,    19,   -76,   -77,   -77,   -19,   -77,   -77,    39,
     -77
  };

  const signed char
   parser ::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    10,    11,    30,    31,
      32,    39,    40,    12,    43,    44,    45,    33,    34,    26,
      67,    77,    78,    88,    89,   101,    35,    28,    64,    65,
      90
  };

  const unsigned char
   parser ::yytable_[] =
  {
      16,    17,    18,    27,    13,    91,    98,   100,    42,     2,
       3,    23,    13,     2,     3,    14,     2,     3,    24,   106,
      69,    15,    29,    36,    37,    38,    72,    42,    63,    46,
      70,    71,    15,    53,   112,   109,    15,   111,    55,    15,
      80,    81,    74,    82,   110,    83,    99,    75,    46,    80,
      81,     1,    82,    19,    83,    22,    63,    84,    85,    86,
      87,    58,    59,    96,    76,    25,    84,    85,    86,    87,
      97,    60,    58,    59,    48,    49,    47,    15,   102,   103,
      61,    62,    50,     2,     3,    51,    54,    66,    15,     4,
      56,    61,    62,    93,    20,    76,    41,     1,     2,     3,
       2,     3,    68,    95,     4,    79,     4,    92,   104,   105,
     108,   113,    21,    73,    94,    57,   107,    52
  };

  const unsigned char
   parser ::yycheck_[] =
  {
       2,     3,     4,    22,     0,    70,    82,    83,    25,     4,
       5,    16,     8,     4,     5,    31,     4,     5,    23,    95,
      13,    27,    17,     6,     7,     8,    17,    44,    47,    25,
      23,    24,    27,    35,   110,    17,    27,   102,    40,    27,
      11,    12,    21,    14,    26,    16,    17,    26,    44,    11,
      12,     3,    14,     0,    16,    13,    75,    28,    29,    30,
      31,    11,    12,    19,    66,    16,    28,    29,    30,    31,
      26,    21,    11,    12,    23,    24,    20,    27,    23,    24,
      30,    31,    24,     4,     5,    17,    16,    18,    27,    10,
      17,    30,    31,    17,     6,    97,    17,     3,     4,     5,
       4,     5,    24,    13,    10,    24,    10,    24,    24,    24,
      15,    24,     8,    54,    75,    44,    97,    31
  };

  const unsigned char
   parser ::yystos_[] =
  {
       0,     3,     4,     5,    10,    33,    34,    35,    36,    37,
      38,    39,    45,    50,    31,    27,    51,    51,    51,     0,
      35,    37,    13,    16,    23,    16,    51,    58,    59,    17,
      40,    41,    42,    49,    50,    58,     6,     7,     8,    43,
      44,    17,    45,    46,    47,    48,    50,    20,    23,    24,
      24,    17,    42,    51,    16,    51,    17,    48,    11,    12,
      21,    30,    31,    58,    60,    61,    18,    52,    24,    13,
      23,    24,    17,    40,    21,    26,    51,    53,    54,    24,
      11,    12,    14,    16,    28,    29,    30,    31,    55,    56,
      62,    52,    24,    17,    61,    13,    19,    26,    55,    17,
      55,    57,    23,    24,    24,    24,    55,    54,    15,    17,
      26,    52,    55,    24
  };

  const unsigned char
   parser ::yyr1_[] =
  {
       0,    32,    33,    33,    34,    34,    35,    36,    36,    37,
      37,    38,    38,    39,    39,    39,    39,    40,    41,    41,
      42,    42,    43,    44,    44,    44,    45,    45,    46,    47,
      47,    48,    48,    49,    49,    49,    49,    50,    50,    51,
      52,    53,    53,    54,    55,    55,    55,    56,    56,    57,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    61,
      61,    61,    62,    62,    62,    62,    62,    62
  };

  const unsigned char
   parser ::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     2,     1,     2,     1,
       1,     5,     7,     5,     6,     7,     8,     1,     1,     2,
       1,     1,     2,     1,     1,     1,     4,     5,     1,     1,
       2,     1,     1,     3,     5,     5,     7,     1,     1,     1,
       3,     1,     3,     3,     1,     1,     3,     2,     3,     1,
       3,     1,     1,     3,     4,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  parser ::yytname_[] =
  {
  "\"end\"", "error", "$undefined", "\"import\"", "\"alias\"",
  "\"struct\"", "\"public\"", "\"private\"", "\"protected\"", "\"const\"",
  "\"namespace\"", "\"false\"", "\"true\"", "\"=\"", "\"(\"", "\")\"",
  "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\"<\"", "\">\"", "\"::\"", "\":\"",
  "\";\"", "\".\"", "\",\"", "\"identifier\"", "\"float literal\"",
  "\"double literal\"", "\"integer literal\"", "\"string literal\"",
  "$accept", "program", "import_list", "import",
  "top_level_declaration_list", "top_level_declaration",
  "alias_declaration", "struct_declaration", "struct_body",
  "struct_body_declaration_list", "struct_body_declaration", "inheritance",
  "inheritance_type", "namespace_declaration", "namespace_body",
  "namespace_body_declaration_list", "namespace_body_declaration",
  "member_declaration", "type_declaration", "identifier", "attribute_list",
  "attribute_body", "attribute", "expression", "initializer_list",
  "initializer_body", "type", "templated_type", "template_argument_list",
  "template_argument", "literal", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   parser ::yyrline_[] =
  {
       0,   108,   108,   111,   117,   120,   125,   132,   137,   144,
     148,   155,   161,   170,   175,   181,   187,   196,   203,   208,
     216,   220,   226,   229,   230,   231,   234,   239,   247,   254,
     259,   266,   270,   277,   283,   290,   297,   309,   313,   319,
     327,   333,   338,   345,   353,   358,   363,   369,   373,   380,
     385,   392,   397,   404,   409,   417,   422,   429,   434,   439,
     444,   449,   456,   461,   466,   471,   476,   481
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
#line 1682 "yy_parser.cpp" // lalr1.cc:1167
#line 489 "parser.y" // lalr1.cc:1168


namespace yy
{
// Mandatory error function
void parser::error (const parser::location_type& loc, const std::string& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

}
