#ifndef PEREGRINE_PARSER_HPP
#define PEREGRINE_PARSER_HPP

#include "ast/ast.hpp"
#include "errors/error.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <map>
#include <vector>

enum PrecedenceType {
    pr_lowest,    // lowest possible precedence
    pr_and_or,    // and,or
    pr_not,       // not
    pr_compare,   // ==, !=, <, >, <=, >=
    pr_sum_minus, // +, -
    pr_mul_div,   // *, /, %, //
    pr_prefix,    // -x
    pr_call       // x()
};

std::map<TokenType, PrecedenceType> createMap();

class Parser {
  private:
    size_t m_tokIndex{0};
    Token m_currentToken;

    std::vector<Token> m_tokens;
    std::string m_filename;

    std::vector<PEError> m_errors;

    std::map<TokenType, PrecedenceType> precedenceMap = createMap();

    void advance();
    void advanceOnNewLine();
    bool expect(TokenType expectedType);
    Token next();
    PrecedenceType nextPrecedence();

    void error(Token tok, std::string_view msg);

    AstNodePtr parseInteger();
    AstNodePtr parseDecimal();
    AstNodePtr parseString(bool isFormatted, bool isRaw);
    AstNodePtr parseBool();
    AstNodePtr parseNone();
    AstNodePtr parseIdentifier();
    AstNodePtr parseList();
    AstNodePtr ParseDict();
    AstNodePtr parseCpp();

    AstNodePtr parseExpression(PrecedenceType type);
    AstNodePtr ParsePrefixExpression();
    AstNodePtr parseGroupedExpr();

    AstNodePtr parseBinaryOperation(AstNodePtr left);
    AstNodePtr parseFunctionCall(AstNodePtr left);

    AstNodePtr parseStatement();
    AstNodePtr parseBlockStatement();
    AstNodePtr parseVariableStatement();
    AstNodePtr parseConstDeclaration();
    AstNodePtr parseIf();
    AstNodePtr parseWhile();
    AstNodePtr parseFunctionDef();
    AstNodePtr parseReturn();

  public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    AstNodePtr parse();
};

#endif