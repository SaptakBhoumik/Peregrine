#include "lexer.hpp"
#include "tokens.hpp"
#include <iostream>
#include <string>
#include <vector>
std::string next(INTEGER index, std::vector<std::string> code) {
  std::string next_item;
  if (index != code.size()) {
    next_item = code[index + 1];
  }
  return next_item;
}
std::vector<std::string> split(std::string code) {
  std::vector<std::string> split_code;
  std::string character;
  INTEGER index = 0;
  //reserving a the number of charecters for vector because this function literally breaks down the code into a vector of charecter
  //vectors copy and move value to a new location whenever we exceed that limit which makes it slow. to prevent this we are reserving space 
  //i am using vector because it is much plesent to work with
  split_code.reserve(code.size());
  while (index < code.length()) {
    character = code[index];
    split_code.emplace_back(character);
    index++;
  }
  return split_code;
}

Token token_init(std::string keyword, TokenType tk_type, INTEGER start,
                 INTEGER end, INTEGER line) {
  return Token{.keyword = keyword,
               .start = start,
               .end = end,
               .line = line,
               .tk_type = tk_type};
}
TokenType token_type(std::string keyword){
  TokenType result=tk_identifier;
  return result;
}
TokenType is_number(std::string keyword){
  TokenType result=tk_identifier;
  return result;
}
std::vector<Token> lexer(std::vector<std::string> charecters) {
  Token token;
  std::vector<Token> tokens;
  std::vector<INTEGER> identation_level;
  std::vector<std::string> operators{"!",  "/",  "//", "+", "-", "*", "%",
                                     "<<", ">>", "&",  "|", "^", "="};
  std::string string_starter;
  std::string statement;
  std::string item;
  std::string keyword;
  INTEGER start_index = 0;
  INTEGER previous_identation = 0;
  INTEGER current_index = 0;
  INTEGER second_bracket_count = 0;
  INTEGER third_bracket_count = 0;
  INTEGER line = 1;
  INTEGER curr_identation_level = 0;
  INTEGER last_line = 0;
  INTEGER cpp_bracket_count = 0;
  bool is_list_dictionary_cpp_string = false;
  bool is_array = false;
  bool is_dictionary = false;
  bool is_cpp = false;
  bool is_comment = false;
  bool is_string = false;
  bool is_tab = true;
  tokens.reserve(charecters.size());//we are reserving extra space but he advantage is that it will reduce the time it takes to run
  while (current_index < charecters.size()) {
    item = charecters[current_index];
    if (item != "\n") {
      statement += item;
    } else {
      statement = "";
    }
    if (item == " " && is_tab == true) {
      curr_identation_level += 1;
    } else if (item != " " && is_tab == true && item != "\n") {
      is_tab = false;
      previous_identation = 0;

      if (identation_level.size() != 0) {
        previous_identation = identation_level[identation_level.size() - 1];
      }

      if (curr_identation_level > previous_identation) {
        token = token_init("", tk_ident, start_index, current_index, line);
        tokens.emplace_back(token);

        identation_level.emplace_back(curr_identation_level);
      }

      while (curr_identation_level < previous_identation) {
        identation_level.pop_back();
        token = token_init("", tk_dedent, start_index, current_index, line);
        tokens.emplace_back(token);

        if (identation_level.size() != 0) {
          if (curr_identation_level >=
              identation_level[identation_level.size() - 1]) {
            break;
          }
          previous_identation = identation_level[identation_level.size() - 1];
        } else {
          previous_identation = 0;
        }
      }
    }
    else if(item == "\n" || item == "\r\n"){
            last_line = current_index;
            line += 1;
            is_tab = true;
            curr_identation_level = 0;
    }
    if (is_cpp == true){
            keyword += item;
            if (item == "(" && is_list_dictionary_cpp_string == false){
                cpp_bracket_count += 1;
                }
            else if (item == "'" ||  item == "\""){
                if (is_list_dictionary_cpp_string == true && string_starter != item){
                  //do nothing
                }
                else if (item == string_starter && is_list_dictionary_cpp_string == true){
                    is_list_dictionary_cpp_string = false;
                    string_starter = "";
                }
                else{
                    string_starter = item;
                    is_list_dictionary_cpp_string = true;
                }
            }
            else if (item == ")" && is_list_dictionary_cpp_string == false){
                cpp_bracket_count -= 1;
                if (cpp_bracket_count == 0){
                    is_array = false;
                }
            }
        token = token_init(keyword,cpp, start_index, current_index, line);
                    
    }
    else if (item == "#" && is_string == false && is_list_dictionary_cpp_string == false && is_comment == false){
            is_comment = true;
    }
    else if (is_comment == true){
            if (item == "\n" || item == "\r\n"){
                is_comment = false;
            }
    }
    else if (is_string == true && string_starter != item){
            keyword += item;
    }

    else if (is_array == true){
            keyword += item;
            if (item == "[" && is_list_dictionary_cpp_string == false){
                second_bracket_count += 1;
            }
            else if (item == "'" || item == "\""){
                if (is_list_dictionary_cpp_string == true && string_starter != item){
                }
                else if (item == string_starter && is_list_dictionary_cpp_string == true){
                    is_list_dictionary_cpp_string = false;
                    string_starter = "";
                }
                else{
                    string_starter = item;
                    is_list_dictionary_cpp_string = true;
                }
            }
            else if (item == "]" && is_list_dictionary_cpp_string == false){
                second_bracket_count -= 1;
                if (second_bracket_count == 0){
                    is_array = false;
                    token = token_init(keyword,array, start_index, current_index, line);
                }
            }
    }

    else if (is_dictionary == true){
            keyword += item;
            if (item == "{" && is_list_dictionary_cpp_string == false){
                third_bracket_count += 1;
            }
            else if (item == "'" || item == "\""){
                if (is_list_dictionary_cpp_string == true && string_starter != item){
                }
                else if (item == string_starter && is_list_dictionary_cpp_string == true){
                    is_list_dictionary_cpp_string = false;
                    string_starter = "";
                }
                else{
                    string_starter = item;
                    is_list_dictionary_cpp_string = true;
                }
            }
            else if (item == "}" && is_list_dictionary_cpp_string == false){
                third_bracket_count -= 1;
                if (third_bracket_count == 0){
                    is_dictionary = false;
                    token = token_init(keyword,array, start_index, current_index, line);
                }
            }
    }
    else if ((item == " " && is_dictionary == false && is_array == false && is_string == false) || item == "\n" || item == "\r\n"){
            if (keyword != ""){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
            }
    }
    else if (item == "[" && is_dictionary == false && is_string == false && is_array == false){
            if (keyword != ""){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index;
            keyword = item;
            is_array = true;
            second_bracket_count += 1;
    }
    else if (item == "{" && is_dictionary == false && is_string == false && is_array == false){
            if (keyword != ""){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            start_index = current_index;
            keyword = item;
            is_dictionary = true;
            third_bracket_count += 1;
    }
    else if ((item == "'" || item == "\"") && is_array == false && is_dictionary == false){
            if (is_string == true && string_starter == item){
                is_string = false;
                keyword += item;
                token = token_init(keyword,tk_string, start_index, current_index, line);
                string_starter = "";
            }

            else{
                if (keyword != ""){
                  token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                    tokens.emplace_back(token);
                    token = Token();
                    keyword = "";
                }
                is_string = true;
                start_index = current_index;
                keyword = item;
                string_starter = item;
            }
    }
    else if (item == "("){
            if (keyword != ""){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (tokens.back().tk_type == tk_cppcode){
                is_cpp = true;
                cpp_bracket_count = 1;
                keyword = item;
            }
            else{
                keyword = item;
                token = token_init(keyword,tk_l_paren, start_index, current_index, line);
            }
    }
    else if (item == "."){
            if ( keyword == ""){
                keyword = item;
                start_index = current_index;
                token = token_init(keyword,tk_dot, start_index, current_index, line);
            }
            else{
                if (is_number(keyword) == tk_integer){
                    keyword += item;
                }
                else{
                    token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                    tokens.emplace_back(token);
                    token = Token();
                    keyword = "";
                    keyword = item;
                    start_index = current_index;
                    token = token_init(keyword,tk_dot, start_index, current_index, line);
                }
            }
    }
    else if (item == ")"){
            if (keyword != ""){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            keyword = item;
            token = token_init(keyword,tk_r_paren, start_index, current_index, line);
    }
    else if (item == "+"){
            if (keyword != "" && keyword != "+"){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == "+"){
                token = token_init("++",tk_increment, start_index, current_index, line);
            }
            else if (next(current_index,charecters) == "+" || next(current_index, charecters) == "="){
                keyword = item;
                start_index = current_index;
            }
            else{
                start_index = current_index;
                keyword=item;
                token = token_init(keyword,tk_plus, start_index, current_index, line);
            }
    }
    else if (item == ">"){
            if (keyword != "" && keyword != ">"){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == ">"){
                if (next(current_index, charecters) == "="){
                    keyword = ">>";
                }
                else{
                    token = token_init(">>",tk_shift_right, start_index, current_index, line);
                }
            }
            else if (keyword == "-"){
                token = token_init("->",tk_shift_right, start_index, current_index, line);
            }
            else if (next(current_index,charecters) == ">" || next(current_index, charecters) == "=" ){
                keyword = item;
                start_index = current_index;
            }
            else{
                start_index = current_index;
                keyword = item;
                token = token_init("->",tk_greater, start_index, current_index, line);
            }
    }
    else if (item == "<"){
            if (keyword != "" && keyword != "<"){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }

            if (keyword == "<"){
                if (next(current_index, charecters) == "="){
                    keyword = "<<";
                }
                else{
                token = token_init("<<",tk_shift_left, start_index, current_index, line);
                }
            }
            else if (next(current_index, charecters) == "<" || next(current_index, charecters) == "=" ){
                keyword = item;
                start_index = current_index;
            }
            else{
                start_index = current_index;
                keyword = item;
                token = token_init(keyword,tk_less, start_index, current_index, line);
            }
    }
    else if (item == "-"){
            if (keyword != "" && keyword != "-"){
                token = token_init(keyword,token_type(keyword), start_index, current_index, line);
                tokens.emplace_back(token);
                token = Token();
                keyword = "";
            }
            if (keyword == "-"){
                token = token_init("--",tk_decrement, start_index, current_index, line);
            }

            else if (next(current_index, charecters) == "-" || next(current_index,charecters) == ">" || next(current_index, charecters) == "="){
                keyword = item;
                start_index = current_index;
            }
            else{
                start_index = current_index;
                keyword = item;
                token = token_init(keyword,tk_minus, start_index, current_index, line);
            }
    }
    
    if (token.keyword != ""){
            tokens.emplace_back(token);
            token = Token();
            keyword = "";
    }
    current_index++;
  }
  return tokens;
}
int main() {
  std::vector<std::string> code;
  code = split("Code");
  lexer(code);
  return 0;
}
