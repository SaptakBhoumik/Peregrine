#ifndef JS_PEREGRINE_CODEGEN_HPP
#define JS_PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "utils/symbolTable.hpp"

#include <fstream>
#include <memory>
#include <string>
#include <string_view>

namespace js {

typedef std::shared_ptr<SymbolTable<ast::AstNodePtr>> EnvPtr;

class Codegen : public ast::AstVisitor {
  public:
    Codegen(std::string outputFilename, ast::AstNodePtr ast,bool html,std::string filename);

    EnvPtr createEnv(EnvPtr parent = nullptr);

  private:
    bool is_dot_exp=false;
    bool is_enum=false;
    std::vector<std::string> enum_name={"error"};
    std::string res;
    bool save=false;
    std::string m_filename;
    std::ofstream m_file;
    bool is_func_def=false;
    std::string write(std::string_view code);
    std::string mangleName(ast::AstNodePtr astNode);

    std::string searchDefaultModule(std::string path, std::string moduleName);
    void codegenFuncParams(std::vector<ast::parameter> parameters);
    std::string wrap(ast::AstNodePtr item,std::string contains);
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
    bool visit(const ast::AssertStatement& node);
    bool visit(const ast::WhileStatement& node);
    bool visit(const ast::ForStatement& node);
    bool visit(const ast::MatchStatement& node);
    bool visit(const ast::ScopeStatement& node);
    bool visit(const ast::ReturnStatement& node);
    bool visit(const ast::ContinueStatement& node);
    bool visit(const ast::BreakStatement& node);
    bool visit(const ast::DecoratorStatement& node);
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
    bool visit(const ast::RaiseStatement& node);
    bool visit(const ast::EnumLiteral& node);
    bool visit(const ast::ExportStatement& node);
    bool visit(const ast::TernaryIf& node);
    bool visit(const ast::TryExcept& node);
    bool visit(const ast::MultipleAssign& node);
    bool visit(const ast::AugAssign& node);
    bool visit(const ast::PostfixExpression& node);
    bool visit(const ast::LambdaDefinition& node);
    bool pipeline(const ast::BinaryOperation& node);
    EnvPtr m_env;
};

} // namespace js

#endif
