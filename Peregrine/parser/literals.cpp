#include "ast/ast.hpp"
#include "parser.hpp"

#include <memory>

using namespace ast;

AstNodePtr Parser::parseInteger() {
    return std::make_shared<IntegerLiteral>(m_currentToken,
                                            m_currentToken.keyword);
}

AstNodePtr Parser::parseDecimal() {
    return std::make_shared<DecimalLiteral>(m_currentToken,
                                            m_currentToken.keyword);
}

AstNodePtr Parser::parseString(bool isFormatted, bool isRaw) {
    return std::make_shared<StringLiteral>(
        m_currentToken, m_currentToken.keyword, isFormatted, isRaw);
}

AstNodePtr Parser::parseBool() {
    return std::make_shared<BoolLiteral>(m_currentToken,
                                         m_currentToken.keyword);
}

AstNodePtr Parser::parseList() {
    Token tok = m_currentToken;
    std::vector<AstNodePtr> elements;

    if (next().tkType != tk_list_close) {
        do {
            advance();

            elements.push_back(parseExpression(pr_lowest));

            advance();
        } while (m_currentToken.tkType == tk_comma);
    } else {
        advance();
    }

    if (m_currentToken.tkType != tk_list_close) {
        error(m_currentToken,
              "expected ], got " + m_currentToken.keyword + " instead");
    }

    advanceOnNewLine();

    // TODO: remove the type member? im not sure

    return std::make_shared<ListLiteral>(tok, elements);
}

AstNodePtr Parser::parseDict() {
    Token tok = m_currentToken;
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements;

    if (next().tkType != tk_dict_close) {
        do {
            advance();

            // TODO: keys should not be made out of any type of expression
            AstNodePtr key = parseExpression(pr_lowest);
            expect(tk_colon);
            advance();
            AstNodePtr value = parseExpression(pr_lowest);

            elements.push_back(std::pair(key, value));
            advance();
        } while (m_currentToken.tkType == tk_comma);
    } else {
        advance();
    }

    if (m_currentToken.tkType != tk_dict_close) {
        error(m_currentToken,
              "expected }, got " + m_currentToken.keyword + " instead");
    }

    advanceOnNewLine();

    return std::make_shared<DictLiteral>(tok, elements);
}

AstNodePtr Parser::parseIdentifier() {
    if (m_currentToken.tkType!=tk_identifier){
        error(m_currentToken,
              "Expected identifier but got " + m_currentToken.keyword + " instead");
    }
    return std::make_shared<IdentifierExpression>(m_currentToken,
                                                  m_currentToken.keyword);
}

AstNodePtr Parser::parseName() {
    if (m_currentToken.tkType != tk_identifier) {
        error(m_currentToken, "expected an identifier, got " +
                                  m_currentToken.keyword +
                                  " instead");
    }

    return parseIdentifier();
}

AstNodePtr Parser::parseNone() {
    return std::make_shared<NoneLiteral>(m_currentToken);
}
