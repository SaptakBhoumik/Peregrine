#ifndef PEREGRINE_LEXER_HPP
#define PEREGRINE_LEXER_HPP
#define LEXEME std::vector<Token>
#include "tokens.hpp"
#include <vector>
#include <string>
#include "../errors/error.hpp"
#define LEXEME std::vector<Token>
class LEXER{
    LEXEME m_result;
    size_t m_curr_index=0;
    std::string m_input;
    std::string m_filename;
    std::string m_keyword="";
    std::string m_curr_line;
    size_t m_line=1;
    size_t m_loc=0;
    std::vector<std::string> m_statments;
    std::vector<PEError> m_error;
    size_t m_first_bracket_count = 0;
    size_t m_second_bracket_count = 0;
    size_t m_third_bracket_count = 0;
    size_t m_curr_identation_level=0;
    char m_curr_item='\0';
    bool m_is_tab = true;

    void lex();
    char next();
    bool advance();

    void add_dedent();

    void lex_string();
    void lex_space();
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
    public:
    LEXER(std::string input, std::string filename);
    LEXEME result();
};

LEXEME lexer(std::string src, std::string filename);
#endif