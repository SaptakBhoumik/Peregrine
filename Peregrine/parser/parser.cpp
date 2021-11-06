#include "parser.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <map>
#include <vector>

std::map<TokenType, Precedence_type> create_map() {
    precedence precedence_map;

    precedence_map[tk_negative] = pr_prefix;
    precedence_map[tk_bit_not] = pr_prefix;
    precedence_map[tk_not] = pr_prefix;
    precedence_map[tk_and] = pr_and_or;
    precedence_map[tk_or] = pr_and_or;
    precedence_map[tk_not] = pr_not;
    precedence_map[tk_assign] = pr_assign_compare;
    precedence_map[tk_not_equal] = pr_assign_compare;
    precedence_map[tk_greater] = pr_assign_compare;
    precedence_map[tk_less] = pr_assign_compare;
    precedence_map[tk_gr_or_equ] = pr_assign_compare;
    precedence_map[tk_less_or_equ] = pr_assign_compare;
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
    tk_index++;

    if (tk_index < tokens.size()) {
        current_token = tokens[tk_index];
    }
}

Token Parser::next() {
    Token token;

    if (tk_index + 1 < tokens.size()) {
        token = tokens[tk_index + 1];
    }

    return token;
}

Precedence_type Parser::next_precedence() {
    if (precedence_map.count(next().tk_type) > 0) {
        return precedence_map[next().tk_type];
    }

    return pr_lowest;
}

bool Parser::expect(TokenType expected_type) {
    if (next().tk_type != expected_type) {
        // panic
    }

    advance();
}

AstNode Parser::parse() {
    std::vector<AstNode> statements;

    while (current_token.tk_type != tk_eof) {
        statements.push_back(parse_statement());
        advance(); 
    }

    return Program(statements);
}

AstNode Parser::ParseStatement() {

}


AstNode Parser::ParseExpression(Precedence_type curr_precedence) {
    AstNode left;

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

        case tk_string: {
            left = ParseString(false, false);
            break;
        }

        case tk_true: case tk_false: {
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

        case tk_negative: case tk_not: case tk_bit_not: {
            left = ParsePrefixExpression();
            break;
        }

        default: {
            // panic
            break;
        }
    }

    while (next_precedence() > curr_precedence) {
        advance();
        left = parseBinaryOperation(left);
    }
    
    return left;
}

AstNode Parser::ParseBinaryOperation(AstNode left) {
    std::string op = m_current_token.keyword;
    Precedence_type precedence = precedence_map[m_current_token.tk_type];

    advance();

    AstNode right = ParseExpression(precedence);

    return BinaryOperation(left, op, right);
}

AstNode Parser::ParsePrefixExpression() {
    AstNode res;
    auto infix = current_token;
    res.token = infix;
    res.kind = AST_INFIX;
    advance();
    static AstNode right;
    right = parseExpression(precidence_map[tk_type]);
    res.children.infix.child = &right;
    return res;
}

AstNode Parser::ParseGroupExpr() {
    advance();
    auto node = parseExpression(pr_lowest);
    return node;
}

AstNode Parser::ParseIdentifierExpr() {
    AstNode res;
    res.kind = AST_VAR;
    res.token = current_token;
    // TODO :- Check if a variable or something like that
    return res;
}