#ifndef PEREGRINE_AST_VISITOR_HPP
#define PEREGRINE_AST_VISITOR_HPP

#include "ast.hpp"

namespace ast {

class AstVisitor {
public:
    virtual bool visit(const Program& node) = 0;
    virtual bool visit(const BlockStatement& node) = 0;
    virtual bool visit(const ImportStatement& node) = 0;
    virtual bool visit(const FunctionDefinition& node) = 0;
    virtual bool visit(const VariableStatement& node) = 0;
    virtual bool visit(const ConstDeclaration& node) = 0;
    virtual bool visit(const TypeDefinition& node) = 0;
    virtual bool visit(const PassStatement& node) = 0;
    virtual bool visit(const IfStatement& node) = 0;
    virtual bool visit(const WhileStatement& node) = 0;
    virtual bool visit(const ForStatement& node) = 0;
    virtual bool visit(const MatchStatement& node) = 0;
    virtual bool visit(const ScopeStatement& node) = 0;
    virtual bool visit(const ReturnStatement& node) = 0;
    virtual bool visit(const ContinueStatement& node) = 0;
    virtual bool visit(const BreakStatement& node) = 0;
    virtual bool visit(const ListLiteral& node) = 0;
    virtual bool visit(const DictLiteral& node) = 0;
    virtual bool visit(const ListOrDictAccess& node) = 0;
    virtual bool visit(const BinaryOperation& node) = 0;
    virtual bool visit(const PrefixExpression& node) = 0;
    virtual bool visit(const FunctionCall& node) = 0;
    virtual bool visit(const DotExpression& node) = 0;
    virtual bool visit(const IdentifierExpression& node) = 0;
    virtual bool visit(const TypeExpression& node) = 0;
    virtual bool visit(const ListTypeExpr& node) = 0;
    virtual bool visit(const DictTypeExpr& node) = 0;
    virtual bool visit(const FunctionTypeExpr& node) = 0;
    virtual bool visit(const NoLiteral& node) = 0;
    virtual bool visit(const IntegerLiteral& node) = 0;
    virtual bool visit(const DecimalLiteral& node) = 0;
    virtual bool visit(const StringLiteral& node) = 0;
    virtual bool visit(const BoolLiteral& node) = 0;
    virtual bool visit(const NoneLiteral& node) = 0;
};

}

#endif