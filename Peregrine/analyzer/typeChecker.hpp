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
    void error(Token tok, std::string_view msg);
    EnvPtr createEnv(EnvPtr parent = nullptr);
    std::string identifierName(ast::AstNodePtr identifier);
    void checkBody(ast::AstNodePtr body);

    void check(ast::AstNodePtr expr, const Type& expectedType);

    bool visit(const ast::ImportStatement& node);
    bool visit(const ast::FunctionDefinition& node);
    bool visit(const ast::VariableStatement& node);
    bool visit(const ast::ConstDeclaration& node);
    bool visit(const ast::TypeDefinition& node);
    bool visit(const ast::IfStatement& node);
    bool visit(const ast::WhileStatement& node);
    bool visit(const ast::ForStatement& node);
    bool visit(const ast::MatchStatement& node);
    bool visit(const ast::ScopeStatement& node);
    bool visit(const ast::ReturnStatement& node);
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
    bool visit(const ast::PointerTypeExpr& node);
    bool visit(const ast::NoLiteral& node);
    bool visit(const ast::IntegerLiteral& node);
    bool visit(const ast::DecimalLiteral& node);
    bool visit(const ast::StringLiteral& node);
    bool visit(const ast::BoolLiteral& node);
    bool visit(const ast::NoneLiteral& node);

    std::string m_filename;
    TypePtr m_result;
    EnvPtr m_env;

    // the function whose body is being currently checked
    std::shared_ptr<FunctionType> m_currentFunction;
};

#endif