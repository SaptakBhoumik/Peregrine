#ifndef PEREGRINE_CODEGEN_HPP
#define PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "utils/symbolTable.hpp"

#include <fstream>
#include <memory>
#include <string>
#include <string_view>

namespace cpp {

typedef std::shared_ptr<SymbolTable<ast::AstNodePtr>> EnvPtr;

class Codegen : public ast::AstVisitor {
  public:
    Codegen(std::string outputFilename, ast::AstNodePtr ast);

    EnvPtr createEnv(EnvPtr parent = nullptr);

  private:
    std::ofstream m_file;

    void write(std::string_view code);
    std::string mangleName(ast::AstNodePtr astNode);

    std::string searchDefaultModule(std::string path, std::string moduleName);
    void codegenFuncParams(std::vector<ast::parameter> parameters);

    void matchArg(std::vector<ast::AstNodePtr> matchItem,
                  std::vector<ast::AstNodePtr> caseItem);

    bool visit(const ast::Program& node);
    bool visit(const ast::BlockStatement& node);
    bool visit(const ast::ImportStatement& node);
    bool visit(const ast::FunctionDefinition& node);
    bool visit(const ast::VariableStatement& node);
    bool visit(const ast::ConstDeclaration& node);
    bool visit(const ast::TypeDefinition& node);
    bool visit(const ast::PassStatement& node);
    bool visit(const ast::IfStatement& node);
    bool visit(const ast::WhileStatement& node);
    bool visit(const ast::ForStatement& node);
    bool visit(const ast::MatchStatement& node);
    bool visit(const ast::ScopeStatement& node);
    bool visit(const ast::CppStatement& node);
    bool visit(const ast::ReturnStatement& node);
    bool visit(const ast::ContinueStatement& node);
    bool visit(const ast::BreakStatement& node);
    bool visit(const ast::ListLiteral& node);
    bool visit(const ast::DictLiteral& node);
    bool visit(const ast::ListOrDictAccess& node);
    bool visit(const ast::BinaryOperation& node);
    bool visit(const ast::PrefixExpression& node);
    bool visit(const ast::FunctionCall& node);
    bool visit(const ast::DotExpression& node);
    bool visit(const ast::IdentifierExpression& node);
    bool visit(const ast::TypeExpression& node);
    bool visit(const ast::ListTypeExpr& node);
    bool visit(const ast::DictTypeExpr& node);
    bool visit(const ast::FunctionTypeExpr& node);
    bool visit(const ast::NoLiteral& node);
    bool visit(const ast::IntegerLiteral& node);
    bool visit(const ast::DecimalLiteral& node);
    bool visit(const ast::StringLiteral& node);
    bool visit(const ast::BoolLiteral& node);
    bool visit(const ast::NoneLiteral& node);

    EnvPtr m_env;
};

} // namespace cpp

#endif