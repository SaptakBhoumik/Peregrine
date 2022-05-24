#include "lexer.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#define not_tab()   m_is_tab=false;


LEXER::LEXER(std::string input, std::string filename){
    m_input = input;
    m_filename = filename;
    m_statments=split_ln(input);
    if(m_input.size()>0){
        m_curr_item=m_input[0];
        m_curr_line=m_statments[0];
        lex();
    }
    complete_it();
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
                        type,
                        m_tab_count
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
void LEXER::complete_it(){
    auto result=m_result;
    m_result.clear();
    size_t previous_ident=0;
    size_t current_ident=0;
    for(auto item:result){
        current_ident=item.tab;
        if(current_ident>previous_ident){
            m_result.push_back(Token{
                item.location,
                item.statement,
                "<ident>",
                item.start,
                item.end,
                item.line,
                tk_ident,
            });
            m_tabs.push_back(item.tab);
        }
        else if(current_ident<previous_ident){
            while (current_ident < previous_ident) {
                m_tabs.pop_back();
                m_result.push_back(Token{
                    item.location,
                    item.statement,
                    "<dedent>",
                    item.start,
                    item.end,
                    item.line,
                    tk_dedent,
                });
                if (m_tabs.size() != 0) {
                    if (current_ident >=m_tabs.back()) {
                        break;
                    }
                    previous_ident = m_tabs.back();
                } else {
                    previous_ident = 0;
                }
            }    
        }
        m_result.push_back(item);
        previous_ident=current_ident;
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
                tk_new_line,
                m_tab_count
            });
        }
    }
    auto item=m_result.back();
    for(size_t i=0;i<m_tabs.size();++i){
        m_result.push_back(Token{
                item.location,
                item.statement,
                "<dedent>",
                item.start,
                item.end,
                item.line,
                tk_dedent,
        });
    }
    m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "<tk_eof>",
            m_curr_index,
            m_curr_index+1,
            m_line,
            tk_eof,
            m_tab_count
    });
    
}

void LEXER::lex(){
    while (true){
        switch(m_curr_item){
            case '#':{
                add_unknown();
                while(next()!='\n' && next()!='\0' && next()!='\r'){
                    if(!advance()){
                        break;
                    }
                }
                break;
            }
            case '\"':
            case '\'':{
                not_tab();
                add_unknown();
                if(m_result.size()>0){
                    if(m_result.back().tkType==tk_format){
                        lex_formated_string();
                    }
                    else{
                        lex_string();
                    }
                }
                else{
                    lex_string();
                }
                break;
            }
            case '(':{
                not_tab();
                add_unknown();
                m_first_bracket_count++;
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_l_paren,
                            m_tab_count
                });
                break;
            }
            case '{':{
                not_tab();
                add_unknown();
                m_second_bracket_count++;
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_dict_open,
                            m_tab_count
                });
                break;
            }
            case '[':{
                not_tab();
                add_unknown();
                m_third_bracket_count++;
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_list_open,
                            m_tab_count
                });
                break;
            }
            case ')':{
                not_tab();
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
                            tk_r_paren,
                            m_tab_count
                });
                break;
            }
            case '}':{
                not_tab();
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
                            tk_dict_close,
                            m_tab_count
                });
                break;
            }
            case ']':{
                not_tab();
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
                            tk_list_close,
                            m_tab_count
                });
                break;
            }
            case '.':{
                not_tab();
                lex_dot();
                break;
            }
            case '+':{
                not_tab();
                add_unknown();
                lex_plus();
                break;
            }
            case '>':{
                not_tab();
                add_unknown();
                lex_greater();
                break;
            }
            case '<':{
                not_tab();
                add_unknown();
                lex_less();
                break;
            }
            case '-':{
                not_tab();
                add_unknown();
                lex_minus();
                break;
            }
            case '=':{
                not_tab();
                add_unknown();
                lex_equal();
                break;
            }
            case '/':{
                not_tab();
                add_unknown();
                lex_slash();
                break;
            }
            case '^':{
                not_tab();
                add_unknown();
                lex_caret();
                break;
            }
            case '%':{
                not_tab();
                add_unknown();
                lex_percent();
                break;
            }
            case '&':{
                not_tab();
                add_unknown();
                lex_ampersand();
                break;
            }
            case '|':{
                not_tab();
                add_unknown();
                lex_pipeline();
                break;
            }
            case '*':{
                not_tab();
                add_unknown();
                lex_star();
                break;
            }
            case '~':{
                not_tab();
                add_unknown();
                lex_tilde();
                break;
            }
            case '$':{
                not_tab();
                add_unknown();
                lex_dollar();
                break;
            }
            case '!':{
                not_tab();
                add_unknown();
                lex_bang();
                break;
            }
            case ':':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_colon,
                            m_tab_count
                });
                break;
            }
            case ',':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_comma,
                            m_tab_count
                });
                break;
            }
            case '@':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_at,
                            m_tab_count
                });
                break;
            }
            case ' ':{
                add_unknown();
                if(m_is_tab&&
                    m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_tab_count++;
                }
                break;
            }
            case '\t':{
                if(m_is_tab&&
                    m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_tab_count+=8;
                }
                add_unknown();
                break;
            }
            case '\n':{
                add_unknown();
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
                            tk_new_line,
                            m_tab_count
                        });
                    }
                }
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-1];
                if(m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_is_tab=true;
                    m_tab_count=0;
                }
                break;
            }
            case '\r':{
                add_unknown();
                if (next()=='\n'){// \r\n
                    advance();
                }
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
                            tk_new_line,
                            m_tab_count
                        });
                    }
                }
                if(m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_is_tab=true;
                    m_tab_count=0;
                }
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-1];
                break;
            }
            default:{
                not_tab();
                m_keyword+=m_curr_item;
                break;
            }
        }
        if(!advance()){
            break;
        }
        if(m_is_formated_str&&m_curr_item=='}'){
            add_unknown();
            break;
        }
    }
}

void LEXER::lex_dollar(){
    m_result.push_back(Token{
        m_loc,
        m_curr_line,
        std::string(1,m_curr_item),
        m_curr_index,
        m_curr_index+1,
        m_line,
        tk_dollar,
        m_tab_count
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
                    tk_bit_not,
                    m_tab_count
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
                    tk_bit_and_equal,
                    m_tab_count
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
                    tk_ampersand,
                     m_tab_count
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
                    tk_not_equal,
                    m_tab_count
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
                    tk_mod_equal,
                    m_tab_count
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
                    tk_modulo,
                    m_tab_count
        });
    }
}
void LEXER::lex_string(){
    char quote=m_curr_item;
    auto loc=m_loc;
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
        if(loc>=m_loc && m_is_tab){
            if(m_curr_item!=' '&&m_curr_item!='\t'){
                str.push_back(m_curr_item);
                m_is_tab=false;
            }
        }
        else{
            str.push_back(m_curr_item);
        }
        if(m_curr_item=='\n'){
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
            m_is_tab=true;
        }
        else if(m_curr_item=='\r'){
            if (next()=='\n'){// \r\n
                advance();
                str.push_back(m_curr_item);
            }
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
            m_is_tab=true;
        }
        redo:{}
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
        if(m_curr_item==quote && str.size()>0){
            if(str.back()=='\\'){
                str.push_back(m_curr_item);
                goto redo;
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
                tk_string,
                m_tab_count
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
            tk_increment,
            m_tab_count
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
            tk_plus_equal,
            m_tab_count
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
            tk_plus,
            m_tab_count
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
            tk_decrement,
            m_tab_count
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
            tk_minus_equal,
            m_tab_count
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
            tk_arrow,
            m_tab_count
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
            tk_minus,
            m_tab_count
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
            tk_gr_or_equ,
            m_tab_count
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
                tk_shift_right_equal,
                m_tab_count
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
                tk_shift_right,
                m_tab_count
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
            tk_greater,
            m_tab_count
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
            tk_less_or_equ,
            m_tab_count
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
                tk_shift_left_equal,
                m_tab_count
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
                tk_shift_left,
                m_tab_count
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
            tk_less,
            m_tab_count
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
            tk_equal,
            m_tab_count
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
            tk_assign,
            m_tab_count
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
            tk_slash_equal,
            m_tab_count
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
                tk_floor_equal,
                m_tab_count
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
                tk_floor,
                m_tab_count
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
            tk_divide,
            m_tab_count
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
            tk_bit_xor_equal,
            m_tab_count
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
            tk_xor,
            m_tab_count
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
                    tk_bit_or_equal,
                    m_tab_count
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
                    tk_pipeline,
                    m_tab_count
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
                    tk_bit_or,
                    m_tab_count
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
                    tk_ellipses,
                    m_tab_count
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
                tk_double_dot,
                m_tab_count
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
            tk_dot,
            m_tab_count
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
            tk_multiply,
            m_tab_count
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
            tk_times_equal,
            m_tab_count
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
                tk_exponent_equal,
                m_tab_count
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
                tk_exponent,
                m_tab_count
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
            tk_multiply,
            m_tab_count
        });
    }
}

#define _show_error()   if(!advance()){\
                            m_error.push_back(PEError({.loc = Location({.line = m_line,\
                                                            .col = m_loc,\
                                                            .loc=m_loc,\
                                                            .file = m_filename,\
                                                            .code = m_curr_line}),\
                                            .msg = "Unexpected end of file",\
                                            .submsg = temp,\
                                            .ecode = "e1"}));\
                            break;\
                        }

void LEXER::lex_formated_string(){
    auto x=m_is_formated_str;
    m_is_formated_str=true;
    char quote=m_curr_item;
    auto loc=m_loc;
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
    bool has_value=false;//to check if the str is empty or not
    while(m_curr_item!=quote){
        if(loc>=m_loc && m_is_tab){
            if(m_curr_item!=' '&&m_curr_item!='\t'){
                str.push_back(m_curr_item);
                m_is_tab=false;
            }
        }
        else{
            str.push_back(m_curr_item);
        }
        if(m_curr_item=='\n'){
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
            m_is_tab=true;
        }
        else if(m_curr_item=='\r'){
            if (next()=='\n'){// \r\n
                advance();
                str.push_back(m_curr_item);
            }
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
            m_is_tab=true;
        }
        redo:{}
        _show_error();
        if(m_curr_item==quote && str.size()>0){
            if(str.back()=='\\'){
                str.push_back(m_curr_item);
                goto redo;
            }
        }
        else if(m_curr_item=='{'&& str.size()>0){
            if(str.back()!='\\'){
                has_value=true;
                m_result.push_back(Token{
                    m_loc,
                    m_curr_line,
                    str,
                    start_index,
                    m_curr_index+1,
                    m_line,
                    tk_format_str,
                    m_tab_count
                });
                str.clear();
                _show_error();
                lex();
                _show_error();
            }
        }
    }
    if(str!=""||!has_value){
        m_result.push_back(Token{
                m_loc,
                m_curr_line,
                str,
                start_index,
                m_curr_index+1,
                m_line,
                tk_format_str,
                m_tab_count
        });
    }
    m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "",
            start_index,
            m_curr_index+1,
            m_line,
            tk_format_str_stopper,
            m_tab_count
    });
    m_is_formated_str=x;
}

