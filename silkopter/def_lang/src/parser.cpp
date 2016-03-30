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
            case 80: // TIDENTIFIER

#line 122 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 372 "parser.cpp" // lalr1.cc:614
        break;

      case 81: // TFLOAT_LITERAL

#line 122 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 379 "parser.cpp" // lalr1.cc:614
        break;

      case 82: // TDOUBLE_LITERAL

#line 122 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 386 "parser.cpp" // lalr1.cc:614
        break;

      case 83: // TINTEGER_LITERAL

#line 122 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 393 "parser.cpp" // lalr1.cc:614
        break;

      case 84: // TSTRING_LITERAL

#line 122 "parser.y" // lalr1.cc:614
        { delete (yysym.value.string); }
#line 400 "parser.cpp" // lalr1.cc:614
        break;

      case 89: // alias_declaration

#line 124 "parser.y" // lalr1.cc:614
        { delete (yysym.value.type); }
#line 407 "parser.cpp" // lalr1.cc:614
        break;

      case 90: // struct_declaration

#line 124 "parser.y" // lalr1.cc:614
        { delete (yysym.value.type); }
#line 414 "parser.cpp" // lalr1.cc:614
        break;

      case 95: // namespace

#line 125 "parser.y" // lalr1.cc:614
        { delete (yysym.value.namespace); }
#line 421 "parser.cpp" // lalr1.cc:614
        break;

      case 97: // member_declaration

#line 126 "parser.y" // lalr1.cc:614
        { delete (yysym.value.member_def); }
#line 428 "parser.cpp" // lalr1.cc:614
        break;

      case 100: // attribute_list

#line 128 "parser.y" // lalr1.cc:614
        { delete (yysym.value.attribute_list); }
#line 435 "parser.cpp" // lalr1.cc:614
        break;

      case 102: // attribute

#line 127 "parser.y" // lalr1.cc:614
        { delete (yysym.value.attribute); }
#line 442 "parser.cpp" // lalr1.cc:614
        break;

      case 103: // expression

#line 130 "parser.y" // lalr1.cc:614
        { delete (yysym.value.expression); }
#line 449 "parser.cpp" // lalr1.cc:614
        break;

      case 104: // initializer_list

#line 131 "parser.y" // lalr1.cc:614
        { delete (yysym.value.initializer_list); }
#line 456 "parser.cpp" // lalr1.cc:614
        break;

      case 107: // builtin_type

#line 123 "parser.y" // lalr1.cc:614
        { delete (yysym.value.type); }
#line 463 "parser.cpp" // lalr1.cc:614
        break;

      case 112: // literal

#line 129 "parser.y" // lalr1.cc:614
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
#line 146 "parser.y" // lalr1.cc:859
    {
            type_system->import((yystack_[0].value.string))
        }
#line 702 "parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 152 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Alias();
                    }
#line 710 "parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 156 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Alias();
                    }
#line 718 "parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 162 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 726 "parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 166 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 734 "parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 170 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 742 "parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 174 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.type) = new ts::Struct_Type();
                    }
#line 750 "parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 198 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.namespace) = new ts::Namespace();
            }
#line 758 "parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 202 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.namespace) = new ts::Namespace();
            }
#line 766 "parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 211 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.member_def) = new ts::Member_Def();
                }
#line 774 "parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 215 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.member_def) = new ts::Member_Def();
                }
#line 782 "parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 219 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.member_def) = new ts::Member_Def();
                }
#line 790 "parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 223 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.member_def) = new ts::Member_Def();
                }
#line 798 "parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 237 "parser.y" // lalr1.cc:859
    {
                    (yylhs.value.attribute_list) = new std::vector<std::unique_ptr<ts::IAttribute>>();
                }
#line 806 "parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 247 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::Min_Attribute((yystack_[0].value.expression)->Evaluate());
            }
#line 814 "parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 251 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::Max_Attribute((yystack_[0].value.expression)->Evaluate());
            }
#line 822 "parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 255 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::Decimals_Attribute((yystack_[0].value.string));
            }
#line 830 "parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 259 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.attribute) = new ts::UI_Name_Attribute((yystack_[0].value.string));
            }
#line 838 "parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 265 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.expression) = new ts::Literal_Expression((yystack_[0].value.literal));
            }
#line 846 "parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 269 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.expression) = new ts::Initializer_List_Expression((yystack_[0].value.initializer_list));
            }
#line 854 "parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 273 "parser.y" // lalr1.cc:859
    {
                (yylhs.value.expression) = (yystack_[1].value.expression);
            }
#line 862 "parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 279 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.initializer_list) = new ts::Initializer_List();
                    }
#line 870 "parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 283 "parser.y" // lalr1.cc:859
    {
                        (yylhs.value.initializer_list) = new ts::Initializer_List();
                    }
#line 878 "parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 296 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = nullptr; }
#line 884 "parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 297 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_bool_type().get(); }
#line 890 "parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 298 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_float_type().get(); }
#line 896 "parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 299 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_double_type().get(); }
#line 902 "parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 300 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_string_type().get(); }
#line 908 "parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 301 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_int8_type().get(); }
#line 914 "parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 302 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_uint8_type().get(); }
#line 920 "parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 303 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_int16_type().get(); }
#line 926 "parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 304 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_uint16_type().get(); }
#line 932 "parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 305 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_int32_type().get(); }
#line 938 "parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 306 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_uint32_type().get(); }
#line 944 "parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 307 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_int64_type().get(); }
#line 950 "parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 308 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_uint64_type().get(); }
#line 956 "parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 309 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2f_type().get(); }
#line 962 "parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 310 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2d_type().get(); }
#line 968 "parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 311 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2u8_type().get(); }
#line 974 "parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 312 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2s8_type().get(); }
#line 980 "parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 313 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2u16_type().get(); }
#line 986 "parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 314 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2s16_type().get(); }
#line 992 "parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 315 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2u32_type().get(); }
#line 998 "parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 316 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec2s32_type().get(); }
#line 1004 "parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 317 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3f_type().get(); }
#line 1010 "parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 318 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3d_type().get(); }
#line 1016 "parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 319 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3u8_type().get(); }
#line 1022 "parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 320 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3s8_type().get(); }
#line 1028 "parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 321 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3u16_type().get(); }
#line 1034 "parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 322 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3s16_type().get(); }
#line 1040 "parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 323 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3u32_type().get(); }
#line 1046 "parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 324 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec3s32_type().get(); }
#line 1052 "parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 325 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4f_type().get(); }
#line 1058 "parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 326 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4d_type().get(); }
#line 1064 "parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 327 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4u8_type().get(); }
#line 1070 "parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 328 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4s8_type().get(); }
#line 1076 "parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 329 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4u16_type().get(); }
#line 1082 "parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 330 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4s16_type().get(); }
#line 1088 "parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 331 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4u32_type().get(); }
#line 1094 "parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 332 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_vec4s32_type().get(); }
#line 1100 "parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 333 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_quatf_type().get(); }
#line 1106 "parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 334 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_quafd_type().get(); }
#line 1112 "parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 335 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_mat2f_type().get(); }
#line 1118 "parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 336 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_mat2d_type().get(); }
#line 1124 "parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 337 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_mat3f_type().get(); }
#line 1130 "parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 338 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_mat3d_type().get(); }
#line 1136 "parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 339 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_mat4f_type().get(); }
#line 1142 "parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 340 "parser.y" // lalr1.cc:859
    { (yylhs.value.type) = type_system->get_mat4d_type().get(); }
#line 1148 "parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 360 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 1156 "parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 364 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 1164 "parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 368 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 1172 "parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 372 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.string));
        }
#line 1180 "parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 376 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.token));
        }
#line 1188 "parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 380 "parser.y" // lalr1.cc:859
    {
            (yylhs.value.literal) = new ts::Literal((yystack_[0].value.token));
        }
#line 1196 "parser.cpp" // lalr1.cc:859
    break;


#line 1200 "parser.cpp" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -39;

  const signed char parser::yytable_ninf_ = -1;

  const short int
  parser::yypact_[] =
  {
      69,   -27,   -10,   -10,   -10,   133,   138,   -39,   -39,   -39,
      79,   -39,   -39,   -39,   -39,    78,    62,    76,   -39,   -39,
     -39,   310,   158,    10,    60,   -39,    73,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
     -24,   -39,    71,   -39,   -39,    80,   -39,   -39,   -10,   -39,
     -39,   -39,   -39,    86,   -10,   -39,    88,   -39,    -3,   -39,
      85,   -39,   -39,    72,   -18,   235,   -39,   -39,   -39,   -39,
     -39,   -39,   -25,   -39,     8,    82,   -39,   148,    85,   -39,
     132,   140,   -39,    77,    81,   145,   150,   152,    64,   -39,
     -39,   -39,   -39,   148,    70,   -39,   -39,   -39,   -39,     2,
     -39,   -39,   137,   -39,   142,   -39,   148,   148,   139,   141,
     -39,     8,   153,   -39,   -39,   -19,    85,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   148,   146,   -39,
     -39
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     5,    31,    30,
       4,    21,     3,     7,    32,     0,     0,     0,     1,     6,
      22,     0,     0,     0,     0,    53,     0,    51,    52,    50,
      55,    54,    57,    56,    59,    58,    61,    60,    62,    63,
      65,    64,    67,    66,    69,    68,    70,    71,    73,    72,
      75,    74,    77,    76,    78,    79,    81,    80,    83,    82,
      85,    84,    86,    87,    88,    89,    90,    91,    92,    93,
       0,    49,     0,    15,    14,     0,    16,    47,     0,    48,
      18,    19,    20,     0,     0,    23,     0,    25,     0,    94,
       0,     8,    10,     0,     0,     0,    17,    24,    95,   101,
     100,    99,     0,    97,     0,     0,    11,     0,     0,    26,
       0,     0,    96,     0,     0,     0,     0,     0,     0,    34,
       9,   106,   107,     0,     0,   102,   103,   104,   105,     0,
      41,    40,     0,    12,     0,    98,     0,     0,     0,     0,
      33,     0,     0,    43,    45,     0,     0,    28,    27,    13,
      36,    37,    38,    39,    35,    42,    44,     0,     0,    46,
      29
  };

  const short int
  parser::yypgoto_[] =
  {
     -39,   -39,   -39,   216,    33,    34,   129,   -39,   -39,   -39,
     217,   -39,   -39,   202,    -2,   253,   -39,    92,   239,   -39,
     -39,   -38,   213,   -39,   -39,   -39,   122,   -39
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    75,    83,    84,    10,
      11,    86,    76,    12,    77,   105,   118,   119,   129,   130,
     145,    78,    79,    71,    89,   102,   103,   131
  };

  const unsigned char
  parser::yytable_[] =
  {
      15,    16,    17,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,   107,   112,
     101,   156,    90,    91,   113,    73,    74,    13,   108,   109,
     157,   114,   115,   116,     2,     3,    80,    81,    82,   117,
      14,    98,     1,     2,     3,   101,    94,    14,   146,   147,
      99,   100,    96,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    73,    74,
      85,    22,     4,    18,   121,   122,   140,   123,    23,   124,
     143,     1,     4,   141,    21,    24,    88,   136,    92,   106,
      93,   125,   126,   127,   128,    95,   104,    14,    97,   120,
      99,   100,     2,     3,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,   133,
     134,   137,   121,   122,   148,   123,   138,   124,   139,   149,
     152,   155,    19,   160,   111,   153,    87,    20,    72,   125,
     126,   127,   128,   154,    70,   135,     0,     0,    14,     2,
       3,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,   132,   142,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   150,   151,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   159,     0,     0,   158
  };

  const short int
  parser::yycheck_[] =
  {
       2,     3,     4,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    66,    74,
      88,    70,    76,    77,    79,    22,    22,    84,    76,    77,
      79,    53,    54,    55,     4,     5,    56,    57,    58,    61,
      80,    74,     3,     4,     5,   113,    78,    80,    76,    77,
      83,    84,    84,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    95,    95,
      70,    69,    63,     0,    64,    65,    72,    67,    76,    69,
      70,     3,    63,    79,    66,    69,    73,    66,    77,    77,
      70,    81,    82,    83,    84,    69,    71,    80,    70,    77,
      83,    84,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    77,
      70,    66,    64,    65,    77,    67,    66,    69,    66,    77,
      81,    68,     6,    77,    95,    84,    24,    10,    70,    81,
      82,    83,    84,   141,    21,   113,    -1,    -1,    80,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   108,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   146
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,    63,    86,    87,    88,    89,    90,
      94,    95,    98,    84,    80,    99,    99,    99,     0,    88,
      95,    66,    69,    76,    69,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     107,   108,    70,    89,    90,    91,    97,    99,   106,   107,
      56,    57,    58,    92,    93,    70,    96,    98,    73,   109,
      76,    77,    77,    70,    99,    69,    99,    70,    74,    83,
      84,   106,   110,   111,    71,   100,    77,    66,    76,    77,
      70,    91,    74,    79,    53,    54,    55,    61,   101,   102,
      77,    64,    65,    67,    69,    81,    82,    83,    84,   103,
     104,   112,   100,    77,    70,   111,    66,    66,    66,    66,
      72,    79,   103,    70,   103,   105,    76,    77,    77,    77,
     103,   103,    81,    84,   102,    68,    70,    79,   100,   103,
      77
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    85,    86,    86,    86,    87,    87,    88,    89,    89,
      90,    90,    90,    90,    91,    91,    91,    92,    93,    93,
      93,    94,    94,    95,    95,    96,    97,    97,    97,    97,
      98,    98,    99,   100,   101,   101,   102,   102,   102,   102,
     103,   103,   103,   104,   104,   105,   105,   106,   106,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   108,   109,   109,   110,   110,   111,
     111,   111,   112,   112,   112,   112,   112,   112
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     2,     2,     5,     7,
       5,     6,     7,     8,     1,     1,     1,     2,     1,     1,
       1,     1,     2,     4,     5,     1,     3,     5,     5,     7,
       1,     1,     1,     3,     1,     3,     3,     3,     3,     3,
       1,     1,     3,     2,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "TIMPORT", "TALIAS", "TSTRUCT",
  "TSTRING", "TVECTOR", "TFLOAT", "TDOUBLE", "TBOOL", "TUINT8_T",
  "TINT8_T", "TUINT16_T", "TINT16_T", "TUINT32_T", "TINT32_T", "TUINT64_T",
  "TINT64_T", "TVEC2F", "TVEC2D", "TVEC2S8", "TVEC2U8", "TVEC2S16",
  "TVEC2U16", "TVEC2S32", "TVEC2U32", "TVEC3F", "TVEC3D", "TVEC3S8",
  "TVEC3U8", "TVEC3S16", "TVEC3U16", "TVEC3S32", "TVEC3U32", "TVEC4F",
  "TVEC4D", "TVEC4S8", "TVEC4U8", "TVEC4S16", "TVEC4U16", "TVEC4S32",
  "TVEC4U32", "TQUATF", "TQUATD", "TMAT2F", "TMAT2D", "TMAT3F", "TMAT3D",
  "TMAT4F", "TMAT4D", "TCOLOR_RGB", "TCOLOR_RGBA", "TMIN", "TMAX",
  "TDECIMALS", "TPUBLIC", "TPRIVATE", "TPROTECTED", "TCONST",
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
  "initializer_body", "type", "builtin_type", "builtin_templated_type",
  "template_argument_list", "template_argument_body", "template_argument",
  "literal", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   137,   137,   138,   139,   142,   143,   145,   151,   155,
     161,   165,   169,   173,   179,   180,   181,   185,   188,   189,
     190,   193,   194,   197,   201,   207,   210,   214,   218,   222,
     228,   229,   232,   236,   242,   243,   246,   250,   254,   258,
     264,   268,   272,   278,   282,   288,   289,   292,   293,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   343,   346,   347,   350,   351,   354,
     355,   356,   359,   363,   367,   371,   375,   379
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
    };
    const unsigned int user_token_number_max_ = 339;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 1793 "parser.cpp" // lalr1.cc:1167
#line 386 "parser.y" // lalr1.cc:1168


