#ifndef PEREGRINE_CODEGEN_HPP
#define PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"

#include <fstream>
#include <string>
#include <string_view>

class Codegen {
    std::ofstream m_file;

  public:
    void write(std::string_view code);
    Codegen(std::string outputFilename);

    std::string generate(AstNodePtr astNode);
};

#endif