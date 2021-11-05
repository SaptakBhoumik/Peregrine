#include <iostream>
#include <vector>
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"
int main() {
    std::vector<Token> res = lexer("a+b-8", "");

    for (auto& tok : res) {
        std::cout << "Statement: " << tok.statement << " | Keyword: " << tok.keyword << " | Type: " << tok.tk_type << "\n";
    }
  Parser parse_obj;
  parse_obj.tokens = res;
  auto x = parse_obj.parse();
  std::cout << parse_obj.sexp << std::endl;
  std::cout << x.root[0].token.keyword << std::endl;
  std::cout << x.root[0].children.operator_op.left->token.keyword << std::endl;
  std::cout << x.root[0].children.operator_op.right->token.keyword << std::endl;
  std::cout << x.root[0].children.operator_op.right->children.operator_op.left->token.keyword << std::endl;
  std::cout << x.root[0].children.operator_op.right->children.operator_op.right->token.keyword << std::endl;
  return 0;
}