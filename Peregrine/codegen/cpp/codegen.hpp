#ifndef PEREGRINE_CODEGEN_HPP
#define PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"
#include "utils/symbolTable.hpp"

#include <fstream>
#include <memory>
#include <string>
#include <string_view>

namespace cpp {

typedef std::shared_ptr<SymbolTable<ast::AstNodePtr>> EnvPtr;

class Codegen {
    std::ofstream m_file;

    void write(std::string_view code);
    std::string mangleName(ast::AstNodePtr astNode);

    std::string searchDefaultModule(std::string path, std::string moduleName);
    void codegenImport(std::shared_ptr<ast::ImportStatement> importNode);
    void codegenFuncParams(std::vector<ast::parameter> parameters, EnvPtr env);

    void matchArg(std::vector<ast::AstNodePtr> matchItem,
                  std::vector<ast::AstNodePtr> caseItem);

  public:
    Codegen(std::string outputFilename);

    void generate(ast::AstNodePtr astNode, EnvPtr env);
    EnvPtr createEnv(EnvPtr parent = nullptr);
};

} // namespace cpp

#endif