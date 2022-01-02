#ifndef PEREGRINE_PARSER_HPP
#define PEREGRINE_PARSER_HPP

#include "ast/ast.hpp"
#include "errors/error.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <map>
#include <vector>

using namespace ast;

enum PrecedenceType {
    pr_lowest,      // lowest possible precedence
    pr_and_or,      // and,or
    pr_not,         // not
    pr_compare,     // ==, !=, <, >, <=, >=
    pr_bit_or,      // |
    pr_bit_xor,     // ^
    pr_bit_and,     // &
    pr_bit_shift,   // >> , <<
    pr_sum_minus,   // +, -
    pr_mul_div,     // *, /, %, //
    pr_expo,        // **
    pr_prefix,      // -x
    pr_dot_arrow_ref,     // x.test(), x.prop ,x->y
    pr_list_access, // x[0], x["test"]
    pr_call         // x()
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
    bool is_dot_arrow_exp=false;
    bool is_imported_var();
    void advance();
    void advanceOnNewLine();
    void expect(TokenType expectedType);
    Token next();
    PrecedenceType nextPrecedence();

    void error(Token tok, std::string_view msg);

    AstNodePtr parseInteger();
    AstNodePtr parseDecimal();
    AstNodePtr parseString(bool isFormatted, bool isRaw);
    AstNodePtr parseBool();
    AstNodePtr parseNone();
    AstNodePtr parseIdentifier();
    AstNodePtr parseName();
    AstNodePtr parseList();
    AstNodePtr parseDict();
    AstNodePtr parseCpp();

    AstNodePtr parseType();
    AstNodePtr parsePointerType();
    AstNodePtr parseListType();
    AstNodePtr parseDictType();
    AstNodePtr parseFuncType();

    AstNodePtr parseExpression(PrecedenceType type = pr_lowest);
    AstNodePtr parsePrefixExpression();
    AstNodePtr parseGroupedExpr();

    AstNodePtr parseBinaryOperation(AstNodePtr left);
    AstNodePtr parseFunctionCall(AstNodePtr left);
    AstNodePtr parseListOrDictAccess(AstNodePtr left);
    AstNodePtr parseDotExpression(AstNodePtr left);
    AstNodePtr parseArrowExpression(AstNodePtr left);

    AstNodePtr parseVirtual();
    AstNodePtr parseCast();
    AstNodePtr parseStatement();
    AstNodePtr parseWith();
    AstNodePtr parseDecoratorCall();
    AstNodePtr parseBlockStatement();
    AstNodePtr parseImport();
    AstNodePtr parseRaise();
    AstNodePtr parseStatic();
    AstNodePtr parseInline();
    AstNodePtr parseVariableStatement();
    AstNodePtr parseConstDeclaration();
    AstNodePtr parseIf();
    AstNodePtr parseAssert();
    AstNodePtr parseMatch();
    AstNodePtr parseScope();
    AstNodePtr parseWhile();
    AstNodePtr parseFor();
    AstNodePtr parseFunctionDef();
    AstNodePtr parseClassDefinition();
    AstNodePtr parseReturn();
    AstNodePtr parseUnion();
    AstNodePtr parseEnum();
    AstNodePtr parseExport();
    AstNodePtr parseTypeDef();
    AstNodePtr parseDefaultArg();

  public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    AstNodePtr parse();
};

#endif
