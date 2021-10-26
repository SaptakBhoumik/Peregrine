#ifndef PEREGRINE_TOKENS
#define PEREGRINE_TOKENS
#include <string>

// Create a string variable
typedef enum {
  // Some operators
  tk_plus,      // +    //done
  tk_minus,     // -    //done
  tk_divide,    // /    //done
  tk_asterisk,  // *    //done
  tk_multiply,  // *  //done
  tk_xor,       // ^        //done
  tk_modulo,    // %     //done
  tk_greater,   // >     //done
  tk_less,      // <     //done
  tk_ampersand, // &     //done
  tk_bit_or,    // |     //done
  tk_bit_not,   // ~     //done
  tk_assign,    // =     //done
  tk_excl,      // !     //done
  tk_colon,     // :     //done
  tk_dot,       // .     //done
  tk_l_paren,   // (     //done
  tk_r_paren,   // )     //done
  tk_hashtag,   // //     //done
  tk_comma,     // ,     //done

  tk_exponent,          //**      //done
  tk_equal,             // ==   //done
  tk_not_equal,         // !=   //done
  tk_floor,             // //   //done
  tk_gr_or_equ,         // >=   //done
  tk_less_or_equ,       // <=   //done
  tk_increment,         // ++   //done
  tk_decrement,         // --   //done
  tk_arrow,             // ->   //done
  tk_shift_right,       // >>   //done
  tk_shift_left,        // <<   //done
  tk_floor_equal,       // //=   //done
  tk_plus_equal,        // +=   //done
  tk_minus_equal,       // -=   //done
  tk_times_equal,       // *=   //done
  tk_slash_equal,       // /=   //done
  tk_mod_equal,         // %=   //done
  tk_shift_right_equal, // >>=   //done
  tk_shift_left_equal,  // <<=   //done
  tk_bit_and_equal,     // &=   //done
  tk_bit_or_equal,      // |=   //done
  tk_bit_xor_equal,     // ^=   //done

  tk_identifier, // foo, bar   //done

  // keywords tokens
  tk_scope,  // scope //done
  tk_enum,   // enum  //done
  tk_any,    // any //done
  tk_assert, // assert 
  tk_try,    // try 
  tk_except, // except
  tk_raise,  // raise
  tk_true,   // True  //done
  tk_false,  // False //done
  tk_with,   // with
  tk_as,     // as
  // This is not necessary because peregrine has special library for
  // multithreading
  // tk_async,     // async
  // tk_await,     // await
  tk_flags,     // flags
  tk_none,      // None
  tk_const,     // const
  tk_import,    // import
  tk_cppimport, // cppimport
  tk_himport,   // himport
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
  tk_def,       // def  //done
  tk_pass,      // pass
  tk_return,    // return //done
  tk_and,       // and //done
  tk_or,        // or //done
  tk_not,       // not
  tk_is,        // is
  tk_in,        // in
  tk_cppcode,   // Cppcode
  tk_class,     // class //done
  tk_struct,    // struct
  tk_dict,      // dict
  tk_type,      // type defination //done

  // value type
  tk_decimal,//done
  tk_integer,//done
  tk_string,//done
  cpp,//done
  array,//done
  dictionary,//done

  tk_ident,  // beginning of identation
  tk_dedent, // end of identation

  tk_raw,    // raw string i.e. r //done
  tk_format, // formatted string i.e. f //done
  tk_eof//end of file //done
} TokenType;

typedef struct {
  std::string statement;
  std::string keyword;
  unsigned long int start;
  unsigned long int end;
  unsigned long int line;
  TokenType tk_type;
} Token;
#endif