#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace ast;

Parser::Parser(const std::vector<Token>& tokens,std::string filename) : m_tokens(tokens) {
    m_currentToken = tokens[0];
    m_filename=filename;
}

Parser::~Parser() {}

AstNodePtr Parser::parse() {
    std::vector<AstNodePtr> statements;
    std::string comment;
    while (m_currentToken.tkType != tk_eof) {
        if(m_currentToken.tkType==tk_string && statements.size()==0 && comment==""){
          comment=m_currentToken.keyword;
        }
        statements.push_back(parseStatement());
        if(m_currentToken.tkType!=tk_new_line && m_currentToken.tkType!=tk_dedent){
            error(m_currentToken,"Expected newline after statement");
        }
        advance();
    }
    

    return std::make_shared<Program>(statements,comment);
}

AstNodePtr Parser::parseStatement() {
    AstNodePtr stmt;

    switch (m_currentToken.tkType) {
        case tk_string: {
            while (m_currentToken.tkType == tk_string ||
                   m_currentToken.tkType == tk_new_line) {
                advance();
            }
            stmt = parseStatement();
            break;
        }
        case tk_dollar:{
            auto tok=m_currentToken;
            advance();
            auto x=is_compile_time;
            is_compile_time=true;
            stmt=parseStatement();
            is_compile_time=x;
            stmt=std::make_shared<CompileTimeExpression>(tok,stmt);
            break;
        }
        case tk_static: {
            stmt = parseStatic();
            break;
        }
        case tk_private: {
            stmt = parsePrivate();
            break;
        }
        case tk_with: {
            stmt = parseWith();
            break;
        }

        case tk_inline: {
            stmt = parseInline();
            break;
        }

        case tk_const: {
            stmt = parseConstDeclaration();
            break;
        }

        case tk_assert: {
            stmt = parseAssert();
            break;
        }

        case tk_at: {
            stmt = parseDecoratorCall();
            break;
        }

        case tk_if: {
            stmt = parseIf();
            break;
        }

        case tk_while: {
            stmt = parseWhile();
            break;
        }

        case tk_for: {
            stmt = parseFor();
            break;
        }

        case tk_from:
        case tk_import: {
            stmt = parseImport();
            break;
        }

        case tk_def: {
            stmt = parseFunctionDef();
            break;
        }

        case tk_break: {
            stmt = std::make_shared<BreakStatement>(m_currentToken);
            advanceOnNewLine();
            break;
        }

        case tk_raise: {
            stmt = parseRaise();
            advanceOnNewLine();
            break;
        }

        case tk_ellipses:
        case tk_pass: {
            stmt = std::make_shared<PassStatement>(m_currentToken);
            advanceOnNewLine();
            break;
        }

        case tk_match: {
            stmt = parseMatch();
            break;
        }

        case tk_continue: {
            stmt = std::make_shared<ContinueStatement>(m_currentToken);
            advanceOnNewLine();
            break;
        }

        case tk_return: {
            stmt = parseReturn();
            break;
        }

        case tk_scope: {
            stmt = parseScope();
            break;
        }

        case tk_type: {
            stmt = parseTypeDef();
            break;
        }
        case tk_asm: {
            stmt = parseAsm();
            break;
        }

        case tk_class: {
            stmt = parseClassDefinition();
            break;
        }

        case tk_union: {
            stmt = parseUnion();
            break;
        }

        case tk_enum: {
            stmt = parseEnum();
            break;
        }
        case tk_export:{
            stmt = parseExport();
            break;
        }
        case tk_try:{
            stmt = parseTryExcept();
            break;
        }
        case tk_extern:{
            stmt=parseExtern();
            break;
        }
        case tk_virtual:{
            error(m_currentToken, "Virtual function should be inside class only","","","e4");
            break;
        }
        case tk_else:{
            error(m_currentToken, "else statement without a previous if","","","e4");
            break;
        }
        case tk_elif:{
            error(m_currentToken, "elif statement without a previous if","","","e4");
            break;
        }
        case tk_except:{
            error(m_currentToken, "except statement without a previous try","","","");
            break;
        }
        case tk_case:{
            error(m_currentToken, "case statement without a previous match","","","");
            break;
        }
        case tk_default:{
            error(m_currentToken, "default statement without a previous match","","","");
            break;
        }

        case tk_identifier: {
            if ((next().tkType == tk_colon || next().tkType == tk_assign)) {
                // variable
                stmt = parseVariableStatement();
                break;
            } 
            // if it got here, it will go down the cases and match the default
            // case. DO NOT add another case below this one
        }

        default: {
            /*
                if it didn't match the statements above, then it must be
                either an expression or invalid
            */
            stmt = parseExpression();
            if(next().tkType==tk_assign){
                advance();
                auto tok=m_currentToken;
                advance();
                auto value=parseExpression();
                AstNodePtr varType = std::make_shared<NoLiteral>();
                stmt = std::make_shared<VariableStatement>(tok, varType, stmt, value);
            }
            else if (next().tkType==tk_comma){
                stmt = parseMultipleAssign(stmt);
            }
            else if(std::count(aug_operators.begin(), aug_operators.end(), next().tkType)!=0){
                advance();
                auto tok=m_currentToken;
                advance();
                auto value=parseExpression();
                stmt = std::make_shared<AugAssign>(tok,stmt,value);
            }
            break;
        }
    }

    return stmt;
}

AstNodePtr Parser::parseBlockStatement() {
    advance(); // when this is called, we are on the tk_ident token

    std::vector<AstNodePtr> statements;

    while (m_currentToken.tkType != tk_dedent) {
        if (m_currentToken.tkType == tk_eof) {
            error(m_currentToken,
                  "Expected end of identation, got EOF instead","","","e1");
        }

        statements.push_back(parseStatement());
        advance();
    }

    return std::make_shared<BlockStatement>(statements);
}

AstNodePtr Parser::parseVirtual() {
    auto tok = m_currentToken;
    expect(tk_def,
           "Expected a function declaration but got "+next().keyword+" instead","Declare a function here","","e4");
    AstNodePtr body = parseFunctionDef();
    return std::make_shared<VirtualStatement>(tok, body);
}

AstNodePtr Parser::parseImport() {
    Token tok = m_currentToken;
    bool hasFrom = m_currentToken.tkType == tk_from;

    advance(); // skip from or import token

    AstNodePtr moduleName=std::make_shared<NoLiteral>();
    std::pair<AstNodePtr, AstNodePtr> tmpmoduleName={std::make_shared<NoLiteral>(),std::make_shared<NoLiteral>()};
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols;
    do {
        if (m_currentToken.tkType==tk_comma){
            advance();
        }
        tmpmoduleName.first = parseName();
        if(next().tkType==tk_dot){
            advance();
            while(m_currentToken.tkType==tk_dot){
                auto tok=m_currentToken;
                expect(tk_identifier,next().keyword+" is not a identifier","","","");
                tmpmoduleName.first=std::make_shared<DotExpression>(tok,tmpmoduleName.first,parseName());
                advance();
                if(m_currentToken.tkType!=tk_dot){
                    break;
                }
            }
            
        }
        if (!hasFrom) {
            if (next().tkType == tk_as) {
                advance();
                advance();
                tmpmoduleName.second = parseName();
            }
            else{
                tmpmoduleName.second = std::make_shared<NoLiteral>();
            }
            advanceOnNewLine();
            importedSymbols.push_back(tmpmoduleName);
            if(next().tkType==tk_comma){
                advance();
            }
        }
        else{
            break;
        }
    }while (m_currentToken.tkType == tk_comma);
    if(!hasFrom){
        return std::make_shared<ImportStatement>(tok, moduleName,
                                                    importedSymbols);
    }
    moduleName=tmpmoduleName.first;
    expect(tk_import);
    if(next().tkType==tk_multiply){
        advance();
        advance();
        return std::make_shared<ImportStatement>(tok, moduleName,
                                                    importedSymbols,true);
    }
    do {
        advance();

        std::pair<AstNodePtr, AstNodePtr> importedSymbol;

        importedSymbol.first = parseName();

        if (next().tkType == tk_as) {
            advance();
            advance();

            importedSymbol.second = parseName();
        }
        else{
            importedSymbol.second = std::make_shared<NoLiteral>();
        }
        importedSymbols.push_back(importedSymbol);

        if (next().tkType == tk_comma)
            advance();

    } while (m_currentToken.tkType == tk_comma);

    advanceOnNewLine();
    return std::make_shared<ImportStatement>(tok, moduleName, importedSymbols);
}

AstNodePtr Parser::parseStatic() {
    auto tok = m_currentToken;
    advance();
    AstNodePtr body;
    switch (m_currentToken.tkType) {
        case tk_def: {
            body = parseFunctionDef();
            break;
        }
        case tk_inline: {
            body = parseInline();
            break;
        }
        case tk_const:{
            body=parseConstDeclaration();
            break;
        }
        case tk_identifier: {
            if (next().tkType == tk_colon || next().tkType == tk_assign) {
                body = parseVariableStatement();
                break;
            }
            // if it got here, it will go down the cases and match the default
            // case. DO NOT add another case below this one
        }
        default: {
            error(m_currentToken , "Expected a function or variable or constant declaration but got "+m_currentToken.keyword+" instead","","","");
        }
    }
    return std::make_shared<StaticStatement>(tok, body);
}

AstNodePtr Parser::parseInline() {
    auto tok = m_currentToken;
    expect(tk_def, "Expected function defination but got " +
                          next().keyword +
                          " instead");
    AstNodePtr body;
    body = parseFunctionDef();
    return std::make_shared<InlineStatement>(tok, body);
}

AstNodePtr Parser::parseDefaultArg(){
    auto tok=m_currentToken;
    AstNodePtr name=parseName();
    advance();
    advance();
    AstNodePtr value=parseExpression();
    return std::make_shared<DefaultArg>(tok,name, value);
}

AstNodePtr Parser::parseExport() {
    auto tok = m_currentToken;
    expect(tk_def, "Expected function defination but got " +
                          next().keyword +
                          " instead");
    AstNodePtr body=parseFunctionDef();
    return std::make_shared<ExportStatement>(tok, body);
}

AstNodePtr Parser::parseExtern(){
    auto tok=m_currentToken;
    expect(tk_identifier,"Expected identifier but got "+next().keyword+" instead","","","");
    auto name=m_currentToken.keyword;
    std::vector<std::string> libs;
    expect(tk_assign,"Expected = but got "+next().keyword+" instead","","","");
    expect(tk_import,"Expected = but got "+next().keyword+" instead","","","");
    expect(tk_l_paren,"Expected = but got "+next().keyword+" instead","","","");
    while(m_currentToken.tkType!=tk_r_paren){
        expect(tk_string,"Expected string but got "+next().keyword+" instead","","","");
        libs.push_back(m_currentToken.keyword);
        advance();
    }
    advanceOnNewLine();
    return std::make_shared<ExternStatement>(tok,libs,name);
}

AstNodePtr Parser::parsePrivate(bool is_class){
    auto tok=m_currentToken;
    advance();
    AstNodePtr exp;
    switch (m_currentToken.tkType){
        case tk_identifier:{
            exp=parseVariableStatement();
            break;
        }
        case tk_static:{
            exp=parseStatic();
            break;
        }
        case tk_union:{
            exp=parseUnion();
            break;
        }
        case tk_inline:{
            exp=parseInline();
            break;
        }
        case tk_def:{
            exp=parseFunctionDef();
            break;
        }
        case tk_class:{
            exp=parseClassDefinition();
            break;
        }
        case tk_type:{
            exp=parseTypeDef();
            break;
        }
        case tk_const:{
            exp=parseConstDeclaration();
            break;
        }
        case tk_enum:{
            exp=parseEnum();
            break;
        }
        case tk_virtual:{
            if(!is_class){
                error(m_currentToken,"Virtual statement not allowed outside class","","","");
            }
            exp=parseVirtual();
            break;
        }
        default:{
            error(m_currentToken,"Expected a defination of a class,function,union or variable but got "+m_currentToken.keyword+" instead","","","");
        }
    }
    return std::make_shared<PrivateDef>(tok,exp);
}