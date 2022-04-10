#include "parser.hpp"
#include <iostream>
#include <algorithm>


void Parser::advance() {
    m_tokIndex++;

    if (m_tokIndex < m_tokens.size()) {
        m_currentToken = m_tokens[m_tokIndex];
    }
}

void Parser::advanceOnNewLine() {
    if (next().tkType == tk_new_line) {
        advance();
    }
}

Token Parser::next() {
    Token token;

    if (m_tokIndex + 1 < m_tokens.size()) {
        token = m_tokens[m_tokIndex + 1];
    }

    return token;
}

PrecedenceType Parser::nextPrecedence() {
    if(m_currentToken.tkType == tk_new_line||m_currentToken.tkType == tk_dedent) {
        return pr_lowest;
    }
    else if (precedenceMap.count(next().tkType) > 0) {
        return precedenceMap[next().tkType];
    }
    return pr_lowest;
}

void Parser::error(Token tok, std::string msg,std::string submsg,std::string hint,std::string ecode) {
    PEError err = {{tok.line, tok.start,tok.location, m_filename, tok.statement},
                   std::string(msg),
                   submsg,
                   hint,
                   ecode};

    display(err);
    exit(1);
}

void Parser::expect(TokenType expectedType, std::string msg,std::string submsg,std::string hint,std::string ecode) {
    if (next().tkType != expectedType) {
        if(msg==""){
            msg="expected token of type " + std::to_string(expectedType) +", got " + std::to_string(next().tkType) + " instead";
        }
        if(next().tkType==tk_new_line){
            error(m_currentToken,msg,submsg,hint,ecode);
        }
        else{
            error(next(),msg,submsg,hint,ecode);
        }
    }
    advance();
}

std::map<TokenType, PrecedenceType> createMap() {
    std::map<TokenType, PrecedenceType> precedenceMap;
    precedenceMap[tk_for] = pr_conditional;
    precedenceMap[tk_double_dot] = pr_range;
    precedenceMap[tk_dollar] = pr_prefix;
    precedenceMap[tk_bit_not] = pr_prefix;
    precedenceMap[tk_if] = pr_conditional;
    precedenceMap[tk_else] = pr_conditional;
    precedenceMap[tk_and] = pr_and;
    precedenceMap[tk_or] = pr_or;
    precedenceMap[tk_not] = pr_not;
    precedenceMap[tk_not_equal] = pr_compare;
    precedenceMap[tk_is_not] = pr_compare;
    precedenceMap[tk_is] = pr_compare;
    precedenceMap[tk_not_in] = pr_compare;
    precedenceMap[tk_in] = pr_compare;
    precedenceMap[tk_greater] = pr_compare;
    precedenceMap[tk_less] = pr_compare;
    precedenceMap[tk_gr_or_equ] = pr_compare;
    precedenceMap[tk_less_or_equ] = pr_compare;
    precedenceMap[tk_equal] = pr_compare;
    precedenceMap[tk_bit_or] = pr_bit_or;
    precedenceMap[tk_xor] = pr_bit_xor;
    precedenceMap[tk_ampersand] = pr_bit_and;
    precedenceMap[tk_shift_left] = pr_bit_shift_pipeline;
    precedenceMap[tk_shift_right] = pr_bit_shift_pipeline;
    precedenceMap[tk_pipeline] = pr_bit_shift_pipeline;
    precedenceMap[tk_plus] = pr_sum_minus;
    precedenceMap[tk_minus] = pr_sum_minus;
    precedenceMap[tk_multiply] = pr_mul_div;
    precedenceMap[tk_divide] = pr_mul_div;
    precedenceMap[tk_modulo] = pr_mul_div;
    precedenceMap[tk_floor] = pr_mul_div;
    precedenceMap[tk_exponent] = pr_expo;
    precedenceMap[tk_dot] = pr_dot_arrow_ref;
    precedenceMap[tk_arrow] = pr_dot_arrow_ref;
    precedenceMap[tk_list_open] = pr_list_access;
    precedenceMap[tk_l_paren] = pr_call;
    precedenceMap[tk_increment] = pr_postfix;
    precedenceMap[tk_decrement] = pr_postfix;

    return precedenceMap;
}
parameter Parser::parseParameter(){
    AstNodePtr paramType = std::make_shared<NoLiteral>();
    AstNodePtr paramDefault = std::make_shared<NoLiteral>();
    AstNodePtr paramName = std::make_shared<NoLiteral>();
    bool is_const = false;
    if(m_currentToken.tkType==tk_const){
        is_const=true;
        advance();
    }
    auto tok=m_currentToken;
    if(m_currentToken.tkType==tk_multiply){
        advance();
        ParamType x;
        if(m_currentToken.tkType==tk_multiply){
            paramType=std::make_shared<VarKwargTypeExpr>(tok);
            expect(tk_identifier,"Expected identifier but got "+next().keyword,"","","");
            x=VarKwarg;
            paramName=parseName();
        }
        else if(m_currentToken.tkType==tk_identifier){
            paramType=std::make_shared<VarArgTypeExpr>(tok);
            paramName=parseName();
            x=VarArg;
        }
        else{
            error(m_currentToken,"Expected identifier but got "+m_currentToken.keyword,"","","");
        }
        advance();
        return parameter{paramType, paramName,paramDefault,is_const,x};
    }
    else if(m_currentToken.tkType==tk_ellipses){
        ParamType x=Ellipses;
        paramType=std::make_shared<EllipsesTypeExpr>(tok);
        if(next().tkType==tk_identifier){
            advance();
            paramName=parseName();
        }
        advance();
        return parameter{paramType, paramName,paramDefault,is_const,x};
    }
    paramName = parseName();
    if(next().tkType==tk_comma || next().tkType==tk_r_paren|| next().tkType==tk_assign){}
    else{
        expect(tk_colon,"Expected a : but got "+next().keyword+" instead");
        advance();
        paramType = parseType();
    }
    advance();
    if(m_currentToken.tkType==tk_assign){
        advance();
        paramDefault=parseExpression();
        advance();
    }
    return parameter{paramType, paramName,paramDefault,is_const};
}