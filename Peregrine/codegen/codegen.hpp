#ifndef PEREGRINE_CODEGEN_HPP
#define PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"

#include <fstream>
#include <string>
#include <string_view>

class Codegen {
    std::ofstream m_file;

    void write(std::string_view code);

  public:
    Codegen(std::string output_filename);

    void generate(AstNodePtr ast_node);
};

#endif