#include "../ast/ast.hpp"
#include "parser.hpp"
/// numberexpr ::= number
ast_node Parser::ParseNumberExpr() {
  ast_node Result;
  Result.kind = AST_NUMBER;
  Result.token = current_token;

  return Result;
}
/// stringexpr ::= string
ast_node Parser::ParseStringExpr(bool is_formatted, bool is_raw) {
  ast_node Result;
  Result.kind = AST_STRING;
  Result.token = current_token;
  Result.children.string.is_formatted = is_formatted;
  Result.children.string.is_raw = is_raw;
  return Result;
}

/// true_false_expr ::= true or false
ast_node Parser::ParseBoolExpr() {
  ast_node Result;
  Result.kind = AST_BOOL;
  Result.token = current_token;

  return Result;
}

/// ListExprAST ::= list
ast_node Parser::ParseListExpr() {
  ast_node Result;
  Result.kind = AST_LIST;
  Result.token = current_token;

  return Result;
}

/// DictExprAST ::= dictionary
ast_node Parser::ParseDictExpr() {
  ast_node Result;
  Result.kind = AST_DICT;
  Result.token = current_token;

  return Result;
}

/// CppExprAST ::= C++ code
ast_node Parser::ParseCppExpr() {
  ast_node Result;
  Result.kind = AST_CPP;
  Result.token = current_token;

  return Result;
}
/// NoneExprAST ::= None
ast_node Parser::ParseNoneExpr() {
  ast_node Result;
  Result.kind = AST_NONE;
  Result.token = current_token;

  return Result;
}
