#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"

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

        case tk_static: {
            stmt = parseStatic();
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

        case tk_cppcode: {
            advance();
            stmt = parseCpp();
            advanceOnNewLine();
            break;
        }

        case tk_type: {
            stmt = parseTypeDef();
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
        case tk_virtual:{
            error(m_currentToken, "Virtual function should be inside class only","","","e4");
            break;
        }
        // TODO: variables currently do not work with all the types, we need to
        // fix this
        case tk_identifier: {
            if ((next().tkType == tk_colon || next().tkType == tk_assign)) {
                // variable
                stmt = parseVariableStatement();
                break;
            } else if ((next().tkType == tk_dot||next().tkType == tk_list_open||next().tkType == tk_arrow) && is_imported_var()) {
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

AstNodePtr Parser::parseClassDefinition() {
    Token tok = m_currentToken;

    std::vector<AstNodePtr> other; // nested union and class
    std::vector<AstNodePtr> attributes;
    std::vector<AstNodePtr> methods;
    std::vector<AstNodePtr> dec_methods;

    expect(tk_identifier,
                "Expected the name of the class but got "+next().keyword+" instead","Add a name here","","e2");

    AstNodePtr name = parseName();
    std::vector<AstNodePtr> parent;

    if (next().tkType == tk_l_paren) {
        advance();
        advance();
        while (m_currentToken.tkType != tk_r_paren) {
            parent.push_back(parseType()); // IMPORTANT: It should always use
                                           // parseType instead of parseName
            advance();
            if (m_currentToken.tkType == tk_comma) {
                advance();
            }
        }
    }

    expect(tk_colon,
    "Expected : but got "+next().keyword+" instead","Add a : here","","e2");

    expect(tk_ident,"Expected an ident but got "+next().keyword+" instead");
    advance();
    std::string comment;
    if(m_currentToken.tkType==tk_string){
      comment=m_currentToken.keyword;
    }
    while (m_currentToken.tkType != tk_dedent) {
        switch (m_currentToken.tkType) {
            case tk_string: { // multiline comment
                while (m_currentToken.tkType == tk_string) {
                    advance();
                }
                break;
            }
            case tk_identifier: {
                attributes.push_back(parseVariableStatement());
                break;
            }
            case tk_const: {
                attributes.push_back(parseConstDeclaration());
                break;
            }
            case tk_static: {
                if (next().tkType == tk_def || next().tkType == tk_inline) {
                    methods.push_back(parseStatic());
                } else {
                    attributes.push_back(parseStatic());
                }
                break;
            }
            case tk_def: {
                methods.push_back(parseFunctionDef());
                break;
            }
            case tk_virtual: {
                methods.push_back(parseVirtual());
                break;
            }
            case tk_inline: {
                methods.push_back(parseInline());
                break;
            }
            case tk_union: { // union def in class
                other.push_back(parseUnion());
                break;
            }
            case tk_class: { // nested class
                other.push_back(parseClassDefinition());
                break;
            }
            case tk_enum: {
                other.push_back(parseEnum());
                break;
            }
            case tk_pass:{
                advance();
                break;
            }
            case tk_eof:{
                error(m_currentToken,
                      "Expected end of identation, got EOF instead","","","e1");
                break;
            }
            default: {
                error(m_currentToken,
                      "Expected a method or variable declaration or enums or nested class/union but got " +
                          m_currentToken.keyword + " instead" ,"A class can only contain methods(functions) or variable declaration or enums or nested class/union ","","e3");
            }
        }

        advance();
    }
    return std::make_shared<ClassDefinition>(tok, name, parent, attributes,
                                             methods, other,comment);
}

AstNodePtr Parser::parseVirtual() {
    auto tok = m_currentToken;
    expect(tk_def,
           "Expected a function declaration but got "+next().keyword+" instead","Declare a function here","","e4");
    AstNodePtr body = parseFunctionDef();
    return std::make_shared<VirtualStatement>(tok, body);
}

AstNodePtr Parser::parseImport() {
    //TODO: Show error
    Token tok = m_currentToken;
    bool hasFrom = m_currentToken.tkType == tk_from;

    advance(); // skip from or import token

    std::pair<AstNodePtr, AstNodePtr> moduleName;
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols;

    moduleName.first = parseName();

    if (!hasFrom) {
        if (next().tkType == tk_as) {
            advance();
            advance();

            moduleName.second = parseName();
        }
        advanceOnNewLine();
        return std::make_shared<ImportStatement>(tok, moduleName,
                                                 importedSymbols);
    }

    expect(tk_import);

    do {
        advance();

        std::pair<AstNodePtr, AstNodePtr> importedSymbol;

        importedSymbol.first = parseName();

        if (next().tkType == tk_as) {
            advance();
            advance();

            importedSymbol.second = parseName();
        }

        importedSymbols.push_back(importedSymbol);

        if (next().tkType == tk_comma)
            advance();

    } while (m_currentToken.tkType == tk_comma);

    advanceOnNewLine();
    return std::make_shared<ImportStatement>(tok, moduleName, importedSymbols);
}

// TODO: make this invalid: int = 43
AstNodePtr Parser::parseVariableStatement() {
    Token tok = m_currentToken;
    AstNodePtr varType = std::make_shared<NoLiteral>();
    AstNodePtr name = parseName();
    advance();

    if (m_currentToken.tkType == tk_colon) {
        advance();
        varType = parseType();
        advance();
    }
    else{
      if(m_currentToken.tkType==tk_list_open){
        name=parseListOrDictAccess(name);
        advance();
      }
      else{
        while (m_currentToken.tkType == tk_dot) {
            name = parseDotExpression(name);
            advance();
            while (m_currentToken.tkType == tk_arrow) {
                name = parseArrowExpression(name);
                advance();
            }
        }
        while (m_currentToken.tkType == tk_arrow) {
            name = parseArrowExpression(name);
            advance();
            while (m_currentToken.tkType == tk_dot) {
                name = parseDotExpression(name);
                advance();
            }
        }
      }
    }
    AstNodePtr value = std::make_shared<NoLiteral>();

    if (m_currentToken.tkType == tk_assign) {
        advance();
        value = parseExpression();
    } else {
        advanceOnNewLine();
    }

    return std::make_shared<VariableStatement>(tok, varType, name, value);
}

AstNodePtr Parser::parseConstDeclaration() {
    Token tok = m_currentToken;
    expect(tk_identifier);
    AstNodePtr name = parseName();
    // advance();
    AstNodePtr constType = std::make_shared<NoLiteral>();
    if (next().tkType == tk_colon) {
        advance();
        advance();
        constType = parseType();
    }
    expect(tk_assign,"Expected an assignment but got "+next().keyword+" instead","Constants can't have uninitialised value","","");
    advance();

    AstNodePtr value = parseExpression();
    return std::make_shared<ConstDeclaration>(tok, constType, name, value);
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
    }

    AstNodePtr elseBody = std::make_shared<NoLiteral>();

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

AstNodePtr Parser::parseFunctionDef() {
    Token tok = m_currentToken;
    expect(tk_identifier, "Expected a name but got "+next().keyword+" instead","Add a name here","","");

    AstNodePtr name = parseName();

    expect(tk_l_paren,"Expected a ( but got "+next().keyword+" instead");
    std::vector<parameter> parameters;

    advance();
    while (m_currentToken.tkType != tk_r_paren) {
        AstNodePtr paramName = parseName();
        AstNodePtr paramType = std::make_shared<NoLiteral>();
        AstNodePtr paramDefault = std::make_shared<NoLiteral>();
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
        parameters.push_back(parameter{paramType, paramName,paramDefault});
        if (m_currentToken.tkType == tk_comma) {
            advance();
        } else {
            break;
        }
    }

    if (m_currentToken.tkType != tk_r_paren) {
        error(m_currentToken,
              "expected ), got " + m_currentToken.keyword + " instead");
    }

    AstNodePtr returnType =std::make_shared<TypeExpression>(Token(), "void");

    if (next().tkType == tk_arrow) {
        advance();
        advance();

        returnType = parseType();
    }
    std::string comment;
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");
    size_t line=m_currentToken.line;
    AstNodePtr body;
    if(next().tkType!=tk_ident && next().line==line){
      advance();
      std::vector<AstNodePtr> x;
      x.push_back(parseStatement());
      body = std::make_shared<BlockStatement>(x);
    }
    else{
      expect(tk_ident, "Expected an ident but got "+next().keyword+" instead");
      if (next().tkType==tk_string){
        comment=next().keyword;
      }
      body = parseBlockStatement();
    }
    return std::make_shared<FunctionDefinition>(tok, returnType, name,
                                                parameters, body,comment);
}

AstNodePtr Parser::parseReturn() {
    Token tok = m_currentToken;
    AstNodePtr returnValue = std::make_shared<NoLiteral>();

    if (next().tkType != tk_new_line) {
        advance();
        returnValue = parseExpression();
    } else {
        advance();
    }

    return std::make_shared<ReturnStatement>(tok, returnValue);
}

AstNodePtr Parser::parseTypeDef() {
    Token tok = m_currentToken;
    advance();

    AstNodePtr name = parseName();

    expect(tk_assign, "Expected an = but got "+next().keyword+" instead","","","");
    advance();

    AstNodePtr type = parseType();

    advanceOnNewLine();
    return std::make_shared<TypeDefinition>(tok, name, type);
}

AstNodePtr Parser::parseExpression(PrecedenceType currPrecedence) {
    AstNodePtr left;

    switch (m_currentToken.tkType) {
        case tk_integer: {
            left = parseInteger();
            break;
        }

        case tk_decimal: {
            left = parseDecimal();
            break;
        }

        case tk_cast: {
            // TODO: cast<type>(v)=9 maybe?
            left = parseCast();
            break;
        }

        case tk_none: {
            left = parseNone();
            break;
        }

        case tk_format: {
            advance(); // making it a string
            left = parseString(true, false);
            break;
        }

        case tk_raw: {
            advance(); // making it a string
            left = parseString(false, true);
            break;
        }

        case tk_string: {
            left = parseString(false, false);
            break;
        }

        case tk_true:
        case tk_false: {
            left = parseBool();
            break;
        }

        case tk_identifier: {
            left = parseIdentifier();
            break;
        }

        case tk_l_paren: {
            left = parseGroupedExpr();
            break;
        }

        case tk_list_open: {
            left = parseList();
            break;
        }

        case tk_dict_open: {
            left = parseDict();
            break;
        }

        case tk_negative:
        case tk_not:
        case tk_ampersand:
        case tk_plus:
        case tk_multiply:
        case tk_bit_not: {
            left = parsePrefixExpression();
            break;
        }
        case tk_ident: {
            error(m_currentToken,
                  "IndentationError: unexpected indent");
            break;
        }
        case tk_dedent: {
            error(m_currentToken,
                  "IndentationError: unexpected dedent");
            break;
        }
        case tk_new_line: {
            error(m_currentToken,
                  "Unexpected newline");
            break;
        }
        case tk_eof: {
            error(m_tokens[m_tokens.size() - 2],
                  "Unexpected end of file","","","e1");
            break;
        }
        default: {
            error(m_currentToken,
                  m_currentToken.keyword + " is not an expression");
            break;
        }
    }

    while (nextPrecedence() > currPrecedence) {
        advance();

        switch (m_currentToken.tkType) {
            case tk_l_paren: {
                left = parseFunctionCall(left);
                break;
            }
           case tk_if:{
               left = parseTernaryIf(left);
               break;
           }
            case tk_list_open: {
                left = parseListOrDictAccess(left);
                break;
            }

            case tk_dot: {
                left = parseDotExpression(left);
                break;
            }

            case tk_arrow: {
                left = parseArrowExpression(left);
                break;
            }
            case tk_increment:
            case tk_decrement:{
                left = parsePostfixExpression(left);
                break;
            }
            default: {
                left = parseBinaryOperation(left);
                break;
            }
        }
    }

    advanceOnNewLine();

    return left;
}

AstNodePtr Parser::parseBinaryOperation(AstNodePtr left) {
    Token op = m_currentToken;
    PrecedenceType precedence = precedenceMap[m_currentToken.tkType];

    advance();
    AstNodePtr right = parseExpression(precedence);
    return std::make_shared<BinaryOperation>(op, left, op, right);
}

AstNodePtr Parser::parseFunctionCall(AstNodePtr left) {
    Token tok = m_currentToken;
    std::vector<AstNodePtr> arguments;

    if (next().tkType != tk_r_paren) {
        do {
            advance();
            if(m_currentToken.tkType==tk_identifier && next().tkType==tk_assign){
                arguments.push_back(parseDefaultArg());
            }
            else{
                arguments.push_back(parseExpression());
            }

            advance();
        } while (m_currentToken.tkType == tk_comma);
    } else {
        advance();
    }

    if (m_currentToken.tkType != tk_r_paren) {
        error(m_currentToken,
              "expected ), got " + m_currentToken.keyword + " instead");
    }

    advanceOnNewLine();

    return std::make_shared<FunctionCall>(tok, left, arguments);
}

AstNodePtr Parser::parseListOrDictAccess(AstNodePtr left) {
    Token tok = m_currentToken;
    advance();
    std::vector<AstNodePtr> keyOrIndex;
    keyOrIndex.push_back(parseExpression());
    //array slicing
    if(next().tkType==tk_colon){
        advance();
        advance();
        keyOrIndex.push_back(parseExpression());
    }
    expect(tk_list_close, "Expected ] but got "+next().keyword+" instead","Add a ] here","","");

    AstNodePtr node = std::make_shared<ListOrDictAccess>(tok, left, keyOrIndex);
    return node;
}

AstNodePtr Parser::parseDotExpression(AstNodePtr left) {
    Token tok = m_currentToken;
    PrecedenceType currentPrecedence = precedenceMap[tok.tkType];
    advance();
    AstNodePtr referenced;
    // TODO: validate output of parseExpression
    referenced = parseExpression(currentPrecedence);
    return std::make_shared<DotExpression>(tok, left, referenced);
}

AstNodePtr Parser::parsePrefixExpression() {
    Token prefix = m_currentToken;
    PrecedenceType precedence = precedenceMap[m_currentToken.tkType];

    advance();

    AstNodePtr right = parseExpression(precedence);

    return std::make_shared<PrefixExpression>(prefix, prefix, right);
}

AstNodePtr Parser::parsePostfixExpression(AstNodePtr left) {
    Token prefix = m_currentToken;
    return std::make_shared<PostfixExpression>(prefix, prefix, left);
}

AstNodePtr Parser::parseGroupedExpr() {
    advance();

    AstNodePtr expr = parseExpression();

    expect(tk_r_paren, "Expected ) but got "+next().keyword+" instead","Add a ) here","","");

    return expr;
}

AstNodePtr Parser::parseIdentifier() {
    if (m_currentToken.tkType!=tk_identifier){
        error(m_currentToken,
              "Expected identifier but got " + m_currentToken.keyword + " instead");
    }
    return std::make_shared<IdentifierExpression>(m_currentToken,
                                                  m_currentToken.keyword);
}

AstNodePtr Parser::parseType() {
    switch (m_currentToken.tkType) {
        case tk_def:
            return parseFuncType();
        /* TODO: Change in syntax
        case tk_dict:
            return parseDictType();//treate this as generic
        */
        case tk_multiply: {
            return parsePointerType();
        }
        case tk_list_open:
            return parseListType();

        case tk_identifier: {
            if (next().tkType == tk_dot) {
                AstNodePtr left = parseName();
                advance();
                return parseDotExpression(left);
            }
            return std::make_shared<TypeExpression>(m_currentToken,
                                                    m_currentToken.keyword);
        }

        default: {
            error(m_currentToken, m_currentToken.keyword + " is not a type");
        }
    }

    return nullptr;
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

    AstNodePtr elemType = parseType();
    return std::make_shared<ListTypeExpr>(tok, elemType, size);
}

AstNodePtr Parser::parsePointerType() {
    Token tok = m_currentToken;
    advance();
    AstNodePtr typePtr = parseType();
    return std::make_shared<PointerTypeExpr>(tok, typePtr);
}

AstNodePtr Parser::parseDictType() {
    // Token tok = m_currentToken;
    // expect(tk_list_open);
    // advance();

    // AstNodePtr keyType = parseType();

    // expect(tk_list_close);
    // advance();

    // AstNodePtr valueType = parseType();
    // return std::make_shared<DictTypeExpr>(tok, keyType, valueType);
    return nullptr;
}

AstNodePtr Parser::parseFuncType() {
    auto tok = m_currentToken;
    expect(tk_l_paren,"Expected ( but got "+next().keyword+" instead","Add a ( here","","");
    std::vector<AstNodePtr> types; // arg types
    std::vector<AstNodePtr> returnTypes;
    while (m_currentToken.tkType != tk_r_paren) {
        advance();
        if (m_currentToken.tkType == tk_comma) {
            advance();
            types.push_back(parseType());
        } else if (m_currentToken.tkType == tk_r_paren) {
            break;
        } else {
            types.push_back(parseType());
        }
        advance();
    }
    if (next().tkType == tk_arrow) {
        advance();
        advance();
        // TODO: Implement multiple return
        returnTypes.push_back(parseType());
    }
    return std::make_shared<FunctionTypeExpr>(tok, types, returnTypes);
}

AstNodePtr Parser::parseName() {
    if (m_currentToken.tkType != tk_identifier) {
        error(m_currentToken, "expected an identifier, got " +
                                  m_currentToken.keyword +
                                  " instead");
    }

    return parseIdentifier();
}

AstNodePtr Parser::parseMatch() {
    Token tok = m_currentToken;
    // TODO: implement errors
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

AstNodePtr Parser::parseDecoratorCall() {
    auto tok = m_currentToken;
    std::vector<AstNodePtr> decorators;
    AstNodePtr body;
    while (m_currentToken.tkType == tk_at) {
        if (next().tkType != tk_identifier) {
            error(next(), "Expected an identifier, got " +
                                  next().keyword +
                                  " instead");
        }
        advance();
        
        decorators.push_back(parseExpression());
        advance();
    }
    if (m_currentToken.tkType == tk_def) {
        body = parseFunctionDef();
    } else if (m_currentToken.tkType == tk_static) {
        body = parseStatic();
    }
    else if(m_currentToken.tkType==tk_inline){
        error(m_currentToken,"Can't use decorators with inline function","","","");
    }
    else if(m_currentToken.tkType==tk_virtual){
        error(m_currentToken,"Can't use decorators with virtual function","","","");
    }
    else{
        error(m_currentToken, "Expected a function declaration but got "+m_currentToken.keyword+" instead","","","");
    }
    return std::make_shared<DecoratorStatement>(tok, decorators, body);
}

AstNodePtr Parser::parseAssert() {
    auto tok = m_currentToken;
    advance();
    auto condition = parseExpression();
    return std::make_shared<AssertStatement>(tok, condition);
}

AstNodePtr Parser::parseRaise() {
    auto tok = m_currentToken;
    advance();
    AstNodePtr value = parseExpression();
    return std::make_shared<RaiseStatement>(tok, value);
}

AstNodePtr Parser::parseUnion() {
    auto tok = m_currentToken;
    expect(tk_identifier, "Expected an identifier, got " +
                                  next().keyword +
                                  " instead");
    AstNodePtr union_name = parseName();
    expect(tk_colon, "Expected : but got "+next().keyword+" instead","Add a : here","","");
    expect(tk_ident, "Expected identation but got "+next().keyword+" instead","","","");
    advance();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements;
    std::string comment;
    while (m_currentToken.tkType != tk_dedent) {
        while(m_currentToken.tkType==tk_string){
          if(comment=="" && elements.size()==0){
            comment=m_currentToken.keyword;
          }
          advance();
          if(m_currentToken.tkType==tk_new_line){advance();}
        }
        AstNodePtr name = parseName();
        expect(tk_colon);
        advance();
        AstNodePtr type = parseType();
        advance();
        elements.push_back(std::pair(type, name));
        if (m_currentToken.tkType == tk_new_line) {
            advance();
        }
        else if(m_currentToken.tkType==tk_dedent){}
        else{
            error(m_currentToken, "Expected new line or dedent but got "+m_currentToken.keyword+" instead","","","");
        }
    }
    return std::make_shared<UnionLiteral>(tok, elements, union_name,comment);
}

AstNodePtr Parser::parseEnum() {
    auto token = m_currentToken;
    expect(tk_identifier, "Expected an identifier, got " +
                                  next().keyword +
                                  " instead");
    AstNodePtr enum_name = parseName();
    expect(tk_colon, "Expected : but got "+next().keyword+" instead","Add a : here","","");
    auto line=m_currentToken.line;
    TokenType stopat=tk_dedent;
    if(next().tkType!=tk_ident && next().line==line && next().tkType!=tk_new_line){stopat=tk_new_line;}
    else{
        expect(tk_ident,"Expected identation but got "+next().keyword+" instead","","","");
        }
    advance();
    std::string comment;
    if (m_currentToken.tkType==tk_string){
      comment=m_currentToken.keyword;
    }
    std::vector<std::pair<AstNodePtr, AstNodePtr>> fields;
    AstNodePtr val;

    while (m_currentToken.tkType != stopat) {
        while(m_currentToken.tkType==tk_string){
          advance();
          if(m_currentToken.tkType==tk_new_line){advance();}
        }
        AstNodePtr name = parseName();
        advance();
        if (m_currentToken.tkType == tk_assign) {
            advance();
            val = parseExpression();
        } else {
            val = std::make_shared<NoLiteral>();
        }
        if(stopat==tk_dedent){
          advance();
        }
        fields.push_back(std::pair(name, val));
        if (m_currentToken.tkType == tk_comma) {
            advance();
        }
        if (m_currentToken.tkType == tk_new_line && m_currentToken.tkType!=stopat) {
            advance();
        }
    }

    return std::make_shared<EnumLiteral>(token, fields, enum_name,comment);
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

AstNodePtr Parser::parseCast() {
    auto tok = m_currentToken;
    expect(tk_less, "Expected < but got " +
                         next().keyword +
                         " instead");
    advance();
    AstNodePtr type = parseType();
    expect(tk_greater, "Expected > but got " +
                            next().keyword +
                            " instead");
    expect(tk_l_paren,"Expected ( but got "+next().keyword+" instead","","","");
    advance();

    AstNodePtr value = parseExpression();
    expect(tk_r_paren,"Expected ) but got "+next().keyword+" instead","","","");
    return std::make_shared<CastStatement>(tok, type, value);
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

AstNodePtr Parser::parseArrowExpression(AstNodePtr left) {
    Token tok = m_currentToken;
    PrecedenceType currentPrecedence = precedenceMap[tok.tkType];
    advance();
    AstNodePtr referenced;
    // TODO: validate output of parseExpression
    referenced = parseExpression(currentPrecedence);
    return std::make_shared<ArrowExpression>(tok, left, referenced);
}
AstNodePtr Parser::parseTernaryIf(AstNodePtr left){
    auto tok=m_currentToken;
    advance();
    AstNodePtr if_condition=parseExpression(pr_conditional);
    if (next().tkType != tk_else) {
        if (m_currentToken.tkType==tk_new_line){
            error(m_currentToken,"Expected else but got newline instead",
                        "Ternary if statement not possible without an else body",
                        "Add an else body here","");
        }
        else{
            error(next(),"Expected else but got "+
                            next().keyword+
                            " instead",
                            "Ternary if statement not possible without an else body",
                            "Add an else body here","");
        }
    } 
    advance();
    advance();
    AstNodePtr else_value=parseExpression(pr_conditional);
    return std::make_shared<TernaryIf>(tok,left,if_condition,else_value);
}
