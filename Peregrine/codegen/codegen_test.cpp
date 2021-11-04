#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "codegen.hpp"
#include <iostream>
int main() {
  LEXEME test = lexer("a+b", "file name");
  Parser parse_obj;
  parse_obj.tokens = test;
  auto x = parse_obj.parse();
  CodeGen cpp;
  cpp.ast=x;
  std::cout << cpp.codegen() << std::endl;
  return 0;
}