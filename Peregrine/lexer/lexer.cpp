//NOTE this is been rewritten. check lexer_re.cpp

#include "lexer.hpp"
#include "errors/error.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static inline std::string next(size_t index, std::string code) {
    std::string next_item;
    if (index + 1 < code.size()) {
        next_item = code.at(index + 1);
    } else {
        next_item = "";
    }
    return next_item;
}

std::vector<std::string> split(std::string code) {
    std::vector<std::string> split_code;
    std::string character;
    size_t index = 0;
    // reserving a the number of charecters for vector because this function
    // literally breaks down the code into a vector of charecter vectors copy
    // and move value to a new location whenever we exceed that limit which
    // makes it slow. to prevent this we are reserving space i am using vector
    // because it is much pleasent to work with
    split_code.reserve(code.size());
    while (index < code.length()) {
        character = code.at(index);
        split_code.emplace_back(character);
        index++;
    }
    return split_code;
}
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

static inline Token token_init(size_t loc,std::string statement, std::string keyword,
                               TokenType tkType, size_t start, size_t end,
                               size_t line) {
    return Token{.location=loc,
                .statement = statement,
                 .keyword = keyword,
                 .start = start,
                 .end = end,
                 .line = line,
                 .tkType = tkType};
}

static inline TokenType is_number(std::string keyword) {
    std::vector<std::string> split_key = split(keyword);
    const std::vector<std::string> numbers = {"0", "1", "2", "3", "4",
                                              "5", "6", "7", "8", "9"};
    if (std::count(numbers.begin(), numbers.end(), split(keyword)[0])) {

        if (std::count(split_key.begin(), split_key.end(), ".")) {
            return tk_decimal;
        } else {
            return tk_integer;
        }
    } else {
        return tk_identifier;
    }
}
static inline TokenType token_type(std::string item, std::string next_item) {
    if (item == "r" && (next_item == "\"" || next_item == "'")) {
        return tk_raw;
    } else if (item == "f" && (next_item == "\"" || next_item == "'")) {
        return tk_format;
    } else if (item == "True") {
        return tk_true;
    } else if (item == "False") {
        return tk_false;
    } else if (item == "None") {
        return tk_none;
    } else if (item == "import") {
        return tk_import;
    } else if (item == "from") {
        return tk_from;
    }  else if (item == "const") {
        return tk_const;
    } else if (item == "if") {
        return tk_if;
    } else if (item == "type") {
        return tk_type;
    } else if (item == "union") {
        return tk_union;
    } else if (item == "scope") {
        return tk_scope;
    } else if (item == "elif") {
        return tk_elif;
    } else if (item == "else") {
        return tk_else;
    } else if (item == "while") {
        return tk_while;
    } else if (item == "for") {
        return tk_for;
    }else if (item == "break") {
        return tk_break;
    } else if (item == "assert") {
        return tk_assert;
    } else if (item == "try") {
        return tk_try;
    } else if (item == "except") {
        return tk_except;
    } else if (item == "raise") {
        return tk_raise;
    } else if (item == "with") {
        return tk_with;
    } else if (item == "continue") {
        return tk_continue;
    } else if (item == "match") {
        return tk_match;
    } else if (item == "extern") {
        return tk_extern;
    } else if (item == "cast") {
        return tk_cast;
    } else if (item == "case") {
        return tk_case;
    } else if (item == "default") {
        return tk_default;
    } else if (item == "static") {
        return tk_static;
    } else if (item == "def") {
        return tk_def;
    } else if (item == "pass") {
        return tk_pass;
    } else if (item == "return") {
        return tk_return;
    } else if (item == "as") {
        return tk_as;
    }  else if (item == "enum") {
        return tk_enum;
    } else if (item == "and") {
        return tk_and;
    } else if (item == "or") {
        return tk_or;
    } else if (item == "not") {
        return tk_not;
    } else if (item == "is") {
        return tk_is;
    } else if (item == "in") {
        return tk_in;
    } else if (item == "inline") {
        return tk_inline;
    } else if (item == "virtual") {
        return tk_virtual;
    } else if (item == "class") {
        return tk_class;
    }else if (item == "export") {
        return tk_export;
    }else if (item == "private") {
        return tk_private;
    }else if (item == "__asm__") {
        return tk_asm;
    }else {
        return is_number(item);
    }
}
static inline TokenType equal(std::string keyword) {
    TokenType result = tk_identifier;
    if (keyword == "!=") {
        result = tk_not_equal;
    } else if (keyword == "/=") {
        result = tk_slash_equal;
    } else if (keyword == "//=") {
        result = tk_floor_equal;
    } else if (keyword == "+=") {
        result = tk_plus_equal;
    } else if (keyword == "-=") {
        result = tk_minus_equal;
    } else if (keyword == "*=") {
        result = tk_times_equal;
    } else if (keyword == "%=") {
        result = tk_mod_equal;
    } else if (keyword == "<<=") {
        result = tk_shift_left_equal;
    } else if (keyword == ">>=") {
        result = tk_shift_right_equal;
    } else if (keyword == "&=") {
        result = tk_bit_and_equal;
    } else if (keyword == "|=") {
        result = tk_bit_or_equal;
    } else if (keyword == "^=") {
        result = tk_bit_xor_equal;
    } else if (keyword == "<=") {
        result = tk_less_or_equ;
    } else if (keyword == ">=") {
        result = tk_gr_or_equ;
    }
    else if (keyword == "**=") {
        result = tk_exponent_equal;
    }
    return result;
}
LEXEME lexer(std::string src, std::string filename) {

    std::vector<Token> tokens;
    if (src == ""){
        tokens.push_back(token_init(0,"", "", tk_eof, 0, 0, 0));
        return tokens;
    }

    std::vector<std::string> seperate_lines(split_ln(src));
    Token token;
    std::vector<size_t> identation_level;
    const std::vector<std::string> operators({">", "<", "!", "/", "//", "+",
                                              "-", "*", "%", "<<", ">>", "&",
                                              "|", "^", "=","**"});
    std::string string_starter;
    std::string statement = seperate_lines.at(0);
    std::string item;
    std::string keyword;
    size_t start_index = 0;
    size_t previous_identation = 0;
    size_t current_index = 0;
    size_t second_bracket_count = 0;
    size_t third_bracket_count = 0;
    size_t line = 1;
    size_t last_line = 0;
    size_t curr_identation_level = 0;
    size_t cpp_bracket_count = 0;
    size_t first_bracket_count = 0;
    bool is_cpp_string = false;
    bool is_array = false;
    bool is_dictionary = false;
    bool is_cpp = false;
    bool is_comment = false;
    bool is_string = false;
    bool is_tab = true;

    tokens.reserve(
        src.size()); // we are reserving extra space but the advantage
                     // is that it will reduce the time it takes to run
    for (char temp : src) {
        item=temp;
        if (item == "\n" || item=="\r") {
            // if(current_index>0){
            // src.at(current_index-1);
            bool increment=false;
            if(current_index==0){increment=true;}
            else{
                char prev=src.at(current_index-1);
                //To prevent counting \r\n as 2 diffrent lines
                if(item=="\n" && prev=='\r'){}
                else{increment=true;}
            }
            if(increment){
                last_line = current_index;
                line++;
                if (seperate_lines.size() > line - 1) {
                    statement = seperate_lines.at(line - 1);
                }
            }
        }
        if (item == "(" && !is_comment && !is_string && !is_cpp_string) {
            first_bracket_count++;
        } else if (item == ")" && !is_comment && !is_string && !is_cpp_string) {
            first_bracket_count--;
        }
        if (is_array == false && is_dictionary == false && is_cpp == false &&
            is_string == false && first_bracket_count == 0) {
            if (item == " " && is_tab == true) {
                curr_identation_level += 1;
            }
            else if(item=="\t" && is_tab==true){curr_identation_level+=4;}
            
            else if ((item != " " || item!="\t")&& is_tab == true &&
                       !(item == "\n" || item == "\r")) {
                is_tab = false;
                previous_identation = 0;
                if (item != "#") {
                    if (identation_level.size() != 0) {
                        previous_identation =
                            identation_level.at(identation_level.size() - 1);
                    }
                    if (curr_identation_level > previous_identation) {
                        token = token_init(current_index - last_line,statement, "<ident>", tk_ident,
                                           current_index, current_index, line);
                        tokens.emplace_back(token);
                        token=Token();
                        identation_level.emplace_back(curr_identation_level);
                    }
                    while (curr_identation_level < previous_identation) {
                        identation_level.pop_back();
                        token = token_init(current_index - last_line,statement, "<dedent>", tk_dedent,
                                           current_index, current_index, line);
                        tokens.emplace_back(token);
                        token=Token();
                        if (identation_level.size() != 0) {
                            if (curr_identation_level >=
                                identation_level.at(identation_level.size() -
                                                    1)) {
                                break;
                            }
                            previous_identation = identation_level.at(
                                identation_level.size() - 1);
                        } else {
                            previous_identation = 0;
                        }
                    }
                }
            } else if (item == "\n" ||  item == "\r") {
                is_tab = true;
                curr_identation_level = 0;
            }
        }
        // lexing starts here
        if (is_cpp == true) {
            if (is_cpp_string == false && cpp_bracket_count == 1 &&
                item == ")") {
                // do nothing
            } else {
                keyword += item;
            }
            if (item == "(" && is_cpp_string == false) {
                cpp_bracket_count += 1;
            } else if (item == "'" || item == "\"") {
                if (is_cpp_string == true && string_starter != item) {
                    // do nothing
                } else if (item == string_starter && is_cpp_string == true) {
                    is_cpp_string = false;
                    string_starter = "";
                } else {
                    string_starter = item;
                    is_cpp_string = true;
                }
            } else if (item == ")" && is_cpp_string == false) {
                cpp_bracket_count -= 1;
                if (cpp_bracket_count == 0) {
                    is_cpp = false;
                    token = token_init(current_index - last_line,statement, keyword, tk_cpp, start_index,
                                       current_index, line);
                }
            }
        } else if (item == "#" && is_string == false &&
                   is_cpp_string == false && is_comment == false) {
            is_comment = true;
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
        } else if (is_comment == true) {
            if (item == "\n" ||  item == "\r") {
                is_comment = false;
                if (tokens.back().tkType !=tk_new_line && // we dont want to add 2 newline one after the other
                    tokens.back().tkType !=tk_colon && tokens.size()>0
                ){
                    token = token_init(current_index - last_line,statement, "<tk_new_line>", tk_new_line,
                                       current_index, current_index, line);
                    tokens.emplace_back(token);
                    token = Token();
                }
            }
        } else if (is_string == true && string_starter != item) {
            keyword += item;
        } else if (is_array == true && item == "]" && is_string == false) {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            second_bracket_count -= 1;
            token = token_init(current_index - last_line,statement, item, tk_list_close, start_index,
                               current_index, line);
            if (second_bracket_count == 0) {
                is_array = false;
            }
        } else if (is_dictionary == true && item == "}" && is_string == false) {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            third_bracket_count -= 1;
            token = token_init(current_index - last_line,statement, item, tk_dict_close,
                               current_index - 1, current_index, line);
            if (third_bracket_count == 0) {
                is_dictionary = false;
            }
        } else if ((item == " "||item == "\t") || item == "\n" || 
                   item == "\r") {
            if (keyword != "") {
                size_t i=0;
                auto x=current_index - last_line;
                if(item == "\n" || item == "\r"){
                    i=line-1;
                    x=seperate_lines[i-1].size()-keyword.size();
                }
                else{i=line;}
                token = token_init(x,
                    seperate_lines[i-1], keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, i);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            if ((item == "\n" ||  item == "\r") &&
                tokens.size() > 0 &&
                first_bracket_count ==0) { // we want the no of bracket count to be zero
                // because peregrine allows you to write code as follows
                // print(arg1,
                //       arg2)
                // to reduce the confusion while parsing
                if (tokens.back().tkType !=tk_new_line // we dont want to add 2 newline one after the other
                    && tokens.back().tkType !=tk_colon // again to reduce confusion
                    && is_dictionary == false && is_array == false &&
                    is_string == false) {
                    token = token_init(seperate_lines[line-2].size(),seperate_lines[line-2], "<tk_new_line>", tk_new_line,
                                       current_index, current_index, line-1);
                    tokens.emplace_back(token);
                    token = Token();
                }
            }
        } else if (item == "[" && is_string == false) {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            is_array = true;
            token = token_init(current_index - last_line,statement, item, tk_list_open, start_index,
                               current_index, line);
            second_bracket_count += 1;
        } else if (item == "{" && is_string == false) {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            is_dictionary = true;
            token = token_init(current_index - last_line,statement, item, tk_dict_open, start_index,
                               current_index, line);
            third_bracket_count += 1;
        } else if ((item == "'" || item == "\"")) {
            if (is_string == true && string_starter == item) {
                is_string = false;
                token = token_init(current_index - last_line,statement, keyword, tk_string, start_index,
                                   current_index, line);
                string_starter = "";
            } else {
                if (keyword != "") {
                    token = token_init(current_index - last_line,
                        statement, keyword,
                        token_type(keyword, next(current_index - 1, src)),
                        start_index, current_index - 1, line);
                    tokens.emplace_back(token);
                    token = Token();
                    keyword = "";
                }
                is_string = true;
                start_index = current_index;
                string_starter = item;
            }
        } else if (item == "(") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_l_paren, start_index,
                                   current_index, line);
        } else if (item == ".") {
            if (keyword == "") {
                if(next(current_index,src)=="."){
                    keyword=".";
                }
                else{
                    keyword = item;
                    start_index = current_index - 1;
                    if(tokens.back().tkType==tk_new_line){
                        tokens.pop_back();
                    }
                    token = token_init(current_index - last_line,statement, keyword, tk_dot, start_index,
                                    current_index, line);
                }
            } else {
                if (is_number(keyword) == tk_integer && next(current_index,src)!=".") {
                    keyword += item;
                }
                else if (next(current_index,src)=="."){
                    if(keyword=="."){
                        keyword="..";
                    }
                    else{
                        token = token_init(current_index - last_line,
                            statement, keyword,
                            token_type(keyword, next(current_index - 1, src)),
                            start_index, current_index - 1, line);
                        tokens.emplace_back(token);
                        token = Token();
                        keyword=".";
                        start_index = current_index;
                    }
                }
                else {
                    if (keyword=="."){
                        token = token_init(current_index - last_line,statement, "..", tk_double_dot, start_index,
                                       current_index, line);
                    }
                    else if (keyword==".."){
                        token = token_init(current_index - last_line,statement, "...", tk_ellipses, start_index,
                                       current_index, line);
                    }
                    else {
                        token = token_init(current_index - last_line,
                            statement, keyword,
                            token_type(keyword, next(current_index - 1, src)),
                            start_index, current_index - 1, line);
                        tokens.emplace_back(token);
                        token = Token();
                        keyword = item;
                        start_index = current_index - 1;
                        if(tokens.back().tkType==tk_new_line){
                            tokens.pop_back();
                        }
                        token = token_init(current_index - last_line,statement, keyword, tk_dot, start_index,
                                        current_index, line);
                    }
                }
            }
        } else if (item == ")") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            keyword = item;
            start_index = current_index - 1;
            token = token_init(current_index - last_line,statement, keyword, tk_r_paren, start_index,
                               current_index, line);
        } else if (item == "+") {
            if (keyword != "" && keyword != "+") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == "+") {
                token = token_init(current_index - last_line,statement, "++", tk_increment, start_index,
                                   current_index, line);
            } else if (next(current_index, src) == "+" ||
                       next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_plus, start_index,
                                   current_index, line);
            }
        } else if (item == ">") {
            if (keyword != "" && keyword != ">" && keyword != "|" && keyword != "-") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == ">") {
                if (next(current_index, src) == "=") {
                    keyword = ">>";
                } else {
                    token = token_init(current_index - last_line,statement, ">>", tk_shift_right,
                                       start_index, current_index, line);
                }
            } else if (keyword == "-") {
                if(tokens.back().tkType==tk_new_line){
                    tokens.pop_back();
                }
                token = token_init(current_index - last_line,statement, "->", tk_arrow, start_index,
                                   current_index, line);
            } else if (keyword == "|") {
                if(tokens.back().tkType==tk_new_line){
                    tokens.pop_back();
                }
                token = token_init(current_index - last_line,statement, "|>", tk_pipeline, start_index,
                                   current_index, line);
            } else if (next(current_index, src) == ">" ||
                       next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, ">", tk_greater, start_index,
                                   current_index, line);
            }
        } else if (item == "<") {
            if (keyword != "" && keyword != "<") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == "<") {
                if (next(current_index, src) == "=") {
                    keyword = "<<";
                } else {
                    token = token_init(current_index - last_line,statement, "<<", tk_shift_left,
                                       start_index, current_index, line);
                }
            } else if (next(current_index, src) == "<" ||
                       next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_less, start_index,
                                   current_index, line);
            }
        } else if (item == "-") {
            if (keyword != "" && keyword != "-") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            if (keyword == "-") {
                token = token_init(current_index - last_line,statement, "--", tk_decrement, start_index,
                                   current_index, line);
            } else if (next(current_index, src) == "-" ||
                       next(current_index, src) == ">" ||
                       next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                if (tokens.size() > 0) {
                    if ((tokens.back().tkType == tk_true ||
                         tokens.back().tkType == tk_false ||
                         tokens.back().tkType == tk_string ||
                         tokens.back().tkType == tk_integer ||
                         tokens.back().tkType == tk_decimal ||
                         tokens.back().tkType == tk_identifier ||
                         tokens.back().tkType == tk_r_paren) &&
                        line == tokens.back().line) {
                        token = token_init(current_index - last_line,statement, keyword, tk_minus,
                                           start_index, current_index, line);
                    } else {
                        token = token_init(current_index - last_line,statement, keyword, tk_minus,
                                           start_index, current_index, line);
                    }
                } else {
                    token = token_init(current_index - last_line,statement, keyword, tk_minus,
                                       start_index, current_index, line);
                }
            }
        } else if (item == "=") {
            if ((std::count(operators.begin(), operators.end(), keyword) ==
                 0) &&
                keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == "=") {
                keyword = "==";
                token = token_init(current_index - last_line,statement, keyword, tk_equal, start_index,
                                   current_index, line);
            } else if (next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else if (keyword == "" && next(current_index, src) != "=") {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_assign, start_index,
                                   current_index, line);
            } else {
                keyword += item;
                token = token_init(current_index - last_line,statement, keyword, equal(keyword),
                                   start_index, current_index, line);
            }
        } else if (item == "/") {
            if (keyword != "" && keyword != "/") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == "/") {
                if (next(current_index, src) == "=") {
                    keyword = "//";
                } else {
                    token = token_init(current_index - last_line,statement, "//", tk_floor, start_index,
                                       current_index, line);
                }
            } else if (next(current_index, src) == "/" ||
                       next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_divide, start_index,
                                   current_index, line);
            }
        } else if (item == "^") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_xor, start_index,
                                   current_index, line);
            }
        } else if (item == "%") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_modulo, start_index,
                                   current_index, line);
            }
        } else if (item == "&") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            if (next(current_index, src) == "=") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                if (tokens.size() > 0) {
                    if ((tokens.back().tkType == tk_true ||
                         tokens.back().tkType == tk_false ||
                         tokens.back().tkType == tk_string ||
                         tokens.back().tkType == tk_integer ||
                         tokens.back().tkType == tk_decimal ||
                         tokens.back().tkType == tk_identifier ||
                         tokens.back().tkType == tk_r_paren) &&
                        line == tokens.back().line) {
                        token = token_init(current_index - last_line,statement, keyword, tk_ampersand,
                                           start_index, current_index, line);
                    } else {
                        token = token_init(current_index - last_line,statement, keyword, tk_ampersand,
                                           start_index, current_index, line);
                    }
                } else {
                    token = token_init(current_index - last_line,statement, keyword, tk_ampersand,
                                       start_index, current_index, line);
                }
            }
        } else if (item == "|") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (next(current_index, src) == "="||next(current_index, src) == ">") {
                keyword = item;
                start_index = current_index;
            } else {
                start_index = current_index - 1;
                keyword = item;
                token = token_init(current_index - last_line,statement, keyword, tk_bit_or, start_index,
                                   current_index, line);
            }
        } else if (item == "*") {
            if (keyword != "" && keyword != "*") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            if ((next(current_index, src) == "*" ||
                 next(current_index, src) == "=") &&
                (tokens.back().tkType == tk_identifier ||
                 tokens.back().tkType == tk_decimal ||
                 tokens.back().tkType == tk_integer ||
                 next(current_index, src) == "=")) {
                keyword += "*";
                start_index = current_index;
            } else if (keyword == "*") {
                token = token_init(current_index - last_line,statement, "**", tk_exponent, start_index,
                                   current_index, line);
            } else {
                keyword="*";
                start_index = current_index - 1;
                token = token_init(current_index - last_line,statement, keyword, tk_multiply,
                                       start_index, current_index, line);
            }
        } else if (item == "~") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                if (token.tkType == tk_not &&
                    tokens[tokens.size() - 1].tkType == tk_is) {
                    tokens[tokens.size() - 1].tkType = tk_is_not;
                    tokens[tokens.size() - 1].keyword = "is not";
                } else if (token.tkType == tk_in &&
                           tokens[tokens.size() - 1].tkType == tk_not) {
                    tokens[tokens.size() - 1].tkType = tk_not_in;
                    tokens[tokens.size() - 1].keyword = "not in";
                } else {
                    tokens.emplace_back(token);
                }
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            token = token_init(current_index - last_line,statement, keyword, tk_bit_not, start_index,
                               current_index, line);
        } else if (item == "!") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index;
            keyword = item;
        } else if (item == ":") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            token = token_init(current_index - last_line,statement, keyword, tk_colon, start_index,
                               current_index, line);
        } else if (item == ",") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            token = token_init(current_index - last_line,statement, keyword, tk_comma, start_index,
                               current_index, line);
        } else if (item == "@") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            token = token_init(current_index - last_line,statement, keyword, tk_at, start_index,
                               current_index, line);
        }else if (item == "$") {
            if (keyword != "") {
                token = token_init(current_index - last_line,
                    statement, keyword,
                    token_type(keyword, next(current_index - 1, src)),
                    start_index, current_index - 1, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index - 1;
            keyword = item;
            token = token_init(current_index - last_line,statement, keyword, tk_dollar, start_index,
                               current_index, line);
        } 
        else {
            if (keyword == "") {
                start_index = current_index;
            }
            keyword += item;
        }
        if (token.keyword != "") {
            tokens.emplace_back(token);
            token = Token();
            keyword = "";
        }
        current_index++;
    }
    
    if (keyword != "") {
        tokens.emplace_back(
            token_init(current_index - last_line,statement, keyword,
                       token_type(keyword, next(current_index - 1, src)),
                       start_index, current_index, line));
    }
    bool error=false;
    if (is_string == true || is_cpp_string == true) {
        std::string temp = "Expecting a " + string_starter;
        display(PEError({.loc = Location({.line = line,
                                          .col = current_index,
                                          .loc=current_index,
                                          .file = filename,
                                          .code = statement}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
        error=true;
    }
    if (is_dictionary == true) {
        std::string temp = "Expecting a }";
        display(PEError({.loc = Location({.line = line,
                                          .col = current_index,
                                          .loc=current_index,
                                          .file = filename,
                                          .code = statement}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
        error=true;
    }
    if (is_array == true) {
        std::string temp = "Expecting a ]";
        display(PEError({.loc = Location({.line = line,
                                          .col = current_index,
                                          .loc=current_index,
                                          .file = filename,
                                          .code = statement}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
        error=true;
    }
    if (is_cpp == true) {
        std::string temp = "Expecting a )";
        display(PEError({.loc = Location({.line = line,
                                          .col = current_index,
                                          .loc=current_index,
                                          .file = filename,
                                          .code = statement}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
        error=true;
    } else if (first_bracket_count != 0) {
        std::string temp = "Expecting a )";
        display(PEError({.loc = Location({.line = line,
                                          .col = current_index,
                                          .loc=current_index,
                                          .file = filename,
                                          .code = statement}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
        error=true;
    }
    if (error){exit(1);}
    if (tokens.size() > 0) {
        size_t total_tab = 0;
        if(tokens.back().tkType!=tk_new_line){
            tokens.push_back(token_init(current_index - last_line,statement, "<tk_new_line>", tk_new_line,
                                       current_index, current_index, line));
        }
        for (auto& tok : tokens) {
            switch (tok.tkType) {
                case tk_ident: {
                    total_tab++;
                    break;
                }
                case tk_dedent: {
                    total_tab--;
                    break;
                }
                default: {
                }
            }
        }
        while (total_tab != 0) {
            token = token_init(current_index - last_line,statement, "", tk_dedent, current_index,
                               current_index, line);
            tokens.push_back(token);
            total_tab--;
        }
        tokens.push_back(
            token_init(current_index - last_line,"", "", tk_eof, current_index, current_index, line));
    }
    else{exit(1);}
    return tokens;
}
