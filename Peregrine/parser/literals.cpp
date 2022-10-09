#include "ast/ast.hpp"
#include "parser.hpp"

#include <memory>
namespace Parser{

using namespace ast;

AstNodePtr Parser::parseInteger() {
    //746
    return std::make_shared<IntegerLiteral>(m_currentToken,
                                            m_currentToken.keyword);
}

AstNodePtr Parser::parseDecimal() {
    //2.56
    return std::make_shared<DecimalLiteral>(m_currentToken,
                                            m_currentToken.keyword);
}

AstNodePtr Parser::parseString(bool isRaw) {
    //"string "
    return std::make_shared<StringLiteral>(
        m_currentToken, m_currentToken.keyword,isRaw);
}

AstNodePtr Parser::parseBool() {
    //True or False
    return std::make_shared<BoolLiteral>(m_currentToken,
                                         m_currentToken.keyword);
}

AstNodePtr Parser::parseList() {
    //list defination
    //[1,2,3,4]
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

    return std::make_shared<ListLiteral>(tok, elements);
}

AstNodePtr Parser::parseDict() {
    //dictionary literalstderr
    //{"key1":"value1","key2":"value2"}
    Token tok = m_currentToken;
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements;

    if (next().tkType != tk_dict_close) {
        do {
            advance();

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
    return std::make_shared<IdentifierExpression>(m_currentToken,
                                                  m_currentToken.keyword);
}

AstNodePtr Parser::parseName() {
    //identifier name
    if (m_currentToken.tkType != tk_identifier) {
        error(m_currentToken, "expected an identifier, got " +
                                  m_currentToken.keyword +
                                  " instead");
    }

    return parseIdentifier();
}

AstNodePtr Parser::parseNone() {
    //None
    return std::make_shared<NoneLiteral>(m_currentToken);
}
}