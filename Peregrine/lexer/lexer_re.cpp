#include "lexer_re.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
std::vector<std::string> split_ln(std::string code) {
    std::vector<std::string> split_code;
    std::string character;
    std::string prev;
    std::string temp;
    size_t index = 0;
    while (index < code.length()) {
        character = code.at(index);
        if (character == "\n" || character == "\r") {
            //To prevent counting \r\n as 2 diffrent lines
            if(prev=="\r" && character=="\n"){}
            else{
                split_code.push_back(temp);
                temp = "";
            }
        } else {
            temp += character;
        }
        index++;
        prev=character;
    }
    if (temp != "") {
        split_code.push_back(temp);
    }
    return split_code;
}
bool is_int(const std::string s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}


void LEXER::add_unknown(){
    TokenType type;
    std::map<std::string,TokenType> key_map={
        {"True",tk_true},
        {"False",tk_false},
        {"None",tk_none},
        {"import",tk_import},
        {"from",tk_from},
        {"const",tk_const},
        {"if",tk_if},
        {"type",tk_type},
        {"union",tk_union},
        {"scope",tk_scope},
        {"elif",tk_elif},
        {"else",tk_else},
        {"while",tk_while},
        {"for",tk_for},
        {"break",tk_break},
        {"assert",tk_assert},
        {"try",tk_try},
        {"except",tk_except},
        {"raise",tk_raise},
        {"with",tk_with},
        {"continue",tk_continue},
        {"match",tk_match},
        {"extern",tk_extern},
        {"cast",tk_cast},
        {"case",tk_case},
        {"default",tk_default},
        {"static",tk_static},
        {"def",tk_def},
        {"private",tk_private},
        {"pass",tk_pass},
        {"return",tk_return},
        {"as",tk_as},
        {"enum",tk_enum},
        {"and",tk_and},
        {"or",tk_or},
        {"not",tk_not},
        {"is",tk_is},
        {"in",tk_in},
        {"inline",tk_inline},
        {"virtual",tk_virtual},
        {"class",tk_class},
        {"export",tk_export},
        {"__asm__",tk_asm}
    };
    if(m_keyword=="f" && (m_curr_item=='"'||m_curr_item=='\'')){
        type=tk_format;
    }
    else if(m_keyword=="r" && (m_curr_item=='"'||m_curr_item=='\'')){
        type=tk_raw;
    }
    else if (key_map.count(m_keyword) > 0) {
        type = key_map[m_keyword];
    }
    else if(m_keyword!=""){
        if(is_int(m_keyword)){
            type=tk_integer;
        }
        else if(std::regex_match(m_keyword,std::regex(R"(^^\s*[-+]?((\d+(\.\d+)?)|(\d+\.)|(\.\d+))(e[-+]?\d+)?\s*$)"))){
            type=tk_decimal;
        }
        else{
            type=tk_identifier;
            if(!std::regex_match(m_keyword,std::regex("^[a-zA-Z_][a-zA-Z0-9_]*$"))){
                m_error.push_back(PEError(
                    PEError({.loc = Location({.line = m_line,
                                          .col = m_loc,
                                          .loc=m_loc,
                                          .file = m_filename,
                                          .code = m_curr_line}),
                         .msg = "Invalid identifier: "+m_keyword,
                         .ecode = ""})
                ));
            }
        }
    } 
    if(m_keyword!=""){
        auto res = Token{
                        m_loc,
                        m_curr_line,
                        m_keyword,
                        m_curr_index-m_keyword.length(),
                        m_curr_index+1,
                        m_line,
                        type
        };
        if(m_result.size()>0 && type==tk_in){
            if(m_result.back().tkType==tk_not){
                m_result[m_result.size()-1].keyword="not in";
                m_result[m_result.size()-1].tkType=tk_not_in;
                m_result[m_result.size()-1].end=m_curr_index+1;
                m_result[m_result.size()-1].location=m_loc;
            }
            else{
                m_result.push_back(res);
            }
        }
        else if(m_result.size()>0 && type==tk_not){
            if(m_result.back().tkType==tk_is){
                m_result[m_result.size()-1].keyword="is not";
                m_result[m_result.size()-1].tkType=tk_is_not;
                m_result[m_result.size()-1].end=m_curr_index+1;
                m_result[m_result.size()-1].location=m_loc;
            }
            else{
                m_result.push_back(res);
            }
        }
        else{
            m_result.push_back(res);
        }
    }
    m_keyword = "";
}
LEXER::LEXER(std::string input, std::string filename){
    m_input = input;
    m_filename = filename;
    m_statments=split_ln(input);
    if(m_input.size()>0){
        m_curr_item=m_input[0];
        m_curr_line=m_statments[m_line-1];
        lex();
    }
    if(m_keyword!=""){
        add_unknown();
    }
    if(m_first_bracket_count!=0){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unexpected end of file",
                 .submsg = "Expecting a ')'",
                 .ecode = "e1"})
        ));
    }
    else if(m_second_bracket_count!=0){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unexpected end of file",
                 .submsg = "Expecting a '}'",
                 .ecode = "e1"})
        ));
    }
    else if(m_third_bracket_count!=0){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unexpected end of file",
                 .submsg = "Expecting a ']'",
                 .ecode = "e1"})
        ));
    }
    if(m_error.size()>0){
        for (auto& x: m_error) {
            display(x);
        }
        exit(1);
    }
    if(m_result.size()>0){
        if(m_result.back().tkType!=tk_new_line
            && m_result.back().tkType!=tk_dedent
            ){
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "<tk_new_line>",
                m_curr_index,
                m_curr_index+1,
                m_line,
                tk_new_line
            });
        }
    }
    m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "<tk_eof>",
            m_curr_index,
            m_curr_index+1,
            m_line,
            tk_eof
    });
}

bool LEXER::advance(){
    if(m_curr_index<(m_input.size()-1)){
        m_curr_index++;
        m_loc++;
        m_curr_item=m_input[m_curr_index];
        return true;
    }
    else{
        return false;
    }
}
char LEXER::next(){
    if((m_curr_index+1)<m_input.size()){
        return m_input[m_curr_index+1];
    }
    else{
        return '\0';
    }
}
void LEXER::lex(){
    while (true){
        switch(m_curr_item){
            case '#':{
                add_unknown();
                while(m_curr_item!='\n' && m_curr_item!='\r'){
                    if(!advance()){
                        break;
                    }
                }
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-2];
                if(m_result.size()>0){
                    if (m_result.back().tkType!=tk_new_line && m_result.back().tkType!=tk_colon
                        && m_result.back().tkType!=tk_dedent
                        && m_first_bracket_count==0 
                        && m_second_bracket_count==0
                        && m_third_bracket_count==0){
                        m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            "<tk_new_line>",
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_new_line
                        });
                    }
                }
                break;
            }
            case '\"':
            case '\'':{
                add_unknown();
                lex_string();
                break;
            }
            case '(':{
                add_unknown();
                m_first_bracket_count++;
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_l_paren
                });
                break;
            }
            case '{':{
                add_unknown();
                m_second_bracket_count++;
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_dict_open
                });
                break;
            }
            case '[':{
                add_unknown();
                m_third_bracket_count++;
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_list_open
                });
                break;
            }
            case ')':{
                add_unknown();
                if(m_first_bracket_count==0){
                    m_error.push_back(PEError({.loc = Location({.line = m_line,
                                          .col = m_loc,
                                          .loc=m_loc,
                                          .file = m_filename,
                                          .code = m_curr_line}),
                         .msg = "')' without a previous '(' token"}));
                }
                else{
                    m_first_bracket_count--;
                }
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_r_paren
                });
                break;
            }
            case '}':{
                add_unknown();
                if(m_second_bracket_count==0){
                    m_error.push_back(PEError({.loc = Location({.line = m_line,
                                          .col = m_loc,
                                          .loc=m_loc,
                                          .file = m_filename,
                                          .code = m_curr_line}),
                         .msg = "'}' without a previous '{' token"}));
                }
                else{
                    m_second_bracket_count--;
                }
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_dict_close
                });
                break;
            }
            case ']':{
                add_unknown();
                if(m_third_bracket_count==0){
                    m_error.push_back(PEError({.loc = Location({.line = m_line,
                                          .col = m_loc,
                                          .loc=m_loc,
                                          .file = m_filename,
                                          .code = m_curr_line}),
                         .msg = "']' without a previous '[' token"}));
                }
                else{
                    m_third_bracket_count--;
                }
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_list_close
                });
                break;
            }
            case '.':{
                lex_dot();
                break;
            }
            case '+':{
                add_unknown();
                lex_plus();
                break;
            }
            case '>':{
                add_unknown();
                lex_greater();
                break;
            }
            case '<':{
                add_unknown();
                lex_less();
                break;
            }
            case '-':{
                add_unknown();
                lex_minus();
                break;
            }
            case '=':{
                add_unknown();
                lex_equal();
                break;
            }
            case '/':{
                add_unknown();
                lex_slash();
                break;
            }
            case '^':{
                add_unknown();
                lex_caret();
                break;
            }
            case '%':{
                add_unknown();
                lex_percent();
                break;
            }
            case '&':{
                add_unknown();
                lex_ampersand();
                break;
            }
            case '|':{
                add_unknown();
                lex_pipeline();
                break;
            }
            case '*':{
                add_unknown();
                lex_star();
                break;
            }
            case '~':{
                add_unknown();
                lex_tilde();
                break;
            }
            case '$':{
                add_unknown();
                lex_dollar();
                break;
            }
            case '!':{
                add_unknown();
                lex_bang();
                break;
            }
            case ':':{
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_colon
                });
                break;
            }
            case ',':{
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_comma
                });
                break;
            }
            case '@':{
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_at
                });
                break;
            }
            case ' ':{
                //TODO:Tabs
                add_unknown();
                break;
            }
            case '\t':{
                //TODO:Tabs
                add_unknown();
                break;
            }
            case '\n':{
                add_unknown();
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-2];
                if(m_result.size()>0){
                    if (m_result.back().tkType!=tk_new_line && m_result.back().tkType!=tk_colon
                        && m_result.back().tkType!=tk_dedent
                        && m_first_bracket_count==0 
                        && m_second_bracket_count==0
                        && m_third_bracket_count==0){
                        m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            "<tk_new_line>",
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_new_line
                        });
                    }
                }
                break;
            }
            case '\r':{
                add_unknown();
                if (next()=='\n'){// \r\n
                    advance();
                }
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-2];
                if(m_result.size()>0){
                    if (m_result.back().tkType!=tk_new_line && m_result.back().tkType!=tk_colon
                        && m_result.back().tkType!=tk_dedent
                        && m_first_bracket_count==0 
                        && m_second_bracket_count==0
                        && m_third_bracket_count==0){
                        m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            "<tk_new_line>",
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_new_line
                        });
                    }
                }
                break;
            }
            default:{
                m_keyword+=m_curr_item;
            }
        }
        if(!advance()){
            break;
        }
    }
}
void LEXER::lex_space(){
}
void LEXER::add_dedent(){
    
}
void LEXER::lex_dollar(){
    m_result.push_back(Token{
        m_loc,
        m_curr_line,
        std::string(1,m_curr_item),
        m_curr_index,
        m_curr_index+1,
        m_line,
        tk_dollar
    });
}
void LEXER::lex_tilde(){
    m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    std::string(1,m_curr_item),
                    m_curr_index,
                    m_curr_index+1,
                    m_line,
                    tk_bit_not
        });
}
void LEXER::lex_ampersand(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    "&=",
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_bit_and_equal
        });
    }
    else{
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    std::string(1,m_curr_item),
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_ampersand
        });
    }
}
void LEXER::lex_bang(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    "!=",
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_not_equal
        });
    }
    else{
        m_error.push_back(PEError({.loc = Location({.line = m_line,
                                            .col = m_loc,
                                            .loc=m_loc,
                                            .file = m_filename,
                                            .code = m_curr_line}),
                            .msg = "Error: Expected '=' after '!'"
                            }));
    }
}
void LEXER::lex_percent(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    "%=",
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_mod_equal
        });
    }
    else{
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    std::string(1,m_curr_item),
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_modulo
        });
    }
}
void LEXER::lex_string(){
    char quote=m_curr_item;
    size_t start_index=m_curr_index+1;
    std::string temp="Expecting a ";
    temp.push_back(quote);
    std::string str="" ;
    if(!advance()){
        m_error.push_back(PEError({.loc = Location({.line = m_line,
                                          .col = m_loc,
                                          .loc=m_loc,
                                          .file = m_filename,
                                          .code = m_curr_line}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
    }
    while(m_curr_item!=quote){
        str.push_back(m_curr_item);
        if(m_curr_item=='\n'){
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
        }
        else if(m_curr_item=='\r'){
            if (next()=='\n'){// \r\n
                advance();
            }
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
        }
        if(!advance()){
            m_error.push_back(PEError({.loc = Location({.line = m_line,
                                            .col = m_loc,
                                            .loc=m_loc,
                                            .file = m_filename,
                                            .code = m_curr_line}),
                            .msg = "Unexpected end of file",
                            .submsg = temp,
                            .ecode = "e1"}));
            break;
        }
        if(m_curr_item==quote){
            if(str.size()>0){
                if(str.back()=='\\'){
                    str.push_back(m_curr_item);
                    advance();
                }
            }
        }
    }
    m_result.push_back(Token{
                m_loc,
                m_curr_line,
                str,
                start_index,
                m_curr_index+1,
                m_line,
                tk_string
            });
}
void LEXER::lex_plus(){
    size_t start_index=m_curr_index;
    if(next()=='+'){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "++",
            start_index,
            m_curr_index+1,
            m_line,
            tk_increment
        });
    }
    else if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "+=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_plus_equal
        });
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_plus
        });
    }
}
void LEXER::lex_minus(){
    size_t start_index=m_curr_index;
    if(next()=='-'){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "--",
            start_index,
            m_curr_index+1,
            m_line,
            tk_decrement
        });
    }
    else if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "-=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_minus_equal
        });
    }
    else if(next()=='>'){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "->",
            start_index,
            m_curr_index+1,
            m_line,
            tk_arrow
        });
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_minus
        });
    }
}
void LEXER::lex_greater(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            ">=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_gr_or_equ
        });
    }
    else if(next()=='>'){
        advance();
        if(next()=='='){
            advance();
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                ">>=",
                start_index,
                m_curr_index+1,
                m_line,
                tk_shift_right_equal
            });
        }
        else{
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                ">>",
                start_index,
                m_curr_index+1,
                m_line,
                tk_shift_right
            });
        }
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_greater
        });
    }
}
void LEXER::lex_less(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "<=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_less_or_equ
        });
    }
    else if(next()=='<'){
        advance();
        if(next()=='='){
            advance();
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "<<=",
                start_index,
                m_curr_index+1,
                m_line,
                tk_shift_left_equal
            });
        }
        else{
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "<<",
                start_index,
                m_curr_index+1,
                m_line,
                tk_shift_left
            });
        }
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_less
        });
    }
}
void LEXER::lex_equal(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "==",
            start_index,
            m_curr_index+1,
            m_line,
            tk_equal
        });
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_assign
        });
    }
}
void LEXER::lex_slash(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "/=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_slash_equal
        });
    }
    else if(next()=='/'){
        advance();
        if(next()=='='){
            advance();
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "//=",
                start_index,
                m_curr_index+1,
                m_line,
                tk_floor_equal
            });
        }
        else{
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "//",
                start_index,
                m_curr_index+1,
                m_line,
                tk_floor
            });
        }
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_divide
        });
    }
}
void LEXER::lex_caret(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "^=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_bit_xor_equal
        });
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_xor
        });
    }
}
void LEXER::lex_pipeline(){
    size_t start_index=m_curr_index;
    if(next()=='='){
        advance();
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    "|=",
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_bit_or_equal
        });
    }
    else if(next()=='>'){
        advance();
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    "|>",
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_pipeline
        });
    }
    else{
        m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    std::string(1,m_curr_item),
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_bit_or
        });
    }
}
void LEXER::lex_dot(){
    if(is_int(m_keyword) && std::isdigit(next())){
        m_keyword+=m_curr_item;
        return;
    }
    add_unknown();
    if(next()=='.'){
        advance(); 
        if (next()=='.'){
            advance();
            m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    "...",
                    m_curr_index-2,
                    m_curr_index+1,
                    m_line,
                    tk_ellipses
            });
        }
        else{
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "..",
                m_curr_index-1,
                m_curr_index+1,
                m_line,
                tk_double_dot
            });
        }               
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            m_curr_index,
            m_curr_index+1,
            m_line,
            tk_dot
        });
    }
}
void LEXER::lex_star(){
    size_t start_index=m_curr_index;
    bool is_ptr=true;
    if(m_result.size()>0){
        auto back=m_result.back().tkType;
        if((back == tk_true ||
            back == tk_false ||
            back == tk_string ||
            back == tk_integer ||
            back == tk_decimal ||
            back == tk_identifier ||
            back == tk_r_paren) &&
            m_line == m_result.back().line){
            is_ptr=false;
        }
    }
    if(is_ptr){
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_multiply
        });  
    }
    else if(next()=='='){
        advance();
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "*=",
            start_index,
            m_curr_index+1,
            m_line,
            tk_times_equal
        });
    }
    else if(next()=='*'){
        advance();
        if(next()=='='){
            advance();
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "**=",
                start_index,
                m_curr_index+1,
                m_line,
                tk_exponent_equal
            });
        }
        else{
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "**",
                start_index,
                m_curr_index+1,
                m_line,
                tk_exponent
            });
        }
    }
    else{
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            std::string(1,m_curr_item),
            start_index,
            m_curr_index+1,
            m_line,
            tk_multiply
        });
    }
}

LEXEME LEXER::result(){
    return m_result;
}
LEXEME lexer(std::string src, std::string filename){
    LEXER a(src, filename);
    return a.result();
}
int main(){
    auto a=lexer("some source def cast","");
    for (auto x:a){
        std::cout<<x.keyword<<" = "<<x.tkType<<"\n";
    }
}