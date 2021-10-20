#ifndef PEREGRINE_LEXER
#define PEREGRINE_LEXER
#define LEXEME std::vector<Token> 
#include "tokens.hpp"
#include <string>
#include <vector>
#define INTEGER unsigned long int
LEXEME lexer(std::string src,std::string filename);
#endif