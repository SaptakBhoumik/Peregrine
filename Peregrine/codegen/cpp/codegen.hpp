#ifndef PEREGRINE_CODEGEN_HPP
#define PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"
#include "utils/symbolTable.hpp"

#include <fstream>
#include <string>
#include <memory>
#include <string_view>

namespace cpp {

class Codegen {
    std::ofstream m_file;
    bool is_func_def;
    std::string matchArg(std::vector<ast::AstNodePtr> matchItem,std::vector<ast::AstNodePtr> caseItem);
  public:
    Codegen(std::string outputFilename);

    std::shared_ptr<SymbolTable<ast::AstNodePtr>> createEnv(std::shared_ptr<SymbolTable<ast::AstNodePtr>> parent = nullptr);
    void flush(std::string_view code);

    std::string generate(ast::AstNodePtr astNode, std::shared_ptr<SymbolTable<ast::AstNodePtr>> env);
    std::string mangleName(ast::AstNodePtr astNode);
};

}

#endif