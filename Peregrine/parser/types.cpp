#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

AstNodePtr Parser::parseType(bool can_be_sumtype) {
    ast::AstNodePtr res;
    switch (m_currentToken.tkType) {
        case tk_l_paren: {
            advance();
            std::vector<AstNodePtr> types;
            AstNodePtr curr_type = parseType();
            types.push_back(curr_type);
            if(next().tkType==tk_comma){
                advance();
            }
            while(m_currentToken.tkType==tk_comma){
                advance();
                types.push_back(parseType());
                if(next().tkType==tk_comma){
                    advance();
                }
            }
            expect(tk_r_paren, "Expected ) but got "+next().keyword+" instead","Add a ) here","","");
            res=std::make_shared<TypeTuple>(false,types);
            break;
        }
        case tk_def:{
            res = parseFuncType();
            break;
        }
        case tk_multiply: {
            res = parsePointerType();
            break;
        }
        case tk_ampersand: {
            res = parseRefType();
            break;
        }
        case tk_list_open:{
            res = parseListType();
            break;
        }
        case tk_identifier: {
            if (next().tkType == tk_dot) {
                res = parseImportedType();
            }
            else if(next().tkType==tk_dict_open){
                auto tok=m_currentToken;
                advance();
                advance();
                std::vector<AstNodePtr> generic_types;
                while (m_currentToken.tkType != tk_dict_close) {
                    generic_types.push_back(parseType());
                    advance();
                    if (m_currentToken.tkType == tk_comma) {
                        advance();
                    } else if (m_currentToken.tkType == tk_dict_close) {
                        break;
                    }else{
                        error(m_currentToken,"Expected { or , but got "+m_currentToken.keyword+" instead","","","");
                    }
                }
                res = std::make_shared<TypeExpression>(tok,
                                                    tok.keyword,generic_types);
            }
            else{
                res = std::make_shared<TypeExpression>(m_currentToken,
                                                        m_currentToken.keyword);
            }
            break;
        }

        default: {
            error(m_currentToken, m_currentToken.keyword + " is not a type");
        }
    }
    if(next().tkType==tk_bit_or && can_be_sumtype){
        advance();
        std::vector<AstNodePtr> sum_types;
        sum_types.push_back(res);
        while(m_currentToken.tkType==tk_bit_or){
            advance();
            sum_types.push_back(parseType(false));
            if(next().tkType==tk_bit_or){
                advance();
            }
        } 
        res=std::make_shared<SumType>(sum_types);   
    }
    return res;
}

AstNodePtr Parser::parseListType() {
    Token tok = m_currentToken;
    AstNodePtr size=std::make_shared<NoLiteral>();
    if (next().tkType != tk_list_close) {   
        advance();
        size = parseExpression();
    }
    expect(tk_list_close, "Expected ] but got "+next().keyword+" instead","Add a ] here","","");
    advance();

    AstNodePtr elemType = parseType(false);
    return std::make_shared<ListTypeExpr>(tok, elemType, size);
}

AstNodePtr Parser::parsePointerType() {
    Token tok = m_currentToken;
    advance();
    AstNodePtr typePtr = parseType(false);
    return std::make_shared<PointerTypeExpr>(tok, typePtr);
}

AstNodePtr Parser::parseRefType() {
    Token tok = m_currentToken;
    advance();
    AstNodePtr typePtr = parseType(false);
    return std::make_shared<RefTypeExpr>(tok, typePtr);
}
AstNodePtr Parser::parseImportedType(){
    AstNodePtr name=parseName();
    advance();
    while(m_currentToken.tkType==tk_dot){
        auto tok=m_currentToken;
        expect(tk_identifier,next().keyword+" is not a type","","","");
        if(next().tkType!=tk_dot){
            name=std::make_shared<DotExpression>(tok,name,parseType(false));
            break;
        }
        else{
            name=std::make_shared<DotExpression>(tok,name,parseName());
            advance();
        }
    }
    return name;
}

AstNodePtr Parser::parseFuncType() {
    auto tok = m_currentToken;
    expect(tk_l_paren,"Expected ( but got "+next().keyword+" instead","Add a ( here","","");
    std::vector<AstNodePtr> types; // arg types
    AstNodePtr returnTypes=std::make_shared<TypeExpression>(Token(), "void");
    while (m_currentToken.tkType != tk_r_paren) {
        advance();
        if (m_currentToken.tkType == tk_comma) {
            advance();
            if(m_currentToken.tkType==tk_multiply && (next().tkType==tk_comma||next().tkType==tk_r_paren)){
                types.push_back(std::make_shared<VarArgTypeExpr>(m_currentToken));
            }
            else if(m_currentToken.tkType==tk_multiply && next().tkType==tk_multiply){
                if(m_tokens[m_tokIndex+2].tkType==tk_comma||m_tokens[m_tokIndex+2].tkType==tk_r_paren){
                    types.push_back(std::make_shared<VarKwargTypeExpr>(m_currentToken));
                    advance();
                }
                else{
                    types.push_back(parseType());
                }   
            }
            else if(m_currentToken.tkType==tk_ellipses){
                types.push_back(std::make_shared<EllipsesTypeExpr>(m_currentToken));
            }
            else{
                types.push_back(parseType());
            }
        } else if (m_currentToken.tkType == tk_r_paren) {
            break;
        } else {
            if(m_currentToken.tkType==tk_multiply && (next().tkType==tk_comma||next().tkType==tk_r_paren)){
                types.push_back(std::make_shared<VarArgTypeExpr>(m_currentToken));
            }
            else if(m_currentToken.tkType==tk_multiply && next().tkType==tk_multiply){
                if(m_tokens[m_tokIndex+2].tkType==tk_comma||m_tokens[m_tokIndex+2].tkType==tk_r_paren){
                    types.push_back(std::make_shared<VarKwargTypeExpr>(m_currentToken));
                    advance();
                }
                else{
                    types.push_back(parseType());
                }   
            }
            else if(m_currentToken.tkType==tk_ellipses){
                types.push_back(std::make_shared<EllipsesTypeExpr>(m_currentToken));
            }
            else{
                types.push_back(parseType());
            }
        }
        if(next().tkType==tk_r_paren || next().tkType==tk_comma){
            advance();
        }
        else{
            expect(tk_r_paren,"Expected ) but got "+next().keyword+" instead","Add a ) here","","");
        }
    }
    if (next().tkType == tk_arrow) {
        advance();
        advance();
        returnTypes=parseType();
        if(next().tkType==tk_comma){
            returnTypes=parseReturnTypeTurple(returnTypes);
        }
    }
    return std::make_shared<FunctionTypeExpr>(tok, types, returnTypes);
}