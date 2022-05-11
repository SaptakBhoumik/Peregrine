#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

AstNodePtr Parser::parseAsm(){
    auto tok=m_currentToken;
    expect(tk_colon,"Expected an ':' but got "+next().keyword+" instead","","","");
    expect(tk_ident,"Expected an indentation but got "+next().keyword+" instead","","","");
    advance();
    std::string assembly="";
    AstNodePtr output=std::make_shared<NoLiteral>();
    std::vector<std::pair<std::string,AstNodePtr>> inputs;
    while(m_currentToken.tkType!=tk_dedent){
        if(m_currentToken.tkType==tk_identifier){
            output=parseExpression();
            expect(tk_assign,"Expected an '=' but got "+next().keyword+" instead","","","");
            advance();
            if(assembly.size()!=0){
                error(m_currentToken,"Error: Can't have multiple result variable","","","");
            }
            else{
                assembly=m_currentToken.keyword;
            }
        }
        else if(m_currentToken.tkType==tk_string){
            if(assembly.size()!=0 && next().tkType!=tk_assign){
                error(m_currentToken,"Error: Can't have multiple result variable","","","");
            }
            else if(next().tkType==tk_assign){
                auto reg=m_currentToken.keyword;
                advance();
                advance();
                auto exp=parseExpression();
                inputs.push_back(std::make_pair(reg,exp));
            }
            else{
                assembly=m_currentToken.keyword;
            }
        }
        else{
            error(m_currentToken,"Expected an identifier or string but got "+m_currentToken.keyword+" instead","","","");
        }
        advance();
        if(m_currentToken.tkType==tk_dedent){break;}
        if(m_currentToken.tkType==tk_new_line){advance();}
    }
    return std::make_shared<InlineAsm>(tok,assembly,output,inputs);
}
AstNodePtr Parser::parseWith() {
    auto tok = m_currentToken;
    advance();
    std::vector<AstNodePtr> variables;
    std::vector<AstNodePtr> values;
    AstNodePtr body;
    while (m_currentToken.tkType != tk_colon) {
        values.push_back(parseExpression());
        if(next().tkType==tk_colon||next().tkType==tk_comma){
            variables.push_back(std::make_shared<NoLiteral>());
        }
        else{
            expect(tk_as, "Expected as but got " +
                               next().keyword +
                               " instead");
            expect(tk_identifier, "Expected an identifier, got " +
                                          next().keyword +
                                          " instead");
            variables.push_back(parseName());
        }
        advance();
        if (m_currentToken.tkType == tk_comma) {
            advance();
        }
    }
    size_t line=m_currentToken.line;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      body = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident,"Expected identation but got "+next().keyword+" instead","","","");
      body = parseBlockStatement();
    }
    return std::make_shared<WithStatement>(tok, variables, values, body);
}
AstNodePtr Parser::parseRaise() {
    auto tok = m_currentToken;
    advance();
    AstNodePtr value = std::make_shared<NoLiteral>();
    if(m_currentToken.tkType!=tk_new_line){
        value = parseExpression();
    }
    return std::make_shared<RaiseStatement>(tok, value);
}
AstNodePtr Parser::parseIf() {
    Token tok = m_currentToken;
    advance(); // skip the if token

    AstNodePtr condition = parseExpression();
    if (next().tkType!=tk_colon){
        error(m_currentToken,
                "Expected a : after the condition but got "+m_currentToken.keyword+" instead","Add a : here","","");
    }
    advance();

    AstNodePtr ifBody;
    auto line=m_currentToken.line;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      ifBody = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
      ifBody = parseBlockStatement();
    }
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs;
    AstNodePtr elseBody = std::make_shared<NoLiteral>();
    if(is_compile_time){
        if(next().tkType==tk_dollar){
            if(m_tokens[m_tokIndex+2].tkType==tk_elif){
                advance();
            }
            else if(m_tokens[m_tokIndex+2].tkType==tk_else){}
            else{
                return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                         elifs);
            }
        }
        else{
             return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                         elifs);
        }
    }
    while (next().tkType == tk_elif) {
        advance();
        advance();

        AstNodePtr condition = parseExpression();

        if (next().tkType!=tk_colon){
            error(m_currentToken,
                "Expected a : after the condition but got "+m_currentToken.keyword+" instead","Add a : here","","");
        }
        advance();
        AstNodePtr body;
        auto line=m_currentToken.line;
        if(next().tkType!=tk_ident && next().line==line){
          advance();
          std::vector<AstNodePtr> x;
          x.push_back(parseStatement());
          body = std::make_shared<BlockStatement>(x);
        }
        else{
            expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
            body = parseBlockStatement();
        }

        elifs.push_back(std::pair(condition, body));
        if(is_compile_time){
            if(next().tkType==tk_dollar){
                if(m_tokens[m_tokIndex+2].tkType==tk_elif){
                    advance();
                }
                else if (m_tokens[m_tokIndex+2].tkType==tk_else){
                    break;
                }
                else{
                    return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                            elifs);
                }
            }
            else{
                return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                            elifs);
            }
        }
    }
    if(is_compile_time){
        if(next().tkType==tk_dollar){
            if(m_tokens[m_tokIndex+2].tkType==tk_else){
                advance();
            }
            else{
                return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                         elifs);
            }
        }
        else{
             return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                         elifs);
        }
    }
    if (next().tkType == tk_else) {
        advance();
        if (next().tkType!=tk_colon){
            error(next(),
                "Expected a : after else but got "+next().keyword+" instead","Add a : here","","");
        }
        advance();
        auto line=m_currentToken.line;
        if(next().tkType!=tk_ident && next().line==line){
          advance();
          std::vector<AstNodePtr> x;
          x.push_back(parseStatement());
          elseBody = std::make_shared<BlockStatement>(x);
        }
        else{
          expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
          elseBody = parseBlockStatement();
        }

    }

    return std::make_shared<IfStatement>(tok, condition, ifBody, elseBody,
                                         elifs);
}

AstNodePtr Parser::parseAssert() {
    auto tok = m_currentToken;
    advance();
    auto condition = parseExpression();
    return std::make_shared<AssertStatement>(tok, condition);
}

AstNodePtr Parser::parseMatch() {
    Token tok = m_currentToken;
    advance();
    std::vector<AstNodePtr> toMatch;
    while (m_currentToken.tkType != tk_colon) {
        toMatch.push_back(parseExpression());
        advance();
        if (m_currentToken.tkType != tk_colon) {
            advance();
        }
    }
    expect(tk_ident, "Expected identation but got "+next().keyword+" instead","","","");
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases;
    while (next().tkType == tk_case) {
        advance();
        advance();
        std::vector<AstNodePtr> cases_arg;
        while (m_currentToken.tkType != tk_colon) {
            if (m_currentToken.keyword == "_") {
                cases_arg.push_back(std::make_shared<NoLiteral>());
            } else {
                cases_arg.push_back(parseExpression());
            }
            advance();

            if (m_currentToken.tkType == tk_comma) {
                advance();
            }
            else if(m_currentToken.tkType==tk_colon){
                break;
            }
            else{
                error(m_currentToken, "Expected , or : but got "+m_currentToken.keyword+" instead","","","");
            }
        }
        if(cases_arg.size()>toMatch.size()){
            error(m_currentToken, "Too many arguments in case","","","");
        }
        else if(cases_arg.size()==0){
            error(m_currentToken, "Too few arguments in case","","","");
        }
        else if(cases_arg.size()<toMatch.size()&&cases_arg.back()->type()!=KAstNoLiteral){
            error(m_currentToken, "Too few arguments in case","","","");
        }
        AstNodePtr body;
        size_t line=m_currentToken.line;
        if(next().tkType!=tk_ident && next().line==line){
            advance();
            std::vector<AstNodePtr> x;
            x.push_back(parseStatement());
            body = std::make_shared<BlockStatement>(x);
        }
        else{
            expect(tk_ident, "Expected identation but got "+next().keyword+" instead","","","");
            body = parseBlockStatement();
        }
        cases.push_back(
            std::pair<std::vector<AstNodePtr>, AstNodePtr>(cases_arg, body));
    }
    AstNodePtr default_body = std::make_shared<NoLiteral>();

    if (next().tkType == tk_default) {
        advance();
        if(next().tkType!=tk_colon){
            error(next(), "Expected : but got "+next().keyword+" instead","Add a : here","","");
        }
        advance();
        size_t line=m_currentToken.line;
        if(next().tkType!=tk_ident && next().line==line){
            advance();
            std::vector<AstNodePtr> x;
            x.push_back(parseStatement());
            default_body = std::make_shared<BlockStatement>(x);
        }
        else{
            expect(tk_ident, "Expected identation but got "+next().keyword+" instead","","","");
            default_body = parseBlockStatement();
        }
    }
    expect(tk_dedent);
    return std::make_shared<MatchStatement>(tok, toMatch, cases, default_body);
}
AstNodePtr Parser::parseScope() {
    Token tok = m_currentToken;
    if (next().tkType!=tk_colon){
            error(next(),
                "Expected a : after scope but got "+next().keyword+" instead","Add a : here","","");
    }
    advance();
    AstNodePtr body;
    auto line=m_currentToken.line;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      body = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
      body = parseBlockStatement();
    }
    return std::make_shared<ScopeStatement>(tok, body);
}


AstNodePtr Parser::parseWhile() {
    Token tok = m_currentToken;
    advance(); // skip the while token

    AstNodePtr condition = parseExpression();

    if (next().tkType!=tk_colon){
            error(m_currentToken,
                "Expected a : after the condition but got "+m_currentToken.keyword+" instead","Add a : here","","");
    }
    advance();
    AstNodePtr body;
    auto line=m_currentToken.line;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      body = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
      body = parseBlockStatement();
    }

    return std::make_shared<WhileStatement>(tok, condition, body);
}

AstNodePtr Parser::parseFor() {
    Token tok = m_currentToken;
    advance();

    std::vector<AstNodePtr> variable;
    while (m_currentToken.tkType != tk_in) {
        variable.push_back(parseName());
        advance();
        if (m_currentToken.tkType == tk_comma) {
            advance();
        } else if (m_currentToken.tkType != tk_in) {
            error(m_currentToken,
                "Expected an in after the variable but got "+m_currentToken.keyword+" instead","Add an in here","","e5");
        }
    }
    advance();

    AstNodePtr sequence = parseExpression();
    if (next().tkType!=tk_colon){
            error(m_currentToken,
                "Expected a : but got "+m_currentToken.keyword+" instead","Add a : here","","");
    }
    advance();
    AstNodePtr body;
    auto line=m_currentToken.line;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      body = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
      body = parseBlockStatement();
    }

    return std::make_shared<ForStatement>(tok, variable, sequence, body);
}

AstNodePtr Parser::parseReturn() {
    Token tok = m_currentToken;
    AstNodePtr returnValue=std::make_shared<NoLiteral>();
    advance();
    if (m_currentToken.tkType != tk_new_line) {
        returnValue = parseExpression();
        if(next().tkType==tk_comma){
            returnValue=parseReturnExprTurple(returnValue);
        }
    }
    return std::make_shared<ReturnStatement>(tok, returnValue);
}

AstNodePtr Parser::parseTryExcept(){
    auto tok=m_currentToken;
    expect(tk_colon,"Expected : but got "+next().keyword+" instead","Add a : here","","");
    auto line=m_currentToken.line;
    AstNodePtr try_body;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      try_body = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident,"Expected identation but got "+next().keyword+" instead","","","");
      try_body = parseBlockStatement();
    }
    expect(tk_except,"Expected except but got "+next().keyword+" instead","Atleast one except is necessary","","");
    AstNodePtr else_body=std::make_shared<NoLiteral>();
    std::vector<except_type> m_except_clauses;
    while(m_currentToken.tkType==tk_except){
        if(next().tkType==tk_colon){
            advance();
            auto line=m_currentToken.line;
            if(next().tkType!=tk_ident && next().line==line){
              advance();
              std::vector<AstNodePtr> x;
              x.push_back(parseStatement());
              else_body = std::make_shared<BlockStatement>(x);
            }
            else{
              expect(tk_ident,"Expected identation but got "+next().keyword+" instead","","","");
              else_body = parseBlockStatement();
            }
            break;
        }
        else{

            AstNodePtr name=std::make_shared<NoLiteral>();
            AstNodePtr except_body=std::make_shared<NoLiteral>();
            std::vector<AstNodePtr> exceptions;
            advance();
            while(m_currentToken.tkType!=tk_colon && m_currentToken.tkType!=tk_as){
                exceptions.push_back(parseExpression());
                if(next().tkType==tk_comma){
                    advance();
                    advance();
                }
                else if(next().tkType==tk_as||next().tkType==tk_colon){
                    advance();
                }
                else{
                    error(next(),"Expected ',' or 'as' or ':' but got "+
                                    next().keyword+
                                    " instead","","","");
                }
            }
            if(m_currentToken.tkType==tk_as){
                advance();
                name=parseName();
                expect(tk_colon,"Expected : but got "+next().keyword+" instead","Add a : here","","");
            }
            auto line=m_currentToken.line;
            if(next().tkType!=tk_ident && next().line==line){
              advance();
              std::vector<AstNodePtr> x;
              x.push_back(parseStatement());
              except_body = std::make_shared<BlockStatement>(x);
            }
            else{
              expect(tk_ident,"Expected identation but got "+next().keyword+" instead","","","");
              except_body = parseBlockStatement();
            }
            if (next().tkType==tk_except){
                advance();
            }
            m_except_clauses.push_back(std::make_pair(std::make_pair(exceptions,name),except_body));
        }
    }
    return std::make_shared<TryExcept>(tok,try_body,m_except_clauses,else_body);
}
