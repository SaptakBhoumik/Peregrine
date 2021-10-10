typedef enum {
  tk_plus,        // +
  tk_minus,       // -
  tk_divide,      // /
  tk_asterisk,    // *
  tk_hat,         // ^
  tk_modulo,      // %
  tk_greater,     // >
  tk_less,        // <
  tk_bit_and,     // &
  tk_bit_or,      // |
  tk_bit_not,     // ~
  tk_assign,      // =
  tk_excl,        // !
  tk_colon,       // :
  tk_dot,         // .
  tk_l_bracket,   // {
  tk_r_bracket,   // }
  tk_lsq_bracket, // [
  tk_rsq_bracket, // ]
  tk_l_paren,     // (
  tk_r_paren,     // )
  tk_hashtag,     // #
  tk_comma,       // ,
  tk_sin_quote,   // '
  tk_db_quote,    // "

  tk_equal,             // ==
  tk_not_equal,         // !=
  tk_db_slash,          // //
  tk_gr_or_equ,         // >=
  tk_less_or_equ,       // <=
  tk_increment,         // ++
  tk_decrement,         // --
  tk_arrow,             // ->
  tk_shift_right,       // >>
  tk_shift_left,        // <<
  tk_plus_equal,        // +=
  tk_minus_equal,       // -=
  tk_times_equal,       // *=
  tk_slash_equal,       // /=
  tk_mod_equal,         // %=
  tk_shift_right_equal, // >>=
  tk_shift_left_equal,  // <<=
  tk_bit_and_equal,     // &=
  tk_bit_or_equal,      // |=
  tk_bit_xor_equal,     // ^=

  tk_identifier, // foo, bar
  tk_integer,    // 123, 65

  // keywords tokens
  tk_true,     // true
  tk_false,    // false
  tk_none,     // none
  tk_const,    // const
  tk_import,   // import
  tk_if,       // if
  tk_else,     // else
  tk_elif,     // elif
  tk_while,    // while
  tk_for,      // for
  tk_break,    // break
  tk_continue, // continue
  tk_match,    // match
  tk_def,      // def
  tk_pass,     // pass
  tk_return,   // return
  tk_and,      // and
  tk_or,       // or
  tk_not,      // not
  tk_is,       // is
  tk_in,       // in
  tk_try,      // try
  tk_catch,    // catch
  tk_raise,    // raise
  tk_finally,  // finally

  tk_ident,  // beginning of identation
  tk_dedent, // end of identation
} TokenType;