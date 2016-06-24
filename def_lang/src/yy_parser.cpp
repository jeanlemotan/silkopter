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
        value.move< int64_t > (that.value);
        break;

      case 28: // "identifier path"
      case 29: // "identifier"
      case 33: // "string literal"
        value.move< std::string > (that.value);
        break;

      case 38: // top_level_declaration_list
      case 39: // top_level_declaration
      case 40: // alias_declaration
      case 41: // enum_declaration
      case 42: // enum_body
      case 43: // enum_body_item_list
      case 44: // enum_body_item
      case 45: // struct_declaration
      case 46: // struct_body
      case 47: // struct_body_declaration_list
      case 48: // struct_body_declaration
      case 49: // inheritance
      case 50: // namespace_declaration
      case 51: // namespace_body
      case 52: // namespace_body_declaration_list
      case 53: // namespace_body_declaration
      case 54: // member_declaration
      case 55: // type_declaration
      case 56: // identifier
      case 57: // identifier_path
      case 58: // attribute_list
      case 59: // attribute_body
      case 60: // attribute
      case 61: // initializer
      case 62: // initializer_list
      case 63: // initializer_body
      case 64: // type
      case 65: // templated_type
      case 66: // template_argument_list
      case 67: // template_argument
      case 68: // literal
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
        value.copy< int64_t > (that.value);
        break;

      case 28: // "identifier path"
      case 29: // "identifier"
      case 33: // "string literal"
        value.copy< std::string > (that.value);
        break;

      case 38: // top_level_declaration_list
      case 39: // top_level_declaration
      case 40: // alias_declaration
      case 41: // enum_declaration
      case 42: // enum_body
      case 43: // enum_body_item_list
      case 44: // enum_body_item
      case 45: // struct_declaration
      case 46: // struct_body
      case 47: // struct_body_declaration_list
      case 48: // struct_body_declaration
      case 49: // inheritance
      case 50: // namespace_declaration
      case 51: // namespace_body
      case 52: // namespace_body_declaration_list
      case 53: // namespace_body_declaration
      case 54: // member_declaration
      case 55: // type_declaration
      case 56: // identifier
      case 57: // identifier_path
      case 58: // attribute_list
      case 59: // attribute_body
      case 60: // attribute
      case 61: // initializer
      case 62: // initializer_list
      case 63: // initializer_body
      case 64: // type
      case 65: // templated_type
      case 66: // template_argument_list
      case 67: // template_argument
      case 68: // literal
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

#line 527 "yy_parser.cpp" // lalr1.cc:741

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
        yylhs.value.build< int64_t > ();
        break;

      case 28: // "identifier path"
      case 29: // "identifier"
      case 33: // "string literal"
        yylhs.value.build< std::string > ();
        break;

      case 38: // top_level_declaration_list
      case 39: // top_level_declaration
      case 40: // alias_declaration
      case 41: // enum_declaration
      case 42: // enum_body
      case 43: // enum_body_item_list
      case 44: // enum_body_item
      case 45: // struct_declaration
      case 46: // struct_body
      case 47: // struct_body_declaration_list
      case 48: // struct_body_declaration
      case 49: // inheritance
      case 50: // namespace_declaration
      case 51: // namespace_body
      case 52: // namespace_body_declaration_list
      case 53: // namespace_body_declaration
      case 54: // member_declaration
      case 55: // type_declaration
      case 56: // identifier
      case 57: // identifier_path
      case 58: // attribute_list
      case 59: // attribute_body
      case 60: // attribute
      case 61: // initializer
      case 62: // initializer_list
      case 63: // initializer_body
      case 64: // type
      case 65: // templated_type
      case 66: // template_argument_list
      case 67: // template_argument
      case 68: // literal
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
#line 118 "parser.y" // lalr1.cc:859
    {
        }
#line 690 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 121 "parser.y" // lalr1.cc:859
    {
            builder.get_ast_root_node().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
        }
#line 698 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 127 "parser.y" // lalr1.cc:859
    {
            }
#line 705 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 130 "parser.y" // lalr1.cc:859
    {
            }
#line 712 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 135 "parser.y" // lalr1.cc:859
    {
            builder.start_file(yystack_[0].value.as< std::string > ().substr(1, yystack_[0].value.as< std::string > ().size() - 2));
        }
#line 720 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 142 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                            }
#line 729 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 147 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                            }
#line 738 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 154 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 746 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 158 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 754 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 165 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
                    }
#line 764 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 171 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ALIAS_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 775 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 180 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ENUM_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    }
#line 784 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 185 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ENUM_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 794 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 193 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ENUM_BODY, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
            }
#line 803 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 200 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 812 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 205 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 821 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 213 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[1].value.as< ts::ast::Node > ();
                }
#line 829 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 217 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[3].value.as< ts::ast::Node > ();
                    yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("integral_value", yystack_[1].value.as< int64_t > ()));
                }
#line 838 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 222 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[3].value.as< ts::ast::Node > ();
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                }
#line 847 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 227 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[5].value.as< ts::ast::Node > ();
                    yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("integral_value", yystack_[3].value.as< int64_t > ()));
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                }
#line 857 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 236 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    }
#line 866 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 241 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[3].value.as< ts::ast::Node > ()));
                    }
#line 876 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 247 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 886 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 253 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[4].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 897 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 260 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    }
#line 907 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 266 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[7].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[3].value.as< ts::ast::Node > ()));
                    }
#line 918 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 273 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 929 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 280 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[8].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[4].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                    }
#line 941 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 290 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::STRUCT_BODY, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
            }
#line 950 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 297 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 959 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 302 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 968 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 310 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 976 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 314 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                        }
#line 984 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 320 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INHERITANCE, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("visibility", "public"));
            }
#line 994 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 328 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
            }
#line 1003 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 333 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_DECLARATION, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
            }
#line 1013 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 341 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::NAMESPACE_BODY, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
                }
#line 1022 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 348 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 1031 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 353 "parser.y" // lalr1.cc:859
    {
                                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                                }
#line 1040 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 360 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                            }
#line 1048 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 364 "parser.y" // lalr1.cc:859
    {
                                yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                            }
#line 1056 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 371 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
                    }
#line 1066 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 377 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 1077 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 384 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[4].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[1].value.as< ts::ast::Node > ());
                    }
#line 1088 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 391 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::MEMBER_DECLARATION, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[6].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[5].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 1100 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 403 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                    }
#line 1108 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 407 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                    }
#line 1116 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 411 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = yystack_[0].value.as< ts::ast::Node > ();
                    }
#line 1124 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 417 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::IDENTIFIER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< std::string > ()));
            }
#line 1133 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 424 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::IDENTIFIER, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< std::string > ()));
                }
#line 1142 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 432 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = yystack_[1].value.as< ts::ast::Node > ();
                }
#line 1150 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 438 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                }
#line 1159 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 443 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ts::ast::Node > ()));
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                }
#line 1168 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 450 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::ATTRIBUTE, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1178 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 458 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1187 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 463 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1196 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 468 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
            }
#line 1205 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 473 "parser.y" // lalr1.cc:859
    {
                yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[0].value.as< ts::ast::Node > ()));
            }
#line 1214 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 480 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                    }
#line 1222 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 484 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::INITIALIZER_LIST, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                    }
#line 1231 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 491 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > ().add_child(std::move(yystack_[0].value.as< ts::ast::Node > ()));
                    }
#line 1239 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 495 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ts::ast::Node > ()));
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 1248 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 502 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
        }
#line 1257 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 507 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
        }
#line 1266 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 512 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TYPE, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
        }
#line 1275 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 519 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[2].value.as< ts::ast::Node > ());
                }
#line 1284 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 524 "parser.y" // lalr1.cc:859
    {
                    yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_INSTANTIATION, builder.get_location());
                    yylhs.value.as< ts::ast::Node > ().add_child(yystack_[3].value.as< ts::ast::Node > ());
                    yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[1].value.as< ts::ast::Node > ()));
                }
#line 1294 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 532 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LIST, builder.get_location());
                            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                        }
#line 1303 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 537 "parser.y" // lalr1.cc:859
    {
                            yylhs.value.as< ts::ast::Node > ().move_children_from(std::move(yystack_[2].value.as< ts::ast::Node > ()));
                            yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                        }
#line 1312 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 544 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ());
                    }
#line 1321 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 549 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(yystack_[0].value.as< ts::ast::Node > ()).add_attribute(ts::ast::Attribute("const", true));
                    }
#line 1330 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 554 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ts::ast::Attribute("value", true)));
                    }
#line 1339 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 559 "parser.y" // lalr1.cc:859
    {
                        yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::TEMPLATE_ARGUMENT, builder.get_location());
                        yylhs.value.as< ts::ast::Node > ().add_child(ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location()).add_attribute(ts::ast::Attribute("value", false)));
                    }
#line 1348 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 566 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< float > ()));
        }
#line 1357 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 571 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< double > ()));
        }
#line 1366 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 576 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< int64_t > ()));
        }
#line 1375 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 581 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", yystack_[0].value.as< std::string > ().substr(1, yystack_[0].value.as< std::string > ().size() - 2)));
        }
#line 1384 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 586 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", false));
        }
#line 1393 "yy_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 591 "parser.y" // lalr1.cc:859
    {
            yylhs.value.as< ts::ast::Node > () = ts::ast::Node(ts::ast::Node::Type::LITERAL, builder.get_location());
            yylhs.value.as< ts::ast::Node > ().add_attribute(ts::ast::Attribute("value", true));
        }
#line 1402 "yy_parser.cpp" // lalr1.cc:859
    break;


#line 1406 "yy_parser.cpp" // lalr1.cc:859
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
      62,   -13,     4,     4,     4,     4,    59,    61,   -82,   140,
     -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,    79,    28,
     105,   112,   -82,   -82,   -82,    88,    25,     5,    -4,   132,
     -82,   118,   -82,    24,   -82,   122,   130,    92,   -82,   -82,
     -82,     4,    88,     4,    58,   135,   124,   136,     4,   -82,
      78,   -82,   -82,   137,   140,   -82,   -82,    22,   134,   -82,
     -82,   131,   -82,    85,   -82,   143,    91,   -82,    51,   134,
      66,   -82,   133,   -82,   127,   134,   -82,   -82,   -82,    88,
     -82,   -82,   -82,   -82,    20,   -82,   138,   -82,   102,   134,
     -82,   102,   -82,     4,   139,   142,   144,   141,   147,   -82,
      -8,   145,   -82,   -82,    75,   -82,   -82,   -82,    95,   -82,
     -82,   -82,   -82,   -82,   -82,   117,   -82,   -82,   146,   -82,
     -82,   -82,   148,    72,   -82,   149,   134,   -82,   -82,   -82,
     -82,   -82,    -5,   134,   -82,   -82,   -82,   150,   151,   -82,
     152,   -82,   102,   153,   -82,   155,   -82,   -82,   -82,   -82
  };

  const unsigned char
   parser ::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,     2,     4,     3,
       7,    48,    49,    47,    10,     9,     6,    50,     0,     0,
       0,     0,     1,     5,     8,     0,     0,     0,     0,     0,
      51,    65,    64,     0,    66,     0,     0,    30,    31,    34,
      33,     0,     0,     0,     0,     0,     0,     0,    15,    16,
       0,    36,    42,     0,    38,    39,    41,     0,     0,    11,
      22,     0,    32,     0,    35,     0,     0,    53,     0,     0,
       0,    13,     0,    17,     0,     0,    18,    37,    40,     0,
      74,    73,    67,    71,     0,    69,     0,    24,     0,     0,
      43,     0,    52,     0,     0,     0,     0,     0,     0,    14,
       0,     0,    72,    68,     0,    12,    79,    80,     0,    75,
      76,    77,    78,    58,    57,     0,    59,    56,     0,    55,
      54,    26,     0,     0,    23,     0,     0,    19,    20,    70,
      60,    62,     0,     0,    45,    44,    28,     0,     0,    25,
       0,    61,     0,     0,    27,     0,    21,    63,    46,    29
  };

  const short int
   parser ::yypgoto_[] =
  {
     -82,   -82,   -82,   160,   -82,   159,   -82,   -82,   -82,   -82,
     114,   -82,   -63,   -82,   154,   -82,   -14,   -82,   -82,   116,
     -82,     9,    -2,   -80,   -52,   -82,    83,   -81,   -82,   -82,
     -21,   -82,   -82,    73,   -82
  };

  const short int
   parser ::yydefgoto_[] =
  {
      -1,     6,     7,     8,     9,    10,    11,    12,    47,    48,
      49,    13,    36,    37,    38,    44,    14,    53,    54,    55,
      39,    40,    31,    32,    45,    66,    67,   115,   116,   132,
      41,    34,    84,    85,   117
  };

  const unsigned char
   parser ::yytable_[] =
  {
      18,    19,    20,    21,    33,    95,    86,    98,   114,    15,
     119,   114,    42,   141,    46,    52,   126,    96,    15,   127,
      16,    64,   142,   101,    43,    17,    50,   131,   114,     2,
       3,     4,    79,    17,    80,    81,    83,   118,    56,    63,
      52,    65,   103,    35,    82,    26,    50,   104,    58,    59,
      30,    17,    27,    30,    17,     2,     3,     4,   102,    22,
     138,   147,   114,    56,     1,     1,     2,     3,     4,    94,
       2,     3,     4,     5,   140,    68,     2,     3,     4,    30,
      17,   143,    69,    83,    97,    79,   113,    80,    81,   113,
     137,    65,    74,    25,    30,    17,     2,     3,     4,    88,
      30,    17,    75,    30,    17,    76,   113,   106,   107,    89,
      90,    92,   108,   130,   106,   107,    30,    17,    93,   108,
      30,    17,    28,    30,    17,   109,   110,   111,   112,    29,
      30,    17,   109,   110,   111,   112,     2,     3,     4,    57,
     113,   133,   134,     5,     2,     3,     4,    60,    61,    71,
      51,     5,    70,    43,    72,    77,    87,    91,    99,   100,
     122,   123,    73,   105,   121,   125,   124,    23,    24,   145,
      78,   135,   128,   136,   139,   144,   120,   129,   148,   146,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62
  };

  const short int
   parser ::yycheck_[] =
  {
       2,     3,     4,     5,    25,    68,    58,    70,    88,     0,
      91,    91,     7,    18,    18,    29,    24,    69,     9,    27,
      33,    42,    27,    75,    19,    29,    28,   108,   108,     4,
       5,     6,    10,    29,    12,    13,    57,    89,    29,    41,
      54,    43,    22,    18,    22,    17,    48,    27,    24,    25,
      28,    29,    24,    28,    29,     4,     5,     6,    79,     0,
     123,   142,   142,    54,     3,     3,     4,     5,     6,    18,
       4,     5,     6,    11,   126,    17,     4,     5,     6,    28,
      29,   133,    24,   104,    18,    10,    88,    12,    13,    91,
      18,    93,    14,    14,    28,    29,     4,     5,     6,    14,
      28,    29,    24,    28,    29,    27,   108,    12,    13,    24,
      25,    20,    17,    18,    12,    13,    28,    29,    27,    17,
      28,    29,    17,    28,    29,    30,    31,    32,    33,    17,
      28,    29,    30,    31,    32,    33,     4,     5,     6,    21,
     142,    24,    25,    11,     4,     5,     6,    25,    18,    25,
      18,    11,    17,    19,    18,    18,    25,    14,    25,    32,
      18,    17,    48,    25,    25,    18,    25,     7,     9,    18,
      54,    25,    27,    25,    25,    25,    93,   104,    25,    27,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37
  };

  const unsigned char
   parser ::yystos_[] =
  {
       0,     3,     4,     5,     6,    11,    35,    36,    37,    38,
      39,    40,    41,    45,    50,    55,    33,    29,    56,    56,
      56,    56,     0,    37,    39,    14,    17,    24,    17,    17,
      28,    56,    57,    64,    65,    18,    46,    47,    48,    54,
      55,    64,     7,    19,    49,    58,    18,    42,    43,    44,
      56,    18,    50,    51,    52,    53,    55,    21,    24,    25,
      25,    18,    48,    56,    64,    56,    59,    60,    17,    24,
      17,    25,    18,    44,    14,    24,    27,    18,    53,    10,
      12,    13,    22,    64,    66,    67,    58,    25,    14,    24,
      25,    14,    20,    27,    18,    46,    58,    18,    46,    25,
      32,    58,    64,    22,    27,    25,    12,    13,    17,    30,
      31,    32,    33,    56,    57,    61,    62,    68,    58,    61,
      60,    25,    18,    17,    25,    18,    24,    27,    27,    67,
      18,    61,    63,    24,    25,    25,    25,    18,    46,    25,
      58,    18,    27,    58,    25,    18,    27,    61,    25,    25
  };

  const unsigned char
   parser ::yyr1_[] =
  {
       0,    34,    35,    35,    36,    36,    37,    38,    38,    39,
      39,    40,    40,    41,    41,    42,    43,    43,    44,    44,
      44,    44,    45,    45,    45,    45,    45,    45,    45,    45,
      46,    47,    47,    48,    48,    49,    50,    50,    51,    52,
      52,    53,    53,    54,    54,    54,    54,    55,    55,    55,
      56,    57,    58,    59,    59,    60,    61,    61,    61,    61,
      62,    62,    63,    63,    64,    64,    64,    65,    65,    66,
      66,    67,    67,    67,    67,    68,    68,    68,    68,    68,
      68
  };

  const unsigned char
   parser ::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     2,     1,     2,     1,
       1,     5,     7,     5,     6,     1,     1,     2,     2,     4,
       4,     6,     5,     7,     6,     8,     7,     9,     8,    10,
       1,     1,     2,     1,     1,     2,     4,     5,     1,     1,
       2,     1,     1,     3,     5,     5,     7,     1,     1,     1,
       1,     1,     3,     1,     3,     3,     1,     1,     1,     1,
       2,     3,     1,     3,     1,     1,     1,     3,     4,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1
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
  "\"integer literal\"", "\"string literal\"", "$accept", "program",
  "import_list", "import", "top_level_declaration_list",
  "top_level_declaration", "alias_declaration", "enum_declaration",
  "enum_body", "enum_body_item_list", "enum_body_item",
  "struct_declaration", "struct_body", "struct_body_declaration_list",
  "struct_body_declaration", "inheritance", "namespace_declaration",
  "namespace_body", "namespace_body_declaration_list",
  "namespace_body_declaration", "member_declaration", "type_declaration",
  "identifier", "identifier_path", "attribute_list", "attribute_body",
  "attribute", "initializer", "initializer_list", "initializer_body",
  "type", "templated_type", "template_argument_list", "template_argument",
  "literal", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   parser ::yyrline_[] =
  {
       0,   117,   117,   120,   126,   129,   134,   141,   146,   153,
     157,   164,   170,   179,   184,   192,   199,   204,   212,   216,
     221,   226,   235,   240,   246,   252,   259,   265,   272,   279,
     289,   296,   301,   309,   313,   319,   327,   332,   340,   347,
     352,   359,   363,   370,   376,   383,   390,   402,   406,   410,
     416,   423,   431,   437,   442,   449,   457,   462,   467,   472,
     479,   483,   490,   494,   501,   506,   511,   518,   523,   531,
     536,   543,   548,   553,   558,   565,   570,   575,   580,   585,
     590
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
#line 1895 "yy_parser.cpp" // lalr1.cc:1167
#line 598 "parser.y" // lalr1.cc:1168


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
