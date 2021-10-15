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
  while (index < code.length()) {
    character = code[index];
    split_code.push_back(character);
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
  bool is_tab = true;
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
        tokens.push_back(token);

        identation_level.push_back(curr_identation_level);
      }

      while (curr_identation_level < previous_identation) {
        identation_level.pop_back();
        token = token_init("", tk_dedent, start_index, current_index, line);
        tokens.push_back(token);

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
