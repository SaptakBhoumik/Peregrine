#ifndef PEREGRINE_TYPE_CHECKER_HPP
#define PEREGRINE_TYPE_CHECKER_HPP

#include "ast/ast.hpp"
#include "ast/types.hpp"
#include "ast/visitor.hpp"
#include "utils/symbolTable.hpp"

#include <memory>
#include <vector>

using namespace types;

using EnvPtr = std::shared_ptr<SymbolTable<TypePtr>>;

class TypeChecker : public ast::AstVisitor {
  public:
    TypeChecker(ast::AstNodePtr ast);

  private:
    void error(std::string_view msg);
    EnvPtr createEnv(EnvPtr parent = nullptr);
    std::string identifierName(ast::AstNodePtr identifier);
    void checkBody(ast::AstNodePtr body);

    void check(ast::AstNodePtr expr, TypePtr expectedType);

    void visit(const ast::Program& node);
    void visit(const ast::BlockStatement& node);
    void visit(const ast::ImportStatement& node);
    void visit(const ast::FunctionDefinition& node);
    void visit(const ast::VariableStatement& node);
    void visit(const ast::ConstDeclaration& node);
    void visit(const ast::TypeDefinition& node);
    void visit(const ast::PassStatement& node);
    void visit(const ast::IfStatement& node);
    void visit(const ast::WhileStatement& node);
    void visit(const ast::ForStatement& node);
    void visit(const ast::MatchStatement& node);
    void visit(const ast::ScopeStatement& node);
    void visit(const ast::ReturnStatement& node);
    void visit(const ast::ContinueStatement& node);
    void visit(const ast::BreakStatement& node);
    void visit(const ast::ListLiteral& node);
    void visit(const ast::DictLiteral& node);
    void visit(const ast::ListOrDictAccess& node);
    void visit(const ast::BinaryOperation& node);
    void visit(const ast::PrefixExpression& node);
    void visit(const ast::FunctionCall& node);
    void visit(const ast::DotExpression& node);
    void visit(const ast::IdentifierExpression& node);
    void visit(const ast::TypeExpression& node);
    void visit(const ast::ListTypeExpr& node);
    void visit(const ast::DictTypeExpr& node);
    void visit(const ast::FunctionTypeExpr& node);
    void visit(const ast::NoLiteral& node);
    void visit(const ast::IntegerLiteral& node);
    void visit(const ast::DecimalLiteral& node);
    void visit(const ast::StringLiteral& node);
    void visit(const ast::BoolLiteral& node);
    void visit(const ast::NoneLiteral& node);

    TypePtr m_result;
    EnvPtr m_env;

    // the function whose body is being currently checked
    std::shared_ptr<FunctionType> m_currentFunction;
};

#endif