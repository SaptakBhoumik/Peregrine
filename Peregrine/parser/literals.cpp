#include "../ast/ast.hpp"
#include "parser.hpp"
/// numberexpr ::= number
std::unique_ptr<Ast> Parser::ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(current_token);
  advance(); // consume the number
  return std::move(Result);
}
/// stringexpr ::= string
std::unique_ptr<Ast> Parser::ParseStringExpr() {
  auto Result = std::make_unique<StringExprAST>(current_token);
  advance(); // consume the string
  return std::move(Result);
}