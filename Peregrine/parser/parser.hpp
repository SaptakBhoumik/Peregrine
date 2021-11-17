#ifndef PEREGRINE_PARSER_HPP
#define PEREGRINE_PARSER_HPP

#include "ast/ast.hpp"
#include "errors/error.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <map>
#include <vector>

enum Precedence_type {
    pr_lowest,         // lowest possible precedence
    pr_and_or,         // and,or
    pr_not,            // not
    pr_compare,        // ==, !=, <, >, <=, >=
    pr_bit_or,         // |
    pr_bit_xor,        // ^
    pr_bit_and,        // &
    pr_bit_shift,      // >> , <<
    pr_sum_minus,      // +, -
    pr_mul_div,        // *, /, %, //
    pr_expo,           // **
    pr_prefix,         // -x
    pr_call            // x()
};

std::map<TokenType, Precedence_type> create_map();

class Parser {
  private:
    size_t m_tk_index{0};
    Token m_current_token;

    std::vector<Token> m_tokens;
    std::string m_filename;

    std::vector<PEError> m_errors;

    std::map<TokenType, Precedence_type> precedence_map = create_map();

    void advance();
    void advanceOnNewLine();
    void expect(TokenType expected_type);
    Token next();
    Precedence_type next_precedence();

    void error(Token tok, std::string_view msg);

    AstNodePtr ParseInteger();
    AstNodePtr ParseDecimal();
    AstNodePtr ParseString(bool is_formatted,bool is_raw);
    AstNodePtr ParseBool();
    AstNodePtr ParseNone();
    AstNodePtr ParseIdentifier();
    AstNodePtr ParseList();
    AstNodePtr ParseDict();
    AstNodePtr ParseCpp();

    AstNodePtr ParseExpression(Precedence_type type);
    AstNodePtr ParsePrefixExpression();
    AstNodePtr ParseGroupedExpr();

    AstNodePtr ParseBinaryOperation(AstNodePtr left);
    AstNodePtr ParseFunctionCall(AstNodePtr left);

    AstNodePtr ParseStatement();
    AstNodePtr ParseBlockStatement();
    AstNodePtr ParseVariableStatement();
    AstNodePtr ParseConstDeclaration();
    AstNodePtr ParseIf();
    AstNodePtr ParseScope();
    AstNodePtr ParseWhile();
    AstNodePtr ParseFunctionDef();
    AstNodePtr ParseReturn();

  public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    AstNodePtr parse();
};

#endif