#include "parser.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/tokens.hpp"
#include <bits/stdc++.h>
#include <cstdint>

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
  return precidence_map;
}
Ast Parser::parse() {
  Ast result;
  result.body.reserve(tokens.size());//reserving extra space to save time
  current_token =tokens[0];
  while (tk_index< tokens.size()) {
    result.body.emplace_back(parse_exp());
    advance();//to prevent infinite loop
  }
  return result;
}
void Parser::advance(){
  tk_index++;
  if (tk_index<tokens.size()){
    current_token=tokens[tk_index];
  }
}
Token Parser::next(){
  Token token;
  if (tk_index+1<tokens.size()){
    token=tokens[tk_index];
  }
  return token;
}