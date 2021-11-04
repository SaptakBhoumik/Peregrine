#ifndef PEREGRINE_AST_AST_HPP
#define PEREGRINE_AST_AST_HPP
#include "../lexer/tokens.hpp"
#include <vector>
struct ast_node;
typedef enum ast_kind {
  AST_NUMBER,
  AST_STRING,
  AST_VARIABLE,
  AST_BOOL,
  AST_LIST,
  AST_DICT,
  AST_CPP,
  AST_NONE,
  AST_OPERATOR,
  AST_INFIX,
  AST_VAR
} ast_kind;
struct binary_op {
  ast_node *left;
  ast_node *right;
};
struct infix_op {
  ast_node *child;
};
struct STRING {
  bool is_formatted;
  bool is_raw;
};
typedef union child {
  binary_op operator_op;
  infix_op infix;
  STRING string;
} child;
struct ast_node {
  ast_kind kind;
  Token token;
  child children;
};
struct AST {
  std::vector<ast_node> root;
};
#endif