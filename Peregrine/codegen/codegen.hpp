#ifndef PEREGRINE_CODEGEN_CODEGEN_HPP
#define PEREGRINE_CODEGEN_CODEGEN_HPP
#include "../parser/parser.hpp"

class CodeGen {
private:
    std::string gen_operator(ast_node curr_node);
    std::string gen(ast_node curr_node);
    std::string gen_infix(ast_node curr_node);
public:
    AST ast;
    std::string codegen();
};
#endif