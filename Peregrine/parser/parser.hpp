#ifndef PEREGRINE_PARSER
#define PEREGRINE_PARSER

#include "ast/ast.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <map>
#include <vector>

enum Precedence_type {
    pr_lowest,         // lowest possible precedence
    pr_and_or,         // and,or
    pr_not,            // not
    pr_assign_compare, // =, ==, !=, <, >, <=, >=
    pr_sum_minus,      //+, -
    pr_mul_div,        //*, /, %, //
    pr_prefix          // -x
};

precedence create_map();

class Parser {
  private:
    size_t m_tk_index{0};
    Token m_current_token;

    std::vector<Token> m_tokens;
    std::string m_filename;

    std::map<TokenType, Precedence_type> precedence_map = create_map();

    void advance();
    bool expect(TokenType expected_type);
    Token next();
    Precedence_type next_precedence();

    AstNode ParseInteger();
    AstNode ParseDecimal();
    AstNode ParseString(bool is_formatted, bool is_raw);
    AstNode ParseBool();
    AstNode ParseNone();
    AstNode ParseList();
    AstNode ParseDict();
    AstNode ParseCpp();

    AstNode ParseExpression(Precedence_type type);
    AstNode ParsePrefixExpression();
    AstNode ParseIdentifierExpr();
    AstNode ParseGroupExpr();
    AstNode ParseBinaryOperation(AstNode left);

    AstNode ParseStatement();

  public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    AstNode parse();
};

#endif