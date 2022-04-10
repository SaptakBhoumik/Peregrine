#ifndef PEREGRINE_TOKENS_HPP
#define PEREGRINE_TOKENS_HPP

#include <string>

// Create a string variable
enum TokenType {
    tk_eof, // end of file
    // Some operators
    tk_dollar,     // $
    tk_pipeline,   // |>
    tk_plus,       // +
    tk_at,         // @
    tk_minus,      // -
    tk_divide,     // /
    tk_multiply,   // *
    tk_xor,        // ^
    tk_modulo,     // %
    tk_greater,    // >
    tk_less,       // <
    tk_ampersand,  // &
    tk_bit_or,     // |
    tk_bit_not,    // ~
    tk_assign,     // =
    tk_colon,      // :
    tk_dot,        // .
    tk_double_dot, // ..
    tk_ellipses,   //...
    tk_l_paren,    // (
    tk_r_paren,    // )
    tk_comma,      // ,
    tk_list_close, // ]
    tk_list_open,  // [
    tk_dict_close, // }
    tk_dict_open,  // {
    tk_new_line,   // \n

    tk_exponent,          // **
    tk_equal,             // ==
    tk_not_equal,         // !=
    tk_floor,             // //
    tk_gr_or_equ,         // >=
    tk_less_or_equ,       // <=
    tk_increment,         // ++
    tk_decrement,         // --
    tk_arrow,             // ->
    tk_range,             // ..
    tk_shift_right,       // >>
    tk_shift_left,        // <<
    tk_floor_equal,       // //=
    tk_plus_equal,        // +=
    tk_minus_equal,       // -=
    tk_exponent_equal,    // **=
    tk_times_equal,       // *=
    tk_slash_equal,       // /=
    tk_mod_equal,         // %=
    tk_shift_right_equal, // >>=
    tk_shift_left_equal,  // <<=
    tk_bit_and_equal,     // &=
    tk_bit_or_equal,      // |=
    tk_bit_xor_equal,     // ^=

    tk_identifier, // foo, bar

    // keywords tokens
    tk_asm,      // __asm__
    tk_private,   // private
    tk_static,    // static
    tk_scope,     // scope
    tk_union,     // union
    tk_cast,     // cast
    tk_assert,    // assert
    tk_try,       // try
    tk_except,    // except
    tk_raise,     // raise
    tk_true,      // True
    tk_false,     // False
    tk_with,      // with
    tk_as,        // as
    tk_none,      // None
    tk_const,     // const
    tk_import,    // import
    tk_from,      // from
    tk_inline, // inline
    tk_virtual,   // virtual
    tk_if,        // if
    tk_else,      // else
    tk_elif,      // elif
    tk_while,     // while
    tk_for,       // for
    tk_break,     // break
    tk_continue,  // continue
    tk_match,     // match
    tk_extern,    // extern
    tk_case,      // case
    tk_default,   // default
    tk_def,       // def
    tk_pass,      // pass
    tk_return,    // return
    tk_and,       // and
    tk_or,        // or
    tk_not,       // not
    tk_is,        // is
    tk_in,        // in
    tk_not_in,    // not in
    tk_is_not,    // is not
    tk_class,     // class
    tk_type,      // type defination
    tk_enum,      // enum
    tk_export,    // export

    // value type
    tk_decimal,
    tk_integer,
    tk_string,
    tk_cpp,

    tk_ident,  // beginning of identation
    tk_dedent, // end of identation

    tk_raw,    // raw string i.e. r
    tk_format, // formatted string i.e. f
};

struct Token {
    size_t location;
    std::string statement;
    std::string keyword;
    size_t start;
    size_t end;
    size_t line;
    TokenType tkType;
};

namespace TokenUtils {

// +, -, *, etc
bool isArithmeticToken(Token tok);

// ==, <=, etc
bool isComparisonToken(Token tok);

// |, &, ^, etc
bool isBitwiseToken(Token tok);
} // namespace TokenUtils

#endif