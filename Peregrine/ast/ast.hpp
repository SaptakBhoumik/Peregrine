
#ifndef PEREGRINE_AST
#define PEREGRINE_AST

typedef enum {
  ast_function_define,
  ast_function_call,
  ast_if,
  ast_elif,
  ast_else,
  ast_int,
  ast_int32,
  ast_int16,
  ast_int8,
  ast_uint,
  ast_uint32,
  ast_uint16,
  ast_uint8,
  ast_float,
  ast_float32,
  ast_str,
  ast_char,

}AST_TYPE;

#endif