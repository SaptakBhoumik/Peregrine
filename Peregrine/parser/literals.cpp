#include "ast/ast.hpp"
#include "parser.hpp"

#include <memory>

AstNodePtr Parser::ParseInteger() {
    return std::make_shared<IntegerLiteral>(m_current_token.keyword);
}

AstNodePtr Parser::ParseDecimal() {
    return std::make_shared<DecimalLiteral>(m_current_token.keyword);
}

AstNodePtr Parser::ParseString(bool is_formatted) {
    return std::make_shared<StringLiteral>(m_current_token.keyword,
                                           is_formatted);
}

AstNodePtr Parser::ParseBool() {
    return std::make_shared<BoolLiteral>(m_current_token.keyword);
}

AstNodePtr Parser::ParseList() {}

AstNodePtr Parser::ParseDict() {}

AstNodePtr Parser::ParseCpp() {}

AstNodePtr Parser::ParseNone() { return std::make_shared<NoneLiteral>(); }
