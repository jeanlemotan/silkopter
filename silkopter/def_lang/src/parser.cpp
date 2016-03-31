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

#line 37 "parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.hpp"

// User implementation prologue.

#line 51 "parser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 8 "parser.y" // lalr1.cc:413

    #include <stdio.h>
    #include <string>
    #include <iostream>
%

#line 60 "parser.cpp" // lalr1.cc:413


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
#line 127 "parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
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
  parser::parser ()
#if YYDEBUG
     :yydebug_ (false),
      yycdebug_ (&std::cerr)
#endif
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  parser::syntax_error::syntax_error (const std::string& m)
    : std::runtime_error (m)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
  {
    value = other.value;
  }


  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v)
    : Base (t)
    , value (v)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
            case 33: // TIDENTIFIER

#line 76 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 372 "parser.cpp" // lalr1.cc:614
        break;

      case 34: // TFLOAT_LITERAL

#line 76 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 379 "parser.cpp" // lalr1.cc:614
        break;

      case 35: // TDOUBLE_LITERAL

#line 76 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 386 "parser.cpp" // lalr1.cc:614
        break;

      case 36: // TINTEGER_LITERAL

#line 76 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 393 "parser.cpp" // lalr1.cc:614
        break;

      case 37: // TSTRING_LITERAL

#line 76 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 400 "parser.cpp" // lalr1.cc:614
        break;

      case 42: // alias_declaration

#line 78 "parser.y" // lalr1.cc:614
        { delete (yysym.value.type); }
#line 407 "parser.cpp" // lalr1.cc:614
        break;

      case 43: // struct_declaration

#line 78 "parser.y" // lalr1.cc:614
        { delete (yysym.value.type); }
#line 414 "parser.cpp" // lalr1.cc:614
        break;

      case 48: // namespace

#line 79 "parser.y" // lalr1.cc:614
        { delete (yysym.value.namespace); }
#line 421 "parser.cpp" // lalr1.cc:614
        break;

      case 50: // member_declaration

#line 80 "parser.y" // lalr1.cc:614
        { delete (yysym.value.member_def); }
#line 428 "parser.cpp" // lalr1.cc:614
        break;

      case 53: // attribute_list

#line 82 "parser.y" // lalr1.cc:614
        { delete (yysym.value.attribute_list); }
#line 435 "parser.cpp" // lalr1.cc:614
        break;

      case 55: // attribute

#line 81 "parser.y" // lalr1.cc:614
        { delete (yysym.value.attribute); }
#line 442 "parser.cpp" // lalr1.cc:614
        break;

      case 56: // expression

#line 84 "parser.y" // lalr1.cc:614
        { delete (yysym.value.expression); }
#line 449 "parser.cpp" // lalr1.cc:614
        break;

      case 57: // initializer_list

#line 85 "parser.y" // lalr1.cc:614
        { delete (yysym.value.initializer_list); }
#line 456 "parser.cpp" // lalr1.cc:614
        break;

      case 59: // type

#line 77 "parser.y" // lalr1.cc:614
        { delete (yysym.value.type); }
#line 463 "parser.cpp" // lalr1.cc:614
        break;

      case 63: // literal

#line 83 "parser.y" // lalr1.cc:614
        { delete (yysym.value.literal); }
#line 470 "parser.cpp" // lalr1.cc:614
        break;


      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
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
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
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

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


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
            yyla.type = yytranslate_ (yylex (&yyla.value));
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
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 7:
#line 100 "parser.y" // lalr1.cc:859
    {
            type_system->import((yystack_[0].value.string))
        }
#line 702 "parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 106 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Alias((yystack_[1].value.type));
                    }
#line 710 "parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 110 "parser.y" // lalr1.cc:859
    {
                        ts::Alias* alias = new ts::Alias((yystack_[3].value.type));
                        for (auto const& att: (yystack_[1].value.attribute_list))
                        {
                            (yylhs.value.type)->add_attribute(std::move(att));
                        }
                        (yylhs.value.type) = alias;
                    }
#line 723 "parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 121 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 731 "parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 125 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 739 "parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 129 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 747 "parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 133 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 755 "parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 157 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.namespace) = new ts::Namespace();
            }
#line 763 "parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 161 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.namespace) = new ts::Namespace();
            }
#line 771 "parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 170 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.member_def) = new ts::Member_Def((yystack_[2].value.type), (yystack_[1].value.string));
                }
#line 779 "parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 174 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.member_def) = new ts::Member_Def((yystack_[4].value.type), (yystack_[3].value.string), (yystack_[1].value.expression));
                }
#line 787 "parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 188 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.attribute_list) = new std::vector<std::unique_ptr<ts::IAttribute>>();
                }
#line 795 "parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 198 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::Min_Attribute((yystack_[0].value.expression)->Evaluate());
            }
#line 803 "parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 202 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::Max_Attribute((yystack_[0].value.expression)->Evaluate());
            }
#line 811 "parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 206 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::Decimals_Attribute((yystack_[0].value.string));
            }
#line 819 "parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 210 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::UI_Name_Attribute((yystack_[0].value.string));
            }
#line 827 "parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 216 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.expression) = new ts::Literal_Expression((yystack_[0].value.literal));
            }
#line 835 "parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 220 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.expression) = new ts::Initializer_List_Expression((yystack_[0].value.initializer_list));
            }
#line 843 "parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 224 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.expression) = (yystack_[1].value.expression);
            }
#line 851 "parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 230 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.initializer_list) = new ts::Initializer_List();
                    }
#line 859 "parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 234 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.initializer_list) = new ts::Initializer_List();
                    }
#line 867 "parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 245 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.type) = type_system.find_type_symbol_by_name((yystack_[0].value.string)).get();
        }
#line 875 "parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 264 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 883 "parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 268 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 891 "parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 272 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 899 "parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 276 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 907 "parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 280 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.token));
        }
#line 915 "parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 284 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.token));
        }
#line 923 "parser.cpp" // lalr1.cc:859
    break;


#line 927 "parser.cpp" // lalr1.cc:859
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
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


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

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


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
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const signed char parser::yypact_ninf_ = -72;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      15,   -10,     8,     8,     8,    33,    48,   -72,   -72,   -72,
      56,   -72,   -72,   -72,   -72,    39,     0,    43,   -72,   -72,
     -72,     8,     3,    71,     7,    51,    -5,   -72,    53,   -72,
     -72,    61,   -72,     8,   -72,   -72,   -72,    63,     8,   -72,
      64,   -72,    37,    62,   -72,   -72,    58,     4,     5,   -72,
     -72,   -72,   -72,   -72,   -72,    44,   -72,    49,    59,   -72,
      32,   -72,    60,    68,   -72,    42,    73,    74,    75,    76,
      21,   -72,   -72,   -72,   -72,    32,    25,   -72,   -72,   -72,
     -72,    66,   -72,   -72,   -72,    67,   -72,    32,    32,    65,
      69,   -72,    49,    77,   -72,   -72,   -17,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,    32,   -72
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     5,    29,    28,
       4,    21,     3,     7,    30,     0,     0,     0,     1,     6,
      22,     0,     0,     0,     0,    46,     0,    45,     0,    15,
      14,     0,    16,     0,    18,    19,    20,     0,     0,    23,
       0,    25,     0,     0,     8,    10,     0,     0,     0,    17,
      24,    47,    53,    52,    51,     0,    49,     0,     0,    11,
       0,    26,     0,     0,    48,     0,     0,     0,     0,     0,
       0,    32,     9,    58,    59,     0,     0,    54,    55,    56,
      57,     0,    39,    38,    12,     0,    50,     0,     0,     0,
       0,    31,     0,     0,    41,    43,     0,    27,    13,    34,
      35,    36,    37,    33,    40,    42,     0,    44
  };

  const signed char
  parser::yypgoto_[] =
  {
     -72,   -72,   -72,    94,    -9,    -8,    54,   -72,   -72,   -72,
      91,   -72,   -72,    79,    -1,   -72,   -72,    12,   -71,   -72,
     -72,   -21,   -72,   -72,    40,   -72
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    31,    37,    38,    10,
      11,    40,    32,    12,    25,    58,    70,    71,    81,    82,
      96,    33,    27,    55,    56,    83
  };

  const unsigned char
  parser::yytable_[] =
  {
      26,    15,    16,    17,    93,    95,   105,     2,     3,     2,
       3,     2,     3,    29,    30,   106,    99,   100,     1,     2,
       3,    54,    22,    60,    43,    44,    28,    13,    62,    23,
      39,     4,    47,    18,    61,   107,    14,    49,    14,    29,
      30,    14,    73,    74,    54,    75,    91,    76,    94,    73,
      74,     1,    75,    92,    76,    66,    67,    68,    21,    77,
      78,    79,    80,    69,    51,    24,    77,    78,    79,    80,
      14,    64,     4,    52,    53,    14,    65,    42,    52,    53,
      34,    35,    36,    45,    46,    48,    57,    50,    59,    72,
      84,    85,    87,    88,    89,    90,    97,    98,   104,   101,
      19,    20,    63,    41,   103,    86,   102
  };

  const unsigned char
  parser::yycheck_[] =
  {
      21,     2,     3,     4,    75,    76,    23,     4,     5,     4,
       5,     4,     5,    22,    22,    32,    87,    88,     3,     4,
       5,    42,    22,    19,    29,    30,    23,    37,    23,    29,
      23,    16,    33,     0,    30,   106,    33,    38,    33,    48,
      48,    33,    17,    18,    65,    20,    25,    22,    23,    17,
      18,     3,    20,    32,    22,     6,     7,     8,    19,    34,
      35,    36,    37,    14,    27,    22,    34,    35,    36,    37,
      33,    27,    16,    36,    37,    33,    32,    26,    36,    37,
       9,    10,    11,    30,    23,    22,    24,    23,    30,    30,
      30,    23,    19,    19,    19,    19,    30,    30,    21,    34,
       6,    10,    48,    24,    92,    65,    37
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,    16,    39,    40,    41,    42,    43,
      47,    48,    51,    37,    33,    52,    52,    52,     0,    41,
      48,    19,    22,    29,    22,    52,    59,    60,    23,    42,
      43,    44,    50,    59,     9,    10,    11,    45,    46,    23,
      49,    51,    26,    29,    30,    30,    23,    52,    22,    52,
      23,    27,    36,    37,    59,    61,    62,    24,    53,    30,
      19,    30,    23,    44,    27,    32,     6,     7,     8,    14,
      54,    55,    30,    17,    18,    20,    22,    34,    35,    36,
      37,    56,    57,    63,    30,    23,    62,    19,    19,    19,
      19,    25,    32,    56,    23,    56,    58,    30,    30,    56,
      56,    34,    37,    55,    21,    23,    32,    56
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    38,    39,    39,    39,    40,    40,    41,    42,    42,
      43,    43,    43,    43,    44,    44,    44,    45,    46,    46,
      46,    47,    47,    48,    48,    49,    50,    50,    51,    51,
      52,    53,    54,    54,    55,    55,    55,    55,    56,    56,
      56,    57,    57,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    62,    62,    63,    63,    63,    63,    63,    63
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     2,     2,     5,     7,
       5,     6,     7,     8,     1,     1,     1,     2,     1,     1,
       1,     1,     2,     4,     5,     1,     3,     5,     1,     1,
       1,     3,     1,     3,     3,     3,     3,     3,     1,     1,
       3,     2,     3,     1,     3,     1,     1,     3,     4,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "TIMPORT", "TALIAS", "TSTRUCT", "TMIN",
  "TMAX", "TDECIMALS", "TPUBLIC", "TPRIVATE", "TPROTECTED", "TCONST",
  "TUI_CONTROL", "TUI_NAME", "TUI_CONST", "TNAMESPACE", "TFALSE", "TTRUE",
  "TEQUAL", "TLPARENTHESIS", "TRPARENTHESIS", "TLBRACE", "TRBRACE",
  "TLBRAKET", "TRBRAKET", "TLANGLED_BRAKET", "TRANGLED_BRAKET",
  "TNAMESPACE_SEPARATOR", "TCOLON", "TSEMICOLON", "TDOT", "TCOMMA",
  "TIDENTIFIER", "TFLOAT_LITERAL", "TDOUBLE_LITERAL", "TINTEGER_LITERAL",
  "TSTRING_LITERAL", "$accept", "program", "import_list", "import",
  "alias_declaration", "struct_declaration", "struct_body", "inheritance",
  "inheritance_type", "namespace_list", "namespace", "namespace_body",
  "member_declaration", "declaration_list", "identifier", "attribute_list",
  "attribute_body", "attribute", "expression", "initializer_list",
  "initializer_body", "type", "templated_type", "template_argument_list",
  "template_argument", "literal", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,    91,    91,    92,    93,    96,    97,    99,   105,   109,
     120,   124,   128,   132,   138,   139,   140,   144,   147,   148,
     149,   152,   153,   156,   160,   166,   169,   173,   179,   180,
     183,   187,   193,   194,   197,   201,   205,   209,   215,   219,
     223,   229,   233,   239,   240,   243,   244,   250,   251,   254,
     255,   258,   259,   260,   263,   267,   271,   275,   279,   283
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
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
  parser::yy_reduce_print_ (int yyrule)
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

  // Symbol number corresponding to token number t.
  inline
  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
    };
    const unsigned int user_token_number_max_ = 292;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 1416 "parser.cpp" // lalr1.cc:1167
#line 290 "parser.y" // lalr1.cc:1168


