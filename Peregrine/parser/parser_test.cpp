#include "../lexer/lexer.hpp"
#include "parser.hpp"
#include <iostream>
int main() {
  LEXEME test = lexer("a+b", "file name");
  Parser parse_obj;
  parse_obj.tokens = test;
  auto x = parse_obj.parse();
  std::cout << parse_obj.sexp << std::endl;
  std::cout << x.root[0].token.keyword << std::endl;
  std::cout << x.root[0].children.operator_op.right->token.keyword << std::endl;
  std::cout << x.root[0].children.operator_op.left->token.keyword << std::endl;
  return 0;
}