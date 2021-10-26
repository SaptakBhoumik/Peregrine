#include "lexer.hpp"
#include "../errors/error.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static inline std::string next(uint64_t index, std::string code) {
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
  uint64_t index = 0;
  // reserving a the number of charecters for vector because this function
  // literally breaks down the code into a vector of charecter vectors copy and
  // move value to a new location whenever we exceed that limit which makes it
  // slow. to prevent this we are reserving space i am using vector because it
  // is much pleasent to work with
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
  std::string temp;
  uint64_t index = 0;
  while (index < code.length()) {
    character = code.at(index);
    if (character == "\n") {
      split_code.push_back(temp);
      temp = "";
    } else {
      temp += character;
    }
    index++;
  }
  if (temp != "") {
    split_code.push_back(temp);
  }
  return split_code;
}

static inline Token token_init(std::string statement, std::string keyword,
                               TokenType tk_type, uint64_t start, uint64_t end,
                               uint64_t line) {
  return Token{.statement = statement,
               .keyword = keyword,
               .start = start,
               .end = end,
               .line = line,
               .tk_type = tk_type};
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
static inline TokenType token_type(std::string item) {
  if (item == "r") {
    return tk_raw;
  } else if (item == "f") {
    return tk_format;
  } else if (item == "True") {
    return tk_true;
  } else if (item == "False") {
    return tk_false;
  } else if (item == "None") {
    return tk_none;
  } else if (item == "import") {
    return tk_import;
  } else if (item == "const") {
    return tk_const;
  } else if (item == "if") {
    return tk_if;
  } else if (item == "type") {
    return tk_type;
  } else if (item == "any") {
    return tk_any;
  } else if (item == "enum") {
    return tk_enum;
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
  } else if (item == "break") {
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
  } else if (item == "case") {
    return tk_case;
  } else if (item == "default") {
    return tk_default;
  } else if (item == "def") {
    return tk_def;
  } else if (item == "pass") {
    return tk_pass;
  } else if (item == "return") {
    return tk_return;
  } else if (item == "as") {
    return tk_as;
  }
  // This is not necessary because peregrine has special library for
  // multithreading
  // else if (item == "async") {
  //   return tk_async;
  // } else if (item == "await") {
  //   return tk_await;
  // }
  else if (item == "and") {
    return tk_and;
  } else if (item == "or") {
    return tk_or;
  } else if (item == "not") {
    return tk_not;
  } else if (item == "is") {
    return tk_is;
  } else if (item == "in") {
    return tk_in;
  } else if (item == "Cppcode") {
    return tk_cppcode;
  } else if (item == "himport") {
    return tk_himport;
  } else if (item == "cppimport") {
    return tk_cppimport;
  } else if (item == "class") {
    return tk_class;
  } else if (item == "struct") {
    return tk_struct;
  } else if (item == "dict") {
    return tk_dict;
  } else if (item == "flags") {
    return tk_flags;
  } else {
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
  }
  return result;
}
LEXEME lexer(std::string src, std::string filename) {
  std::vector<std::string> seperate_lines(split_ln(src));
  Token token;
  std::vector<Token> tokens;
  std::vector<uint64_t> identation_level;
  const std::vector<std::string> operators(
      {"!", "/", "//", "+", "-", "*", "%", "<<", ">>", "&", "|", "^", "="});
  std::string string_starter;
  std::string statement = seperate_lines.at(0);
  std::string item;
  std::string keyword;
  uint64_t start_index = 0;
  uint64_t previous_identation = 0;
  uint64_t current_index = 0;
  uint64_t second_bracket_count = 0;
  uint64_t third_bracket_count = 0;
  uint64_t line = 1;
  uint64_t last_line = 0;
  uint64_t curr_identation_level = 0;
  uint64_t cpp_bracket_count = 0;
  bool is_list_dictionary_cpp_string = false;
  bool is_array = false;
  bool is_dictionary = false;
  bool is_cpp = false;
  bool is_comment = false;
  bool is_string = false;
  bool is_tab = true;
  tokens.reserve(src.size()); // we are reserving extra space but the advantage
                              // is that it will reduce the time it takes to run
  while (current_index < src.size()) {
    item = src.at(current_index);
    if (item == "\n" || item == "\r\n") {
      last_line = current_index;
      line++;
      if (seperate_lines.size() > line - 1) {
        statement = seperate_lines.at(line - 1);
      }
    }
    if (is_comment == false && is_array == false && is_dictionary == false &&
        is_cpp == false && is_string == false) {
      if (item == " " && is_tab == true) {
        curr_identation_level += 1;
      } else if (item != " " && is_tab == true && item != "\n") {
        is_tab = false;
        if (item == "#") {
          curr_identation_level -= 4;
        } else {
          previous_identation = 0;
        }
        if (identation_level.size() != 0) {
          previous_identation =
              identation_level.at(identation_level.size() - 1);
        }
        if (curr_identation_level > previous_identation) {
          token = token_init(statement, "", tk_ident, current_index,
                             current_index, line);
          tokens.emplace_back(token);
          identation_level.emplace_back(curr_identation_level);
        }
        while (curr_identation_level < previous_identation) {
          identation_level.pop_back();
          token = token_init(statement, "", tk_dedent, current_index,
                             current_index, line);
          tokens.emplace_back(token);
          if (identation_level.size() != 0) {
            if (curr_identation_level >=
                identation_level.at(identation_level.size() - 1)) {
              break;
            }
            previous_identation =
                identation_level.at(identation_level.size() - 1);
          } else {
            previous_identation = 0;
          }
        }
      } else if (item == "\n" || item == "\r\n") {
        is_tab = true;
        curr_identation_level = 0;
      }
    }
    // lexing starts here
    if (is_cpp == true) {
      keyword += item;
      if (item == "(" && is_list_dictionary_cpp_string == false) {
        cpp_bracket_count += 1;
      } else if (item == "'" || item == "\"") {
        if (is_list_dictionary_cpp_string == true && string_starter != item) {
          // do nothing
        } else if (item == string_starter &&
                   is_list_dictionary_cpp_string == true) {
          is_list_dictionary_cpp_string = false;
          string_starter = "";
        } else {
          string_starter = item;
          is_list_dictionary_cpp_string = true;
        }
      } else if (item == ")" && is_list_dictionary_cpp_string == false) {
        cpp_bracket_count -= 1;
        if (cpp_bracket_count == 0) {
          is_cpp = false;
          token = token_init(statement, keyword, cpp, start_index,
                             current_index, line);
        }
      }
    } else if (item == "#" && is_string == false &&
               is_list_dictionary_cpp_string == false && is_comment == false) {
      is_comment = true;
    } else if (is_comment == true) {
      if (item == "\n" || item == "\r\n") {
        is_comment = false;
      }
    } else if (is_string == true && string_starter != item) {
      if (item == "\"" && string_starter == "'") {
        keyword += R"(\")";
      } else {
        keyword += item;
      }
    } else if (is_array == true) {
      if (item == "'" &&
          (is_list_dictionary_cpp_string == false || item == string_starter)) {
        keyword += "\"";
      } else if (item == "\"" && item != string_starter &&
                 is_list_dictionary_cpp_string == true) {
        keyword += R"(\")";
      } else {
        keyword += item;
      }
      if (item == "[" && is_list_dictionary_cpp_string == false) {
        second_bracket_count += 1;
      } else if (item == "'" || item == "\"") {
        if (is_list_dictionary_cpp_string == true && string_starter != item) {
        } else if (item == string_starter &&
                   is_list_dictionary_cpp_string == true) {
          is_list_dictionary_cpp_string = false;
          string_starter = "";
        } else {
          string_starter = item;
          is_list_dictionary_cpp_string = true;
        }
      } else if (item == "]" && is_list_dictionary_cpp_string == false) {
        second_bracket_count -= 1;
        if (second_bracket_count == 0) {
          is_array = false;
          token = token_init(statement, keyword, array, start_index,
                             current_index, line);
        }
      }
    } else if (is_dictionary == true) {
      if (item == "'" &&
          (is_list_dictionary_cpp_string == false || item == string_starter)) {
        keyword += "\"";
      } else if (item == "\"" && item != string_starter &&
                 is_list_dictionary_cpp_string == true) {
        keyword += R"(\")";
      } else {
        keyword += item;
      }
      if (item == "{" && is_list_dictionary_cpp_string == false) {
        third_bracket_count += 1;
      } else if (item == "'" || item == "\"") {
        if (is_list_dictionary_cpp_string == true && string_starter != item) {
        } else if (item == string_starter &&
                   is_list_dictionary_cpp_string == true) {
          is_list_dictionary_cpp_string = false;
          string_starter = "";
        } else {
          string_starter = item;
          is_list_dictionary_cpp_string = true;
        }
      } else if (item == "}" && is_list_dictionary_cpp_string == false) {
        third_bracket_count -= 1;
        if (third_bracket_count == 0) {
          is_dictionary = false;
          token = token_init(statement, keyword, array, start_index,
                             current_index, line);
        }
      }
    } else if ((item == " " && is_dictionary == false && is_array == false &&
                is_string == false) ||
               item == "\n" || item == "\r\n") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
      }
    } else if (item == "[" && is_dictionary == false && is_string == false &&
               is_array == false) {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      start_index = current_index;
      keyword = item;
      is_array = true;
      second_bracket_count += 1;
    } else if (item == "{" && is_dictionary == false && is_string == false &&
               is_array == false) {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      start_index = current_index;
      keyword = item;
      is_dictionary = true;
      third_bracket_count += 1;
    } else if ((item == "'" || item == "\"") && is_array == false &&
               is_dictionary == false) {
      if (is_string == true && string_starter == item) {
        is_string = false;
        if (item == "'") {
          keyword += "\"";
        } else {
          keyword += item;
        }
        token = token_init(statement, keyword, tk_string, start_index,
                           current_index, line);
        string_starter = "";
      } else {
        if (keyword != "") {
          token = token_init(statement, keyword, token_type(keyword),
                             start_index, current_index - 1, line);
          tokens.emplace_back(token);
          token = Token();
          keyword = "";
        }
        is_string = true;
        start_index = current_index;
        if (item == "'") {
          keyword = "\"";
        } else {
          keyword = item;
        }
        string_starter = item;
      }
    } else if (item == "(") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (tokens.back().tk_type == tk_cppcode) {
        start_index = current_index;
        is_cpp = true;
        cpp_bracket_count = 1;
        keyword = item;
      } else {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, keyword, tk_l_paren, start_index,
                           current_index, line);
      }
    } else if (item == ".") {
      if (keyword == "") {
        keyword = item;
        start_index = current_index - 1;
        token = token_init(statement, keyword, tk_dot, start_index,
                           current_index, line);
      } else {
        if (is_number(keyword) == tk_integer) {
          keyword += item;
        } else {
          token = token_init(statement, keyword, token_type(keyword),
                             start_index, current_index - 1, line);
          tokens.emplace_back(token);
          token = Token();
          keyword = "";
          keyword = item;
          start_index = current_index - 1;
          token = token_init(statement, keyword, tk_dot, start_index,
                             current_index, line);
        }
      }
    } else if (item == ")") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      keyword = item;
      start_index = current_index - 1;
      token = token_init(statement, keyword, tk_r_paren, start_index,
                         current_index, line);
    } else if (item == "+") {
      if (keyword != "" && keyword != "+") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (keyword == "+") {
        token = token_init(statement, "++", tk_increment, start_index,
                           current_index, line);
      } else if (next(current_index, src) == "+" ||
                 next(current_index, src) == "=") {
        keyword = item;
        start_index = current_index;
      } else {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, keyword, tk_plus, start_index,
                           current_index, line);
      }
    } else if (item == ">") {
      if (keyword != "" && keyword != ">" && keyword != "-") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (keyword == ">") {
        if (next(current_index, src) == "=") {
          keyword = ">>";
        } else {
          token = token_init(statement, ">>", tk_shift_right, start_index,
                             current_index, line);
        }
      } else if (keyword == "-") {
        token = token_init(statement, "->", tk_arrow, start_index,
                           current_index, line);
      } else if (next(current_index, src) == ">" ||
                 next(current_index, src) == "=") {
        keyword = item;
        start_index = current_index;
      } else {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, ">", tk_greater, start_index,
                           current_index, line);
      }
    } else if (item == "<") {
      if (keyword != "" && keyword != "<") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (keyword == "<") {
        if (next(current_index, src) == "=") {
          keyword = "<<";
        } else {
          token = token_init(statement, "<<", tk_shift_left, start_index,
                             current_index, line);
        }
      } else if (next(current_index, src) == "<" ||
                 next(current_index, src) == "=") {
        keyword = item;
        start_index = current_index;
      } else {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, keyword, tk_less, start_index,
                           current_index, line);
      }
    } else if (item == "-") {
      if (keyword != "" && keyword != "-") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (keyword == "-") {
        token = token_init(statement, "--", tk_decrement, start_index,
                           current_index, line);
      } else if (next(current_index, src) == "-" ||
                 next(current_index, src) == ">" ||
                 next(current_index, src) == "=") {
        keyword = item;
        start_index = current_index;
      } else {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, keyword, tk_minus, start_index,
                           current_index, line);
      }
    } else if (item == "=") {
      if ((std::count(operators.begin(), operators.end(), keyword) == 0) &&
          keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (keyword == "=") {
        keyword = "==";
        token = token_init(statement, keyword, tk_equal, start_index,
                           current_index, line);
      } else if (next(current_index, src) == "=") {
        keyword = item;
        start_index = current_index;
      } else if (keyword == "" && next(current_index, src) != "=") {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, keyword, tk_assign, start_index,
                           current_index, line);
      } else {
        keyword += item;
        token = token_init(statement, keyword, equal(keyword), start_index,
                           current_index, line);
      }
    } else if (item == "/") {
      if (keyword != "" && keyword != "/") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if (keyword == "/") {
        if (next(current_index, src) == "=") {
          keyword = "//";
        } else {
          token = token_init(statement, "//", tk_floor, start_index,
                             current_index, line);
        }
      } else if (next(current_index, src) == "/" ||
                 next(current_index, src) == "=") {
        keyword = item;
        start_index = current_index;
      } else {
        start_index = current_index - 1;
        keyword = item;
        token = token_init(statement, keyword, tk_divide, start_index,
                           current_index, line);
      }
    } else if (item == "^") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
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
        token = token_init(statement, keyword, tk_xor, start_index,
                           current_index, line);
      }
    } else if (item == "%") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
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
        token = token_init(statement, keyword, tk_modulo, start_index,
                           current_index, line);
      }
    } else if (item == "&") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
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
        token = token_init(statement, keyword, tk_ampersand, start_index,
                           current_index, line);
      }
    } else if (item == "|") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
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
        token = token_init(statement, keyword, tk_bit_or, start_index,
                           current_index, line);
      }
    } else if (item == "*") {
      if (keyword != "" && keyword != "*") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      if ((next(current_index, src) == "*" ||
           next(current_index, src) == "=") &&
          (tokens.back().tk_type == tk_identifier ||
           tokens.back().tk_type == tk_decimal ||
           tokens.back().tk_type == tk_integer ||
           next(current_index, src) == "=")) {
        keyword = "*";
        start_index = current_index;
      } else if (keyword == "*") {
        token = token_init(statement, "**", tk_exponent, start_index,
                           current_index, line);
      } else {
        start_index = current_index - 1;
        keyword = item;
        if (tokens.size() > 0) {
          if ((tokens.back().tk_type == tk_true ||
               tokens.back().tk_type == tk_false ||
               tokens.back().tk_type == tk_integer ||
               tokens.back().tk_type == tk_decimal ||
               tokens.back().tk_type == tk_identifier ||
               tokens.back().tk_type == tk_r_paren) &&
              line == tokens.back().line) {
            token = token_init(statement, keyword, tk_multiply, start_index,
                               current_index, line);
          } else {
            token = token_init(statement, keyword, tk_asterisk, start_index,
                               current_index, line);
          }
        } else {
          token = token_init(statement, keyword, tk_asterisk, start_index,
                             current_index, line);
        }
      }
    } else if (item == "~") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      start_index = current_index - 1;
      keyword = item;
      token = token_init(statement, keyword, tk_bit_not, start_index,
                         current_index, line);
    } else if (item == "!") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      start_index = current_index;
      keyword = item;
    } else if (item == ":") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      start_index = current_index - 1;
      keyword = item;
      token = token_init(statement, keyword, tk_colon, start_index,
                         current_index, line);
    } else if (item == ",") {
      if (keyword != "") {
        token = token_init(statement, keyword, token_type(keyword), start_index,
                           current_index - 1, line);
        tokens.emplace_back(token);
        token = Token();
        keyword = "";
      }
      start_index = current_index - 1;
      keyword = item;
      token = token_init(statement, keyword, tk_comma, start_index,
                         current_index, line);
    } else {
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
    tokens.emplace_back(token_init(statement, keyword, token_type(keyword),
                                   start_index, current_index, line));
  }
  curr_identation_level /= 4; // dividing by 4 to know the number of tabs
  while (curr_identation_level != 0) {
    token = token_init(statement, "", tk_dedent, current_index, current_index,
                       line);
    tokens.emplace_back(token);
    curr_identation_level--;
  }
  if (is_string == true || is_list_dictionary_cpp_string == true) {
    std::string temp = "Expecting a ";
    temp += string_starter;
    display(PEError({.loc = Location({.line = line,
                                      .col = current_index - last_line,
                                      .file = filename,
                                      .code = statement}),
                     .msg = "Unexpected end of file",
                     .submsg = temp,
                     .ecode = "e1"}));
    tokens.clear();
  }
  if (is_dictionary == true) {
    std::string temp = "Expecting a }";
    display(PEError({.loc = Location({.line = line,
                                      .col = current_index - last_line,
                                      .file = filename,
                                      .code = statement}),
                     .msg = "Unexpected end of file",
                     .submsg = temp,
                     .ecode = "e1"}));
    tokens.clear();
  }
  if (is_array == true) {
    std::string temp = "Expecting a ]";
    display(PEError({.loc = Location({.line = line,
                                      .col = current_index - last_line,
                                      .file = filename,
                                      .code = statement}),
                     .msg = "Unexpected end of file",
                     .submsg = temp,
                     .ecode = "e1"}));
    tokens.clear();
  }
  if (is_cpp == true) {
    std::string temp = "Expecting a )";
    display(PEError({.loc = Location({.line = line,
                                      .col = current_index - last_line,
                                      .file = filename,
                                      .code = statement}),
                     .msg = "Unexpected end of file",
                     .submsg = temp,
                     .ecode = "e1"}));
    tokens.clear();
  }
  if (tokens.size() > 0) {
    tokens.push_back(
        token_init("", "", tk_eof, current_index, current_index, line));
  }
  return tokens;
}
