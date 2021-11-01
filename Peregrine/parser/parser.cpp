#include "parser.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/tokens.hpp"
#include <bits/stdc++.h>
#include <cstdint>
#include <iostream>
#include <vector>

precedence create_map() {
  precedence precidence_map;
  precidence_map[tk_and] = pr_and_or;
  precidence_map[tk_or] = pr_and_or;
  precidence_map[tk_not] = pr_not;
  precidence_map[tk_assign] = pr_assign_compare;
  precidence_map[tk_not_equal] = pr_assign_compare;
  precidence_map[tk_greater] = pr_assign_compare;
  precidence_map[tk_less] = pr_assign_compare;
  precidence_map[tk_gr_or_equ] = pr_assign_compare;
  precidence_map[tk_less_or_equ] = pr_assign_compare;
  precidence_map[tk_plus] = pr_sum_minus;
  precidence_map[tk_minus] = pr_sum_minus;
  precidence_map[tk_multiply] = pr_mul_div;
  precidence_map[tk_divide] = pr_mul_div;
  precidence_map[tk_string] = pr_lowest;
  precidence_map[tk_decimal] = pr_lowest;
  precidence_map[tk_integer] = pr_lowest;
  precidence_map[tk_true] = pr_lowest;
  precidence_map[tk_false] = pr_lowest;
  precidence_map[tk_identifier] = pr_lowest;
  precidence_map[tk_none] = pr_lowest;
  return precidence_map;
}
void Parser::advance() {
  auto temp = current_token.line;
  tk_index++;
  if (tk_index < tokens.size()) {
    current_token = tokens[tk_index];
  }
  if (current_token.line > temp) {
    sexp += "\n";
  }
}
Token Parser::next() {
  Token token;
  if (tk_index + 1 < tokens.size()) {
    token = tokens.at(tk_index + 1);
  }
  return token;
}
Precedence_type Parser::next_precedence() {
  if (precidence_map.count(next().tk_type) > 0) {
    return precidence_map[next().tk_type];
  }

  return pr_lowest;
}
bool Parser::expect(std::vector<TokenType> list, TokenType res) {
  if (std::count(list.begin(), list.end(), res)) {
    return false;
  }
  return true;
}
AST Parser::parse() {
  AST ast;
  current_token = tokens[0];
  while (current_token.tk_type != tk_eof) {
    ast.root.push_back(parse_statement());
    advance(); // eating the token to prevent infinite loop
  }
  return ast;
}
ast_node Parser::parse_statement() {
  ast_node node;

  switch (current_token.tk_type) {
  case tk_format: {
    sexp += "| f |";
    advance(); // to make it a string
    sexp += current_token.keyword;
    node = ParseStringExpr(true, false);
    break;
  }
  case tk_raw: {
    sexp += "| r |";
    advance(); // to make it a string
    sexp += current_token.keyword;
    node = ParseStringExpr(false, true);
    break;
  }
  case tk_string: {
    node = parseExpression(precidence_map[tk_string]);
    break;
  }
  case tk_identifier: {
    node = parseExpression(precidence_map[tk_identifier]);
    break;
  }
  case tk_integer: {
    node = parseExpression(precidence_map[tk_integer]);
    break;
  }
  case tk_decimal: {
    node = parseExpression(precidence_map[tk_decimal]);
    break;
  }
  case tk_true: {
    node = parseExpression(precidence_map[tk_true]);
    break;
  }
  case tk_false: {
    node = parseExpression(precidence_map[tk_false]);
    break;
  }
  case tk_none: {
    node = parseExpression(precidence_map[tk_none]);
    break;
  }
  default: {
    // do something lol
  }
  }
  return node;
}
