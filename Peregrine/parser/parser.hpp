#ifndef PEREGRINE_PARSER
#define PEREGRINE_PARSER
#include <bits/stdc++.h>

#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/tokens.hpp"
#include <cstdint>
#define precedence std::map<TokenType, Precedence_type>
typedef enum {
  pr_lowest,         // lowest possible precedence
  pr_and_or,         // and,or
  pr_not,            // not
  pr_assign_compare, // =, ==, !=, <, >, <=, >=
  pr_sum_minus,      //+, -
  pr_mul_div,        //*, /, %, //
  pr_prefix          // -x
} Precedence_type;
precedence create_map();
class Parser {
private:
  precedence precidence_map = create_map();
  void advance();
  Token next();
  Body parse_exp();

public:
  Token current_token;
  uint64_t tk_index = 0;
  LEXEME tokens;
  std::string filename;
  Ast parse();
};
#endif