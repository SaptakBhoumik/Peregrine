#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

AstNodePtr Parser::parseExpression(PrecedenceType currPrecedence) {
    AstNodePtr left;

    switch (m_currentToken.tkType) {
        case tk_integer: {
            left = parseInteger();
            break;
        }
        case tk_dollar:{
            auto tok=m_currentToken;
            advance();
            left=parseExpression(pr_prefix);
            left=std::make_shared<CompileTimeExpression>(tok,left);
            break;
        }
        case tk_decimal: {
            left = parseDecimal();
            break;
        }

        case tk_cast: {
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
            if(next().tkType==tk_dict_open){
                left=parseGeneric(left);
            }
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

        case tk_minus:
        case tk_not:
        case tk_ampersand:
        case tk_plus:
        case tk_multiply:
        case tk_bit_not: {
            left = parsePrefixExpression();
            break;
        }
        case tk_def:{
            left=parseLambda();
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
            case tk_for:{
                left = parseTernaryFor(left);
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
    PrecedenceType precedence = pr_prefix;

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
    if (next().tkType == tk_comma) {
        advance();
        std::vector<AstNodePtr> items;
        items.push_back(expr);
        while(m_currentToken.tkType==tk_comma){
            advance();
            items.push_back(parseExpression());
        }
        expect(tk_r_paren, "Expected ), got "+next().keyword+" instead","Add a ) here","","");
        return std::make_shared<ExpressionTuple>(false,items);
    }
    expect(tk_r_paren, "Expected ) but got "+next().keyword+" instead","Add a ) here","","");

    return expr;
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
AstNodePtr Parser::parseTernaryFor(AstNodePtr left){
    auto tok=m_currentToken;
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

    AstNodePtr sequence = parseExpression(pr_conditional);
    advanceOnNewLine();
    return std::make_shared<TernaryFor>(tok,left,sequence,variable);
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
AstNodePtr Parser::parseReturnExprTurple(AstNodePtr item){
    advance();
    std::vector<AstNodePtr> items;
    items.push_back(item);
    while(m_currentToken.tkType==tk_comma){
        advance();
        items.push_back(parseExpression());
        if(next().tkType==tk_comma){
            advance();
        }
    }
    return std::make_shared<ExpressionTuple>(true,items);
}

AstNodePtr Parser::parseReturnTypeTurple(AstNodePtr item){
    advance();
    std::vector<AstNodePtr> items;
    items.push_back(item);
    while(m_currentToken.tkType==tk_comma){
        advance();
        items.push_back(parseType());
        if(next().tkType==tk_comma){
            advance();
        }
    }
    return std::make_shared<TypeTuple>(true,items);
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
AstNodePtr Parser::parseLambda(){
    auto tok=m_currentToken;
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
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");
    advance();
    AstNodePtr body=parseExpression(pr_lambda);
    return std::make_shared<LambdaDefinition>(tok,parameters,body);
}


AstNodePtr Parser::parseGeneric(AstNodePtr identifier){
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
    return std::make_shared<GenericCall>(tok,generic_types,identifier);
}