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
      case 37: // top_level_declaration_list
      case 38: // top_level_declaration
      case 39: // alias_declaration
      case 40: // struct_declaration
      case 41: // struct_body
      case 42: // struct_body_declaration_list
      case 43: // struct_body_declaration
      case 44: // inheritance
      case 45: // namespace_declaration
      case 46: // namespace_body
      case 47: // namespace_body_declaration_list
      case 48: // namespace_body_declaration
      case 49: // member_declaration
      case 50: // type_declaration
      case 51: // identifier
      case 52: // identifier_path
      case 53: // attribute_list
      case 54: // attribute_body
      case 55: // attribute
      case 56: // initializer
      case 57: // initializer_list
      case 58: // initializer_body
      case 59: // type
      case 60: // templated_type
      case 61: // template_argument_list
      case 62: // template_argument
      case 63: // literal
        value.move< ::ast::Node > (that.value);
        break;

      case 30: // "double literal"
        value.move< double > (that.value);
        break;

      case 29: // "float literal"
        value.move< float > (that.value);
        break;

      case 31: // "integer literal"
        value.move< int64_t > (that.value);
        break;

      case 27: // "identifier path"
      case 28: // "identifier"
      case 32: // "string literal"
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
      case 37: // top_level_declaration_list
      case 38: // top_level_declaration
      case 39: // alias_declaration
      case 40: // struct_declaration
      case 41: // struct_body
      case 42: // struct_body_declaration_list
      case 43: // struct_body_declaration
      case 44: // inheritance
      case 45: // namespace_declaration
      case 46: // namespace_body
      case 47: // namespace_body_declaration_list
      case 48: // namespace_body_declaration
      case 49: // member_declaration
      case 50: // type_declaration
      case 51: // identifier
      case 52: // identifier_path
      case 53: // attribute_list
      case 54: // attribute_body
      case 55: // attribute
      case 56: // initializer
      case 57: // initializer_list
      case 58: // initializer_body
      case 59: // type
      case 60: // templated_type
      case 61: // template_argument_list
      case 62: // template_argument
      case 63: // literal
        value.copy< ::ast::Node > (that.value);
        break;

      case 30: // "double literal"
        value.copy< double > (that.value);
        break;

      case 29: // "float literal"
        value.copy< float > (that.value);
        break;

      case 31: // "integer literal"
        value.copy< int64_t > (that.value);
        break;

      case 27: // "identifier path"
      case 28: // "identifier"
      case 32: // "string literal"
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

#line 519 "yy_parser.cpp" // lalr1.cc:741

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
      case 37: // top_level_declaration_list
      case 38: // top_level_declaration
      case 39: // alias_declaration
      case 40: // struct_declaration
      case 41: // struct_body
      case 42: // struct_body_declaration_list
      case 43: // struct_body_declaration
      case 44: // inheritance
      case 45: // namespace_declaration
      case 46: // namespace_body
      case 47: // namespace_body_declaration_list
      case 48: // namespace_body_declaration
      case 49: // member_declaration
      case 50: // type_declaration
      case 51: // identifier
      case 52: // identifier_path
      case 53: // attribute_list
      case 54: // attribute_body
      case 55: // attribute
      case 56: // initializer
      case 57: // initializer_list
      case 58: // initializer_body
      case 59: // type
      case 60: // templated_type
      case 61: // template_argument_list
      case 62: // template_argument
      case 63: // literal
        yylhs.value.build< ::ast::Node > ();
        break;

      case 30: // "double literal"
        yylhs.value.build< double > ();
        break;

      case 29: // "float literal"
        yylhs.value.build< float > ();
        break;

      case 31: // "integer literal"
        yylhs.value.build< int64_t > ();
        break;

      case 27: // "identifier path"
      case 28: // "identifier"
      case 32: // "string literal"
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
#line 113 "parser.y" // lalr1.cc:859
    {
        }
#line 678 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 116 "parser.y" // lalr1.cc:859
    {
            builder.get_ast_root_node().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
        }
#line 686 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 122 "parser.y" // lalr1.cc:859
    {
            }
#line 693 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 125 "parser.y" // lalr1.cc:859
    {
            }
#line 700 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 130 "parser.y" // lalr1.cc:859
    {
            builder.start_file(yystack_[0].value.as< std::string > ().substr(1, yystack_[0].value.as< std::string > ().size() - 2));
        }
#line 708 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 137 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST, builder.get_location());
                                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                            }
#line 717 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 142 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                            }
#line 726 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 149 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 734 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 153 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 742 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 160 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
                    }
#line 752 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 166 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[5].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 763 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 175 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                    }
#line 772 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 180 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                    }
#line 782 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 186 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[5].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                    }
#line 792 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 192 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[6].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                    }
#line 803 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 201 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::STRUCT_BODY, builder.get_location());
                yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
            }
#line 812 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 208 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 821 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 213 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 830 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 221 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 838 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 225 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                        }
#line 846 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 231 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INHERITANCE, builder.get_location());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("visibility", "public"));
            }
#line 856 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 239 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
            }
#line 865 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 244 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
            }
#line 875 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 252 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::NAMESPACE_BODY, builder.get_location());
                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
                }
#line 884 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 259 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 893 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 264 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                                }
#line 902 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 271 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                            }
#line 910 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 275 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                            }
#line 918 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 282 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
                    }
#line 928 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 288 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 939 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 295 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[4].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[1].value.as< ::ast::Node > ());
                    }
#line 950 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 302 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[6].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[5].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 962 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 314 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                    }
#line 970 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 318 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = yystack_[0].value.as< ::ast::Node > ();
                    }
#line 978 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 324 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::IDENTIFIER, builder.get_location());
                yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< std::string > ()));
            }
#line 987 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 331 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::IDENTIFIER, builder.get_location());
                    yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< std::string > ()));
                }
#line 996 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 339 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = yystack_[1].value.as< ::ast::Node > ();
                }
#line 1004 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 345 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST, builder.get_location());
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                }
#line 1013 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 350 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ::ast::Node > ()));
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                }
#line 1022 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 357 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::ATTRIBUTE, builder.get_location());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
            }
#line 1032 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 365 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
            }
#line 1041 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 370 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ::ast::Node > ()));
            }
#line 1050 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 377 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                    }
#line 1058 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 381 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                    }
#line 1067 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 388 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > ().add_child(std::move(yystack_[0].value.as< ::ast::Node > ()));
                    }
#line 1075 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 392 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ::ast::Node > ()));
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                    }
#line 1084 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 399 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
        }
#line 1093 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 404 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
        }
#line 1102 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 409 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
        }
#line 1111 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 416 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[2].value.as< ::ast::Node > ());
                }
#line 1120 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 421 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    yylhs.value.as< ::ast::Node > ().add_child(yystack_[3].value.as< ::ast::Node > ());
                    yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ::ast::Node > ()));
                }
#line 1130 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 429 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LIST, builder.get_location());
                            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                        }
#line 1139 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 434 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ::ast::Node > ()));
                            yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                        }
#line 1148 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 441 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(yystack_[0].value.as< ::ast::Node > ());
                    }
#line 1157 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 446 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(ast::Node(ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ast::Attribute("value", true)));
                    }
#line 1166 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 451 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ::ast::Node > ().add_child(ast::Node(ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ast::Attribute("value", false)));
                    }
#line 1175 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 458 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< float > ()));
        }
#line 1184 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 463 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< double > ()));
        }
#line 1193 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 468 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< int64_t > ()));
        }
#line 1202 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 473 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", yystack_[0].value.as< std::string > ().substr(1, yystack_[0].value.as< std::string > ().size() - 2)));
        }
#line 1211 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 478 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", false));
        }
#line 1220 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 483 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ::ast::Node > () = ast::Node(ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ::ast::Node > ().add_attribute(ast::Attribute("value", true));
        }
#line 1229 "yy_parser.cpp" // lalr1.cc:859
    break;


#line 1233 "yy_parser.cpp" // lalr1.cc:859
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


  const signed char  parser ::yypact_ninf_ = -75;

  const signed char  parser ::yytable_ninf_ = -1;

  const signed char
   parser ::yypact_[] =
  {
      66,   -24,   -14,   -14,   -14,    26,    28,   -75,    85,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,    21,    -5,    36,   -75,
     -75,   -75,    69,     5,    31,    75,   -75,    22,   -75,    70,
     -75,    24,    45,    16,   -75,   -75,   -75,   -14,    69,    44,
     -75,   -75,    81,    85,   -75,   -75,    56,    60,   -75,   -75,
      77,   -75,     0,   -75,    11,   -75,   -75,   -75,   -75,   -75,
     -75,    61,   -75,   -14,    78,   -75,    43,    60,   -75,    79,
      87,   -75,    29,    73,    62,   -75,   -75,   -75,   -75,    34,
     -75,   -75,   -75,   -75,    76,   -75,   -75,    82,   -75,    83,
     -75,    43,   -75,   -14,   -75,   -75,    32,    60,   -75,   -75,
     -75,   -75,   -75,   -75,    43,    84,   -75,   -75
  };

  const unsigned char
   parser ::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     4,     3,     7,
      35,    34,    10,     9,     6,    36,     0,     0,     0,     1,
       5,     8,     0,     0,     0,     0,    37,    49,    48,     0,
      50,     0,     0,    17,    18,    21,    20,     0,     0,     0,
      23,    29,     0,    25,    26,    28,     0,     0,    11,    13,
       0,    19,     0,    22,     0,    24,    27,    57,    56,    51,
      55,     0,    53,     0,     0,    14,     0,     0,    30,     0,
       0,    52,     0,     0,     0,    39,    12,    62,    63,     0,
      58,    59,    60,    61,     0,    43,    42,     0,    15,     0,
      54,     0,    38,     0,    44,    46,     0,     0,    32,    31,
      16,    41,    40,    45,     0,     0,    47,    33
  };

  const signed char
   parser ::yypgoto_[] =
  {
     -75,   -75,   -75,    99,   -75,   101,   -75,   -75,    57,   -75,
      80,   -75,   -18,   -75,   -75,    67,   -75,     4,    -2,   -75,
     -61,   -75,    19,   -74,   -75,   -75,   -19,   -75,   -75,    42,
     -75
  };

  const signed char
   parser ::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    10,    11,    32,    33,
      34,    39,    12,    42,    43,    44,    35,    36,    27,    28,
      64,    74,    75,    84,    85,    96,    37,    30,    61,    62,
      86
  };

  const unsigned char
   parser ::yytable_[] =
  {
      16,    17,    18,    29,    13,    95,    87,    41,    14,     2,
       3,    23,    13,    66,    15,     2,     3,   101,    24,    53,
       2,     3,    31,    67,    68,    41,    19,    60,    69,    45,
     106,     1,    26,    15,    22,    52,   105,    38,    26,    15,
      57,    58,    46,    26,    15,    77,    78,    45,    49,   103,
      79,    94,    25,    60,    77,    78,    26,    15,   104,    79,
      54,    73,    50,    80,    81,    82,    83,    57,    58,     1,
       2,     3,    80,    81,    82,    83,     4,    59,    63,     2,
       3,    92,    71,    26,    15,     4,    91,    72,    93,     2,
       3,    73,    40,    47,    48,     4,    26,    15,    55,    97,
      98,    65,    76,    88,    89,    20,    99,   100,   107,    21,
      56,    70,   102,    51,    90
  };

  const unsigned char
   parser ::yycheck_[] =
  {
       2,     3,     4,    22,     0,    79,    67,    25,    32,     4,
       5,    16,     8,    13,    28,     4,     5,    91,    23,    38,
       4,     5,    17,    23,    24,    43,     0,    46,    17,    25,
     104,     3,    27,    28,    13,    37,    97,     6,    27,    28,
      11,    12,    20,    27,    28,    11,    12,    43,    24,    17,
      16,    17,    16,    72,    11,    12,    27,    28,    26,    16,
      16,    63,    17,    29,    30,    31,    32,    11,    12,     3,
       4,     5,    29,    30,    31,    32,    10,    21,    18,     4,
       5,    19,    21,    27,    28,    10,    13,    26,    26,     4,
       5,    93,    17,    23,    24,    10,    27,    28,    17,    23,
      24,    24,    24,    24,    17,     6,    24,    24,    24,     8,
      43,    54,    93,    33,    72
  };

  const unsigned char
   parser ::yystos_[] =
  {
       0,     3,     4,     5,    10,    34,    35,    36,    37,    38,
      39,    40,    45,    50,    32,    28,    51,    51,    51,     0,
      36,    38,    13,    16,    23,    16,    27,    51,    52,    59,
      60,    17,    41,    42,    43,    49,    50,    59,     6,    44,
      17,    45,    46,    47,    48,    50,    20,    23,    24,    24,
      17,    43,    51,    59,    16,    17,    48,    11,    12,    21,
      59,    61,    62,    18,    53,    24,    13,    23,    24,    17,
      41,    21,    26,    51,    54,    55,    24,    11,    12,    16,
      29,    30,    31,    32,    56,    57,    63,    53,    24,    17,
      62,    13,    19,    26,    17,    56,    58,    23,    24,    24,
      24,    56,    55,    17,    26,    53,    56,    24
  };

  const unsigned char
   parser ::yyr1_[] =
  {
       0,    33,    34,    34,    35,    35,    36,    37,    37,    38,
      38,    39,    39,    40,    40,    40,    40,    41,    42,    42,
      43,    43,    44,    45,    45,    46,    47,    47,    48,    48,
      49,    49,    49,    49,    50,    50,    51,    52,    53,    54,
      54,    55,    56,    56,    57,    57,    58,    58,    59,    59,
      59,    60,    60,    61,    61,    62,    62,    62,    63,    63,
      63,    63,    63,    63
  };

  const unsigned char
   parser ::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     2,     1,     2,     1,
       1,     5,     7,     5,     6,     7,     8,     1,     1,     2,
       1,     1,     2,     4,     5,     1,     1,     2,     1,     1,
       3,     5,     5,     7,     1,     1,     1,     1,     3,     1,
       3,     3,     1,     1,     2,     3,     1,     3,     1,     1,
       1,     3,     4,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1
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
  "\";\"", "\".\"", "\",\"", "\"identifier path\"", "\"identifier\"",
  "\"float literal\"", "\"double literal\"", "\"integer literal\"",
  "\"string literal\"", "$accept", "program", "import_list", "import",
  "top_level_declaration_list", "top_level_declaration",
  "alias_declaration", "struct_declaration", "struct_body",
  "struct_body_declaration_list", "struct_body_declaration", "inheritance",
  "namespace_declaration", "namespace_body",
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
       0,   112,   112,   115,   121,   124,   129,   136,   141,   148,
     152,   159,   165,   174,   179,   185,   191,   200,   207,   212,
     220,   224,   230,   238,   243,   251,   258,   263,   270,   274,
     281,   287,   294,   301,   313,   317,   323,   330,   338,   344,
     349,   356,   364,   369,   376,   380,   387,   391,   398,   403,
     408,   415,   420,   428,   433,   440,   445,   450,   457,   462,
     467,   472,   477,   482
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
#line 1687 "yy_parser.cpp" // lalr1.cc:1167
#line 490 "parser.y" // lalr1.cc:1168


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
