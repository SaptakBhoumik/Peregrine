#ifndef PEREGRINE_LEXER
#define PEREGRINE_LEXER
#include "tokens.hpp"
#include <string>
#include <vector>
#define INTEGER unsigned long int
std::vector<Token> lexer(std::string src);
#endif