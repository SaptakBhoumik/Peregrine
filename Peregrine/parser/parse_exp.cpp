#include "parser.hpp"
#include "../ast/ast.hpp"
#include <memory>
#include <string>

std::unique_ptr<Ast> Parser::ParseParenExpr() {
  advance(); // eat (.
  auto V = ParseExpression();
  if (!V){
    return nullptr;
  }

  if (current_token.tk_type != tk_r_paren){
    //raise error
  }
  advance(); // eat ).
  return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<Ast> Parser::ParseIdentifierExpr() {

  advance();  // eat identifier.

  if (current_token.keyword != std::string(1,'(')){// Simple variable ref.
  //todo- do some type inference here to figure out what type the variable is. also show error if undefined
    // return std::make_unique<VariableExprAST>(current_token);
  }
  // Call.
  advance();  // eat (
  std::vector<std::unique_ptr<Ast>> Args;
  if (current_token.keyword != std::string(1,')')) {
    while (1) {
      if (auto Arg = ParseExpression()){
        Args.push_back(std::move(Arg));
      }
      else{
        return nullptr;
      }
      if (current_token.keyword == std::string(1,')')){
        break;
      }
      //to do: check for ','
      if (current_token.keyword != std::string(1,',')){
        return nullptr;//raise error
      }
      advance();
    }
  }

  // Eat the ')'.
  advance();

  return std::make_unique<CallExprAST>(current_token, std::move(Args));
}
