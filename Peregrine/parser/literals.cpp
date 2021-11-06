#include "ast/ast.hpp"
#include "parser.hpp"

AstNode Parser::ParseInteger() {
    return IntegerLiteral(m_current_token.keyword);
}

AstNode Parser::ParseDecimal() {
    return DecimalLiteral(m_current_token.keyword);
}

AstNode Parser::ParseString(bool is_formatted) {
    return StringLiteral(m_current_token.keyword, is_formatted);
}

AstNode Parser::ParseBool() {
    return BoolLiteral(m_current_token.keyword);
}

AstNode Parser::ParseList() {
    
}

AstNode Parser::ParseDict() {
    
}

AstNode Parser::ParseCpp() {

}

AstNode Parser::ParseNone() {
    return NoneLiteral();
}
