#ifndef PEREGRINE_LEXER_HPP
#define PEREGRINE_LEXER_HPP
#define LEXEME std::vector<Token>
#include "tokens.hpp"
#include <vector>
#include <string>
#include "errors/error.hpp"
#define LEXEME std::vector<Token>
class LEXER{
    LEXEME m_result;
    size_t m_curr_index=0;
    std::string m_input;
    std::string m_filename;
    std::string m_keyword="";
    std::string m_curr_line;
    bool m_is_formated_str=false;
    size_t m_line=1;
    size_t m_loc=0;
    std::vector<std::string> m_statments;
    std::vector<PEError> m_error;
    size_t m_first_bracket_count = 0;
    size_t m_second_bracket_count = 0;
    size_t m_third_bracket_count = 0;
    char m_curr_item='\0';
    bool m_is_tab = true;
    size_t m_tab_count = 0;
    std::vector<size_t> m_tabs;

    void lex();

    char next();
    bool advance();
    std::vector<std::string> split_ln(std::string code);
    bool is_int(const std::string s);
    bool is_hex(const std::string s);
    

    void lex_string();
    void add_unknown();
    void lex_dollar();
    void lex_plus();
    void lex_star();
    void lex_tilde();
    void lex_bang();
    void lex_minus();
    void lex_slash();
    void lex_caret();
    void lex_percent();
    void lex_ampersand();
    void lex_pipeline();
    void lex_equal();
    void lex_greater();
    void lex_less();
    void lex_dot();
    void complete_it();
    void lex_formated_string();
    public:
    LEXER(std::string input, std::string filename);
    LEXEME result();
};

#endif