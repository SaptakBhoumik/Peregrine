#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

std::map<TokenType, Precedence_type> create_map() {
    std::map<TokenType, Precedence_type> precedence_map;

    precedence_map[tk_negative] = pr_prefix;
    precedence_map[tk_bit_not] = pr_prefix;
    precedence_map[tk_not] = pr_prefix;
    precedence_map[tk_and] = pr_and_or;
    precedence_map[tk_or] = pr_and_or;
    precedence_map[tk_not] = pr_not;
    precedence_map[tk_not_equal] = pr_compare;
    precedence_map[tk_greater] = pr_compare;
    precedence_map[tk_less] = pr_compare;
    precedence_map[tk_gr_or_equ] = pr_compare;
    precedence_map[tk_less_or_equ] = pr_compare;
    precedence_map[tk_plus] = pr_sum_minus;
    precedence_map[tk_minus] = pr_sum_minus;
    precedence_map[tk_multiply] = pr_mul_div;
    precedence_map[tk_divide] = pr_mul_div;

    return precedence_map;
}

Parser::Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {
    m_current_token = tokens[0];
}

Parser::~Parser() {}

void Parser::advance() {
    m_tk_index++;

    if (m_tk_index < m_tokens.size()) {
        m_current_token = m_tokens[m_tk_index];
    }
}

void Parser::advanceOnNewLine() {
    if (next().tk_type == tk_new_line) {
        advance();
    }
}

Token Parser::next() {
    Token token;

    if (m_tk_index + 1 < m_tokens.size()) {
        token = m_tokens[m_tk_index + 1];
    }

    return token;
}

Precedence_type Parser::next_precedence() {
    if (precedence_map.count(next().tk_type) > 0) {
        return precedence_map[next().tk_type];
    }

    return pr_lowest;
}

void Parser::error(Token tok, std::string_view msg) {
    PEError err = {{tok.line, tok.start, m_filename, tok.statement},
                   std::string(msg),
                   "",
                   "",
                   ""};

    m_errors.push_back(err);
}

bool Parser::expect(TokenType expected_type) {
    if (next().tk_type != expected_type) {
        error(next(), "expected token of type " +
                          std::to_string(expected_type) + ", got " +
                          std::to_string(next().tk_type) + " instead");
    }

    advance();
}

AstNodePtr Parser::parse() {
    std::vector<AstNodePtr> statements;

    while (m_current_token.tk_type != tk_eof) {
        statements.push_back(ParseStatement());
        advance();
    }
    if (!m_errors.empty()) {
        for (auto& err : m_errors) {
            display(err);
        }

        exit(1);
    }

    return std::make_shared<Program>(statements);
}

AstNodePtr Parser::ParseStatement() {
    AstNodePtr stmt;

    switch (m_current_token.tk_type) {
        case tk_const: {
            stmt = ParseConstDeclaration();
            break;
        }

        case tk_if: {
            stmt = ParseIf();
            break;
        }

        case tk_while: {
            stmt = ParseWhile();
            break;
        }

        case tk_def: {
            stmt = ParseFunctionDef();
            break;
        }

        case tk_break: {
            stmt = std::make_shared<BreakStatement>();
            advanceOnNewLine();
            break;
        }

        case tk_continue: {
            stmt = std::make_shared<ContinueStatement>();
            advanceOnNewLine();
            break;
        }

        case tk_return: {
            stmt = ParseReturn();
            break;
        }

        case tk_identifier: {
            if (next().tk_type == tk_identifier || next().tk_type == tk_assign) {
                // variable 
                stmt = ParseVariableStatement();
                break;
            }
            
            // if it got here, it will go down the cases and match the default case
            // DO NOT add another case below this one
        }

        default: {
            /*
                if it didn't match the statements above, then it must be
                either an expression or invalid
            */
        
            stmt = ParseExpression(pr_lowest);
            break;
        }
    }
    
    return stmt;
}

AstNodePtr Parser::ParseBlockStatement() {
    advance(); // when this is called, we are on the tk_ident token

    std::vector<AstNodePtr> statements;

    while (m_current_token.tk_type != tk_dedent) {
        if (m_current_token.tk_type == tk_eof) {
            error(m_current_token, "expected end of identation, got eof instead");
        }

        statements.push_back(ParseStatement());
        advance();
    }

    return std::make_shared<BlockStatement>(statements);
}

// TODO: make this invalid: int = 43
AstNodePtr Parser::ParseVariableStatement() {
    AstNodePtr var_type = std::make_shared<NoneLiteral>();

    if (next().tk_type == tk_identifier) {
        var_type = ParseIdentifier();
        advance();
    }

    AstNodePtr name = ParseIdentifier();

    AstNodePtr value = std::make_shared<NoneLiteral>();
    
    if (next().tk_type == tk_assign) {
        advance();
        advance();

        value = ParseExpression(pr_lowest);
    } else {
        advanceOnNewLine();
    }
    
    return std::make_shared<VariableStatement>(var_type, name, value);    
}

AstNodePtr Parser::ParseConstDeclaration() {
    expect(tk_identifier);

    AstNodePtr const_type = std::make_shared<NoneLiteral>();

    if (next().tk_type == tk_identifier) {
        const_type = ParseIdentifier();
        advance();
    }

    AstNodePtr name = ParseIdentifier();

    expect(tk_assign);
    advance();

    AstNodePtr value = ParseExpression(pr_lowest);

    return std::make_shared<ConstDeclaration>(const_type, name, value);
}

AstNodePtr Parser::ParseIf() {
    advance(); // skip the if token

    AstNodePtr condition = ParseExpression(pr_lowest);

    expect(tk_colon);

    //TODO: maybe support single-line ifs
    expect(tk_ident);

    AstNodePtr if_body = ParseBlockStatement();

    std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs;

    while (next().tk_type == tk_elif) {
        advance();
        advance();

        AstNodePtr condition = ParseExpression(pr_lowest);

        expect(tk_colon);
        expect(tk_ident);

        AstNodePtr body = ParseBlockStatement();

        elifs.push_back(std::pair(condition, body));
    }

    AstNodePtr else_body = std::make_shared<NoneLiteral>();

    if (next().tk_type == tk_else) {
        advance();

        expect(tk_colon);
        expect(tk_ident);

        else_body = ParseBlockStatement();
    }

    return std::make_shared<IfStatement>(condition, if_body, else_body, elifs);
}

AstNodePtr Parser::ParseWhile() {
    advance(); // skip the while token

    AstNodePtr condition = ParseExpression(pr_lowest);

    expect(tk_colon);
    expect(tk_ident);

    AstNodePtr body = ParseBlockStatement();

    return std::make_shared<WhileStatement>(condition, body);
}

AstNodePtr Parser::ParseFunctionDef() {
    expect(tk_identifier);

    AstNodePtr name = ParseIdentifier();

    expect(tk_l_paren);

    std::vector<parameter> parameters;

    do {
        advance();

        //TODO: make this a separate function
        if (m_current_token.tk_type != tk_identifier) {
            error(next(), "expected token of type " +
                          std::to_string(tk_identifier) + ", got " +
                          std::to_string(m_current_token.tk_type) + " instead");
        }

        AstNodePtr param_type = ParseIdentifier();
        expect(tk_identifier);
        AstNodePtr param_name = ParseIdentifier();

        parameters.push_back(parameter {param_type, param_name});
        advance();
    } while (m_current_token.tk_type == tk_comma);

    if (m_current_token.tk_type != tk_r_paren) {
        error(m_current_token, "expected ), got " + m_current_token.keyword + " instead");
    }

    // returns None by default
    AstNodePtr return_type = std::make_shared<IdentifierExpression>("None");

    if (next().tk_type == tk_arrow) {
        advance();
        expect(tk_identifier);

        return_type = ParseIdentifier();
    }

    expect(tk_colon);
    expect(tk_ident);

    AstNodePtr body = ParseBlockStatement();

    return std::make_shared<FunctionDefinition>(return_type, name, parameters, body);
}

AstNodePtr Parser::ParseReturn() {
    AstNodePtr return_value = std::make_shared<NoneLiteral>();

    if (next().tk_type != tk_new_line) {
        advance();
        return_value = ParseExpression(pr_lowest);
    } else {
        advance();
    }

    return std::make_shared<ReturnStatement>(return_value);
}

AstNodePtr Parser::ParseExpression(Precedence_type curr_precedence) {
    AstNodePtr left;

    switch (m_current_token.tk_type) {
        case tk_integer: {
            left = ParseInteger();
            break;
        }

        case tk_decimal: {
            left = ParseDecimal();
            break;
        }

        case tk_none: {
            left = ParseNone();
            break;
        }
        case tk_format:{
            advance();//making it a string
            left = ParseString(true,false);
            break;
        }
        case tk_raw:{
            advance();//making it a string
            left = ParseString(false,true);
            break;
        }
        case tk_string: {
            left = ParseString(false,false);
            break;
        }

        case tk_true:
        case tk_false: {
            left = ParseBool();
            break;
        }

        case tk_identifier: {
            left = ParseIdentifier();
            break;
        }

        case tk_l_paren: {
            left = ParseGroupedExpr();
            break;
        }

        case tk_negative:
        case tk_not:
        case tk_bit_not: {
            left = ParsePrefixExpression();
            break;
        }
       
        default: {
            error(m_current_token,
                  m_current_token.keyword + " is not an expression");
            break;
        }
    }

    while (next_precedence() > curr_precedence) {
        advance();
        left = ParseBinaryOperation(left);
    }

    advanceOnNewLine();

    return left;
}

AstNodePtr Parser::ParseBinaryOperation(AstNodePtr left) {
    std::string op = m_current_token.keyword;
    Precedence_type precedence = precedence_map[m_current_token.tk_type];

    advance();
    AstNodePtr right = ParseExpression(precedence);
    return std::make_shared<BinaryOperation>(left, op, right);
}

AstNodePtr Parser::ParsePrefixExpression() {
    std::string prefix = m_current_token.keyword;
    Precedence_type precedence = precedence_map[m_current_token.tk_type];

    advance();

    AstNodePtr right =ParseExpression(precedence);

    return std::make_shared<PrefixExpression>(prefix, right);
}

AstNodePtr Parser::ParseGroupedExpr() {
    advance();

    AstNodePtr expr =  ParseExpression(pr_lowest);

    expect(tk_r_paren);

    return expr;
}

AstNodePtr Parser::ParseIdentifier() {
    return std::make_shared<IdentifierExpression>(m_current_token.keyword);
}