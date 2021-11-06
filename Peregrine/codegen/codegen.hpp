#ifndef PEREGRINE_CODEGEN_CODEGEN_HPP
#define PEREGRINE_CODEGEN_CODEGEN_HPP
#include "../parser/parser.hpp"

class CodeGen {
  private:
    std::string gen_operator(AstNode curr_node);
    std::string gen(AstNode curr_node);
    std::string gen_infix(AstNode curr_node);

  public:
    AST ast;
    std::string codegen();
};
#endif