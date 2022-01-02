#include "parser.hpp"
#include <iostream>
bool Parser::is_imported_var(){
    auto index=m_tokIndex;
    auto tokens = m_tokens;
    size_t brac_count=0;
    size_t list_brac_count=0;
    Token curr_tok;
    Token prev_tok;
    while (true){
        curr_tok=tokens[index];
        if (curr_tok.tkType==tk_identifier){
            if (prev_tok.tkType==tk_identifier){return false;}//it is the type
        }
        else if (curr_tok.tkType==tk_dot || curr_tok.tkType==tk_arrow){}
        else if (curr_tok.tkType==tk_l_paren){brac_count++;}
        else if (curr_tok.tkType==tk_r_paren){brac_count--;}
        else if (curr_tok.tkType==tk_list_open){list_brac_count++;}
        else if (curr_tok.tkType==tk_list_close){list_brac_count--;}
        else if(curr_tok.tkType==tk_assign){return true;}
        else if (brac_count==0 && list_brac_count==0){return false;}
        prev_tok=curr_tok;
        index++;
    }
}

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
    if (precedenceMap.count(next().tkType) > 0) {
        return precedenceMap[next().tkType];
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

void Parser::expect(TokenType expectedType) {
    if (next().tkType != expectedType) {
        error(next(), "expected token of type " + std::to_string(expectedType) +
                          ", got " + std::to_string(next().tkType) +
                          " instead");
    }

    advance();
}

std::map<TokenType, PrecedenceType> createMap() {
    std::map<TokenType, PrecedenceType> precedenceMap;

    precedenceMap[tk_ampersand] = pr_prefix;
    precedenceMap[tk_negative] = pr_prefix;
    precedenceMap[tk_bit_not] = pr_prefix;
    precedenceMap[tk_and] = pr_and_or;
    precedenceMap[tk_or] = pr_and_or;
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
    precedenceMap[tk_bit_and] = pr_bit_and;
    precedenceMap[tk_shift_left] = pr_bit_shift;
    precedenceMap[tk_shift_right] = pr_bit_shift;
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

    return precedenceMap;
}
