#ifndef PEREGRINE_PARSER
#define PEREGRINE_PARSER
#include <bits/stdc++.h>

#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/tokens.hpp"
#include <cstdint>
#include <vector>
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
  bool expect(std::vector<TokenType> list, TokenType res);
  Precedence_type next_precedence();
  Token current_token;
  uint64_t tk_index = 0;
  // literals
  ast_node ParseStringExpr(bool is_formatted, bool is_raw);
  ast_node ParseNoneExpr();
  ast_node ParseNumberExpr();
  ast_node ParseBoolExpr();
  ast_node ParseListExpr();
  ast_node ParseCppExpr();
  ast_node ParseDictExpr();
  // operators
  ast_node parseExpression(Precedence_type type);
  ast_node parsePrefixExpression();
  ast_node ParseIdentifierExpr();
  ast_node ParseGroupExpr();
  ast_node parseBinaryOperation(ast_node left);

public:
  LEXEME tokens;
  std::string filename;
  std::string sexp; // for debuging
  AST parse();
  ast_node parse_statement();
};
#endif