#ifndef PEREGRINE_AST_VISITOR_HPP
#define PEREGRINE_AST_VISITOR_HPP

#include "ast.hpp"

namespace ast {

class AstVisitor {
  public:
    virtual ~AstVisitor() = default;

    virtual void visit(const Program& node) = 0;
    virtual void visit(const BlockStatement& node) = 0;
    virtual void visit(const ImportStatement& node) = 0;
    virtual void visit(const FunctionDefinition& node) = 0;
    virtual void visit(const VariableStatement& node) = 0;
    virtual void visit(const ConstDeclaration& node) = 0;
    virtual void visit(const TypeDefinition& node) = 0;
    virtual void visit(const PassStatement& node) = 0;
    virtual void visit(const IfStatement& node) = 0;
    virtual void visit(const WhileStatement& node) = 0;
    virtual void visit(const ForStatement& node) = 0;
    virtual void visit(const MatchStatement& node) = 0;
    virtual void visit(const ScopeStatement& node) = 0;
    virtual void visit(const ReturnStatement& node) = 0;
    virtual void visit(const ContinueStatement& node) = 0;
    virtual void visit(const BreakStatement& node) = 0;
    virtual void visit(const ListLiteral& node) = 0;
    virtual void visit(const DictLiteral& node) = 0;
    virtual void visit(const ListOrDictAccess& node) = 0;
    virtual void visit(const BinaryOperation& node) = 0;
    virtual void visit(const PrefixExpression& node) = 0;
    virtual void visit(const FunctionCall& node) = 0;
    virtual void visit(const DotExpression& node) = 0;
    virtual void visit(const IdentifierExpression& node) = 0;
    virtual void visit(const TypeExpression& node) = 0;
    virtual void visit(const ListTypeExpr& node) = 0;
    virtual void visit(const DictTypeExpr& node) = 0;
    virtual void visit(const FunctionTypeExpr& node) = 0;
    virtual void visit(const NoLiteral& node) = 0;
    virtual void visit(const IntegerLiteral& node) = 0;
    virtual void visit(const DecimalLiteral& node) = 0;
    virtual void visit(const StringLiteral& node) = 0;
    virtual void visit(const BoolLiteral& node) = 0;
    virtual void visit(const NoneLiteral& node) = 0;
};

} // namespace ast

#endif