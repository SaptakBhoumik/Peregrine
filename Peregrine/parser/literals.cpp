#include "ast/ast.hpp"
#include "parser.hpp"

#include <memory>

AstNodePtr Parser::ParseInteger() {
    return std::make_shared<IntegerLiteral>(m_current_token.keyword);
}

AstNodePtr Parser::ParseDecimal() {
    return std::make_shared<DecimalLiteral>(m_current_token.keyword);
}

AstNodePtr Parser::ParseString(bool is_formatted,bool is_raw) {
    return std::make_shared<StringLiteral>(m_current_token.keyword,
                                           is_formatted,is_raw);
}

AstNodePtr Parser::ParseBool() {
    return std::make_shared<BoolLiteral>(m_current_token.keyword);
}

AstNodePtr Parser::ParseList() {
    std::vector<AstNodePtr> elements;

    if (next().tk_type != tk_list_close) {
        do {
            advance();

            elements.push_back(ParseExpression(pr_lowest));

            advance();
        } while (m_current_token.tk_type == tk_comma);
    } else {
        advance();
    }

    if (m_current_token.tk_type != tk_list_close) {
        error(m_current_token, "expected ], got " + m_current_token.keyword + " instead");
    }

    advanceOnNewLine();

    //TODO: remove the type member? im not sure
    AstNodePtr type = std::make_shared<NoLiteral>();

    return std::make_shared<ListLiteral>(type, elements);
}

AstNodePtr Parser::ParseDict() {
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements;

    if (next().tk_type != tk_dict_close) {
        do {
            advance();

            //TODO: keys should not be made out of any type of expression
            AstNodePtr key = ParseExpression(pr_lowest);
            expect(tk_colon);
            advance();
            AstNodePtr value = ParseExpression(pr_lowest);

            elements.push_back(std::pair(key, value));
            advance();
        } while (m_current_token.tk_type == tk_comma);
    } else {
        advance();
    }

    if (m_current_token.tk_type != tk_dict_close) {
        error(m_current_token, "expected }, got " + m_current_token.keyword + " instead");
    }

    advanceOnNewLine();

    return std::make_shared<DictLiteral>(elements);
}

AstNodePtr Parser::ParseCpp() {return std::make_shared<CppStatement>(m_current_token.keyword);}

AstNodePtr Parser::ParseNone() { return std::make_shared<NoneLiteral>(); }
