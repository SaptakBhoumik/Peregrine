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

    bool visit(const ast::ClassDefinition& node);
    bool visit(const ast::ImportStatement& node);
    bool visit(const ast::FunctionDefinition& node);
    bool visit(const ast::VariableStatement& node);
    bool visit(const ast::ConstDeclaration& node);
    bool visit(const ast::TypeDefinition& node);
    bool visit(const ast::IfStatement& node);
    bool visit(const ast::AssertStatement& node);
    bool visit(const ast::StaticStatement& node);
    bool visit(const ast::ExportStatement& node);
    bool visit(const ast::InlineStatement& node);
    bool visit(const ast::RaiseStatement& node);
    bool visit(const ast::WhileStatement& node);
    bool visit(const ast::ForStatement& node);
    bool visit(const ast::MatchStatement& node);
    bool visit(const ast::ScopeStatement& node);
    // bool visit(const ast::CppStatement& node);
    bool visit(const ast::ReturnStatement& node);
    bool visit(const ast::DecoratorStatement& node);
    bool visit(const ast::ListLiteral& node);
    bool visit(const ast::DictLiteral& node);
    bool visit(const ast::ListOrDictAccess& node);
    bool visit(const ast::BinaryOperation& node);
    bool visit(const ast::PrefixExpression& node);
    bool visit(const ast::PostfixExpression& node);
    bool visit(const ast::FunctionCall& node);
    bool visit(const ast::DotExpression& node);
    bool visit(const ast::ArrowExpression& node);
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
    bool visit(const ast::UnionLiteral& node);
    bool visit(const ast::EnumLiteral& node);
    bool visit(const ast::WithStatement& node);
    bool visit(const ast::VirtualStatement& node);
    bool visit(const ast::CastStatement& node);
    bool visit(const ast::PointerTypeExpr& node);
    bool visit(const ast::DefaultArg& node);
    bool visit(const ast::TernaryIf& node);
    bool visit(const ast::TryExcept& node);

    std::string m_filename;
    TypePtr m_result;
    EnvPtr m_env;

    // the function whose body is being currently checked
    std::shared_ptr<FunctionType> m_currentFunction;
};

#endif