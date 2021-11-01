#include "../lexer/lexer.hpp"
#include "parser.hpp"
#include <iostream>
int main() {
  LEXEME test = lexer("a+b*8", "file name");
  Parser parse_obj;
  parse_obj.tokens = test;
  auto x = parse_obj.parse();
  std::cout << parse_obj.sexp << std::endl;
  return 0;
}