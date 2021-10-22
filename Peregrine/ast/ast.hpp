#ifndef PEREGRINE_AST
#define PEREGRINE_AST
#include "../lexer/tokens.hpp"
#include <vector>
#include <string>

typedef enum{
    ast_asign//will define more later
}Ast_Type;

typedef struct Body{
    Ast_Type ast_type;
    Token token;
    Body *left =NULL;
    Body *right =NULL;
}Body;

typedef struct{
	std::vector<std::string> import_file;         
	std::vector<std::string> header_file;         
	std::vector<std::string> cpp_file;        
	std::string folder;
	std::string path;
	Body *body;
}Ast;
#endif
