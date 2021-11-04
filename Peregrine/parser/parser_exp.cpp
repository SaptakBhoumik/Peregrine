#include "parser.hpp"
ast_node Parser::parseExpression(Precedence_type curr_precedence) {
  ast_node left;
  sexp += current_token.keyword;
  if (current_token.tk_type == tk_integer) {
    left = ParseNumberExpr();
  } else if (current_token.tk_type == tk_decimal) {
    left = ParseNumberExpr();
  } else if (current_token.tk_type == tk_none) {
    left = ParseNoneExpr();
  } else if (current_token.tk_type == tk_string) {
    left = ParseStringExpr(false, false);
  } else if (current_token.tk_type == tk_identifier) {
    left = ParseIdentifierExpr();
  } else if (current_token.tk_type == tk_l_paren) {
    left = ParseGroupExpr();
  } else if (current_token.tk_type == tk_false ||
             current_token.tk_type == tk_true) {
    left = ParseBoolExpr();
  } else if (current_token.tk_type == tk_negative ||
             current_token.tk_type == tk_not||
             current_token.tk_type == tk_bit_not) {
    left = parsePrefixExpression();
  }
  auto temp = next();
    while (next_precedence() > curr_precedence) {
      advance();
      left = parseBinaryOperation(left);
    }
  return left;
}

ast_node Parser::parseBinaryOperation(ast_node left) {
  ast_node res;
  auto curr_operator = current_token;
  res.token = curr_operator;
  res.kind = AST_OPERATOR;
  res.children.operator_op.left = &left;
  sexp += curr_operator.keyword;
  advance();
  auto currentPrecedence = precidence_map[tk_type];
  auto right = parseExpression(currentPrecedence);
  res.children.operator_op.right = &right;
  return res;
}
ast_node Parser::parsePrefixExpression() {
  ast_node res;
  auto infix = current_token;
  res.token = infix;
  res.kind = AST_INFIX;
  advance();
  auto right = parseExpression(precidence_map[current_token.tk_type]);
  res.children.infix.child = &right;
  return res;
}
ast_node Parser::ParseGroupExpr() {
  advance();
  sexp += current_token.keyword;
  auto node = parseExpression(pr_lowest);
  return node;
}
ast_node Parser::ParseIdentifierExpr() {
  ast_node res;
  res.kind = AST_VAR;
  res.token = current_token; 
  // TODO :- Check if a variable or something like that
  return res;
}