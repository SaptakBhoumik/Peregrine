#ifndef PEREGRINE_LEXER_HPP
#define PEREGRINE_LEXER_HPP
#define LEXEME std::vector<Token>
#include "tokens.hpp"
#include <string>
#include <vector>
LEXEME lexer(std::string src, std::string filename);
#endif