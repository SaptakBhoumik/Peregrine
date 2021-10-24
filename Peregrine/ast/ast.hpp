#ifndef PEREGRINE_AST
#define PEREGRINE_AST
#include "../lexer/tokens.hpp"
#include <string>
#include <vector>

typedef enum {
  ast_assign // will define more later
} Ast_Type;

typedef struct Body {
  Ast_Type ast_type;
  Token token;
  std::vector<Body> left ;
  std::vector<Body> right ;
} Body;

typedef struct {
  std::vector<std::string> import_file;
  std::vector<std::string> header_file;
  std::vector<std::string> cpp_file;
  std::string folder;
  std::string path;
  std::vector<Body> body ;
} Ast;
#endif
