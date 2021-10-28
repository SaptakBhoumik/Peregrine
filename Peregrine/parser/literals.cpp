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

/// true_false_expr ::= true or false
std::unique_ptr<Ast> Parser::ParseBoolExpr() {
  auto Result = std::make_unique<true_false_AST>(current_token);
  advance(); // consume the bool
  return std::move(Result);
}

/// ListExprAST ::= list
std::unique_ptr<Ast> Parser::ParseListExpr() {
  auto Result = std::make_unique<ListExprAST>(current_token);
  advance(); // consume the list
  return std::move(Result);
}
/// DictExprAST ::= dictionary 
std::unique_ptr<Ast> Parser::ParseDictExpr() {
  auto Result = std::make_unique<DictExprAST>(current_token);
  advance(); // consume the dictionary
  return std::move(Result);
}
/// CppExprAST ::= C++ code
std::unique_ptr<Ast> Parser::ParseCppExpr() {
  auto Result = std::make_unique<CppExprAST>(current_token);
  advance(); // consume the C++ code
  return std::move(Result);
}