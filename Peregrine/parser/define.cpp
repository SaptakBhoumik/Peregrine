#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
AstNodePtr Parser::parseFunctionDef() {
    if(next().tkType==tk_l_paren){
        return parseMethodDef();
    }
    Token tok = m_currentToken;
    expect(tk_identifier, "Expected a name but got "+next().keyword+" instead","Add a name here","","");
    if(next().tkType==tk_dot){
        return parseExternFuncDef(tok);
    }
    AstNodePtr name = parseName();
    std::vector<AstNodePtr> generics;
    if(next().tkType==tk_dict_open){
        generics=parseGenericsDef();
    }
    expect(tk_l_paren,"Expected a ( but got "+next().keyword+" instead");
    std::vector<parameter> parameters;

    advance();
    while (m_currentToken.tkType != tk_r_paren) {
        parameters.push_back(parseParameter());
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

    AstNodePtr returnType=std::make_shared<TypeExpression>(Token{},"void");

    if (next().tkType == tk_arrow) {
        advance();
        advance();
        returnType=parseType();
        if(next().tkType==tk_comma){
            returnType=parseReturnTypeTurple(returnType);
        }
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
                                                parameters, body,comment,generics);
}

AstNodePtr Parser::parseMultipleAssign(AstNodePtr left){
    std::vector<AstNodePtr> names;
    std::vector<AstNodePtr> values;
    names.push_back(left);
    expect(tk_comma,"Expected , but got "+next().keyword+" instead","","","");
    while (m_currentToken.tkType==tk_comma){
        advance();
        names.push_back(parseExpression());
        if(next().tkType==tk_comma){
            advance();
        }
    }
    expect(tk_assign,"Expected = but got "+next().keyword+" instead","","","");
    advance();
    values.push_back(parseExpression());
    advance();
    while (m_currentToken.tkType==tk_comma){
        advance();
        values.push_back(parseExpression());
        if(next().tkType==tk_comma){
            advance();
        }
    }
    return std::make_shared<MultipleAssign>(names,values);
}

AstNodePtr Parser::parseMethodDef() {
    Token tok = m_currentToken;
    advance();
    advance();
    bool is_const=false;
    if(m_currentToken.tkType==tk_const){
        advance();
        is_const=true;
    }
    parameter reciever;
    reciever.p_default=std::make_shared<NoLiteral>();;
    reciever.p_name=parseName();
    if(next().tkType==tk_colon){
        advance();
        advance();
        reciever.p_type=parseType();
    }
    else{
        reciever.p_type=std::make_shared<NoLiteral>();
    }
    reciever.is_const=is_const;
    expect(tk_r_paren,"Expected ) but got "+next().keyword+" instead","","","");
    // advance();
    expect(tk_identifier, "Expected a name but got "+next().keyword+" instead","Add a name here","","");

    AstNodePtr name = parseName();
    std::vector<AstNodePtr> generics;
    if(next().tkType==tk_dict_open){
        generics=parseGenericsDef();
    }
    expect(tk_l_paren,"Expected a ( but got "+next().keyword+" instead");
    std::vector<parameter> parameters;

    advance();
    while (m_currentToken.tkType != tk_r_paren) {
        parameters.push_back(parseParameter());
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

    AstNodePtr returnType=std::make_shared<TypeExpression>(Token{},"void");

    if (next().tkType == tk_arrow) {
        advance();
        advance();
        returnType=parseType();
        if(next().tkType==tk_comma){
            returnType=parseReturnTypeTurple(returnType);
        }
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
    return std::make_shared<MethodDefinition>(tok, returnType, name,
                                                parameters,reciever, body,comment,generics);
}


AstNodePtr Parser::parseExternFuncDef(Token tok){
    auto owner=m_currentToken.keyword;
    advance();
    expect(tk_identifier,"Expected identifier but got "+next().keyword+" instead","","","");
    AstNodePtr name=parseName();
    expect(tk_l_paren,"Expected ( but got "+next().keyword+" instead","","","");
    std::vector<AstNodePtr> parameters;
    advance();
    while (m_currentToken.tkType != tk_r_paren) {
        if(m_currentToken.tkType==tk_ellipses){
            parameters.push_back(std::make_shared<EllipsesTypeExpr>(m_currentToken));
            advance();
            break;
        }else{
            parameters.push_back(parseType());
        }
        advance();
        if(m_currentToken.tkType==tk_comma){
            advance();
        }
        else if(m_currentToken.tkType==tk_r_paren){
            break;
        }
        else{
            error(m_currentToken,"Expected , or ) but got "+m_currentToken.keyword+" instead");
        }
    }
    advance();
    AstNodePtr returnType=std::make_shared<TypeExpression>(Token{},"void");
    if(m_currentToken.tkType==tk_arrow){
        advance();
        returnType=parseType();
    }
    advanceOnNewLine();
    return std::make_shared<ExternFuncDef>(tok,returnType,name,parameters,owner);
}
AstNodePtr Parser::parseExternUnion(Token tok) {
    auto owner=m_currentToken.keyword;
    advance();
    expect(tk_identifier, "Expected an identifier, got " +
                                  next().keyword +
                                  " instead");
    AstNodePtr union_name = parseName();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements;
    if(next().tkType!=tk_colon){
        //it is an incomplete defination
        advanceOnNewLine();
        return std::make_shared<ExternUnionLiteral>(tok, elements, union_name,owner);
    }
    expect(tk_colon, "Expected : but got "+next().keyword+" instead","Add a : here","","");
    expect(tk_ident, "Expected identation but got "+next().keyword+" instead","","","");
    advance();
    while (m_currentToken.tkType != tk_dedent) {
        while(m_currentToken.tkType==tk_string){
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
    return std::make_shared<ExternUnionLiteral>(tok, elements, union_name,owner);
}

AstNodePtr Parser::parseExternStruct(Token tok) {
    auto owner=m_currentToken.keyword;
    advance();
    expect(tk_identifier, "Expected an identifier, got " +
                                  next().keyword +
                                  " instead");
    AstNodePtr union_name = parseName();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements;
    if(next().tkType!=tk_colon){
        //it is an incomplete defination
        advanceOnNewLine();
        return std::make_shared<ExternStructLiteral>(tok, elements, union_name,owner);
    }
    expect(tk_colon, "Expected : but got "+next().keyword+" instead","Add a : here","","");
    expect(tk_ident, "Expected identation but got "+next().keyword+" instead","","","");
    advance();
    while (m_currentToken.tkType != tk_dedent) {
        while(m_currentToken.tkType==tk_string){
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
    return std::make_shared<ExternStructLiteral>(tok, elements, union_name,owner);
}
AstNodePtr Parser::parseClassDefinition() {
    Token tok = m_currentToken;

    std::vector<AstNodePtr> other; // nested union and class
    std::vector<AstNodePtr> attributes;
    std::vector<AstNodePtr> methods;
    std::vector<AstNodePtr> dec_methods;

    expect(tk_identifier,
                "Expected the name of the class but got "+next().keyword+" instead","Add a name here","","e2");
    if(next().tkType==tk_dot){
        return parseExternStruct(tok);
    }
    AstNodePtr name = parseName();
    std::vector<AstNodePtr> generics;
    if(next().tkType==tk_dict_open){
        generics=parseGenericsDef();
    }
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
            case tk_private:{
                if(next().tkType==tk_identifier){
                    attributes.push_back(parsePrivate(true));
                }
                else if(next().tkType==tk_const){
                    attributes.push_back(parsePrivate(true));
                }
                else if(next().tkType==tk_union){
                    other.push_back(parsePrivate(true));
                }
                else if(next().tkType==tk_class){
                    other.push_back(parsePrivate(true));
                }
                else if (next().tkType==tk_enum) {
                    other.push_back(parsePrivate(true));
                }
                else if (next().tkType==tk_static) {
                    if(m_tokens[m_tokIndex+2].tkType==tk_const||m_tokens[m_tokIndex+2].tkType==tk_identifier){
                        attributes.push_back(parsePrivate(true));
                    }
                    else{
                        methods.push_back(parsePrivate(true));
                    }
                }
                else{
                    methods.push_back(parsePrivate(true));
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
            case tk_at:{
                methods.push_back(parseDecoratorCall());
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
            case tk_ellipses:
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
                                             methods, other,comment,generics);
}

AstNodePtr Parser::parseVariableStatement() {
    Token tok = m_currentToken;
    AstNodePtr varType = std::make_shared<NoLiteral>();
    AstNodePtr name = parseName();
    bool has_value=false;
    advance();

    if (m_currentToken.tkType == tk_colon) {
        advance();
        varType = parseType();
        advance();
    }
    
    AstNodePtr value = std::make_shared<NoLiteral>();
    if (m_currentToken.tkType == tk_assign||has_value) {
        advance();
        value = parseExpression();
    } else {
        if(m_currentToken.tkType!=tk_new_line){
            error(m_currentToken,
                    "Expected a new line or =  but got "+m_currentToken.keyword+" instead","","","");
        }
        //not necessary ig because the current token the one after a 
        // advanceOnNewLine();
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
AstNodePtr Parser::parseUnion() {
    auto tok = m_currentToken;
    expect(tk_identifier, "Expected an identifier, got " +
                                  next().keyword +
                                  " instead");
    if(next().tkType==tk_dot){
        return parseExternUnion(tok);
    }
    AstNodePtr union_name = parseName();
    std::vector<AstNodePtr> generics;
    if(next().tkType==tk_dict_open){
        generics=parseGenericsDef();
    }
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
    return std::make_shared<UnionLiteral>(tok, elements, union_name,comment,generics);
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
AstNodePtr Parser::parseTypeDef() {
    Token tok = m_currentToken;
    advance();

    AstNodePtr name = parseName();
    std::vector<AstNodePtr> generics;
    if(next().tkType==tk_dict_open){
        generics=parseGenericsDef();
    }
    if(generics.size()>0 && next().tkType==tk_gr_or_equ){
        advance();
    }
    else{
        expect(tk_assign, "Expected an = but got "+next().keyword+" instead","","","");
    }
    advance();

    AstNodePtr type = parseType();

    advanceOnNewLine();
    return std::make_shared<TypeDefinition>(tok, name, type, generics);
}

std::vector<AstNodePtr> Parser::parseGenericsDef(){
    advance();//on the { after advance
    std::vector<AstNodePtr> generics;
    while(m_currentToken.tkType!=tk_dict_close){
        expect(tk_identifier,"Expected an identifier but got "+next().keyword+" instead","","","");
        generics.push_back(parseName());
        if(next().tkType==tk_comma||next().tkType==tk_dict_close){
            advance();
        }
        else{
            expect(tk_dict_close,"Expected '>' but got "+next().keyword+" instead","","","");
        }
    }
    return generics;
}