#ifndef PEREGRINE_AST_VISITOR_HPP
#define PEREGRINE_AST_VISITOR_HPP

#include "ast.hpp"

namespace ast {

class AstVisitor {
  public:
    virtual ~AstVisitor() = default;

    virtual bool visit(const Program& node) { return false; };
    virtual bool visit(const BlockStatement& node) { return false; };
    virtual bool visit(const ClassDefinition& node){ return false; }
    virtual bool visit(const ImportStatement& node) { return false; };
    virtual bool visit(const FunctionDefinition& node) { return false; };
    virtual bool visit(const VariableStatement& node) { return false; };
    virtual bool visit(const ConstDeclaration& node) { return false; };
    virtual bool visit(const TypeDefinition& node) { return false; };
    virtual bool visit(const PassStatement& node) { return false; };
    virtual bool visit(const IfStatement& node) { return false; };
    virtual bool visit(const AssertStatement& node) { return false; };
    virtual bool visit(const StaticStatement& node) { return false; };
    virtual bool visit(const ExportStatement& node) { return false; };
    virtual bool visit(const InlineStatement& node) { return false; };
    virtual bool visit(const RaiseStatement& node) { return false; };
    virtual bool visit(const WhileStatement& node) { return false; };
    virtual bool visit(const ForStatement& node) { return false; };
    virtual bool visit(const MatchStatement& node) { return false; };
    virtual bool visit(const ScopeStatement& node) { return false; };
    virtual bool visit(const ReturnStatement& node) { return false; };
    virtual bool visit(const ContinueStatement& node) { return false; };
    virtual bool visit(const BreakStatement& node) { return false; };
    virtual bool visit(const DecoratorStatement& node) { return false; };
    virtual bool visit(const ListLiteral& node) { return false; };
    virtual bool visit(const DictLiteral& node) { return false; };
    virtual bool visit(const ListOrDictAccess& node) { return false; };
    virtual bool visit(const BinaryOperation& node) { return false; };
    virtual bool visit(const PrefixExpression& node) { return false; };
    virtual bool visit(const PostfixExpression& node) { return false; };
    virtual bool visit(const FunctionCall& node) { return false; };
    virtual bool visit(const DotExpression& node) { return false; };
    virtual bool visit(const ArrowExpression& node) { return false; };
    virtual bool visit(const IdentifierExpression& node) { return false; };
    virtual bool visit(const TypeExpression& node) { return false; };
    virtual bool visit(const ListTypeExpr& node) { return false; };
    virtual bool visit(const DictTypeExpr& node) { return false; };
    virtual bool visit(const FunctionTypeExpr& node) { return false; };
    virtual bool visit(const NoLiteral& node) { return false; };
    virtual bool visit(const IntegerLiteral& node) { return false; };
    virtual bool visit(const DecimalLiteral& node) { return false; };
    virtual bool visit(const StringLiteral& node) { return false; };
    virtual bool visit(const BoolLiteral& node) { return false; };
    virtual bool visit(const NoneLiteral& node) { return false; };
    virtual bool visit(const UnionLiteral& node) { return false; };
    virtual bool visit(const EnumLiteral& node) { return false; };
    virtual bool visit(const WithStatement& node) { return false; };
    virtual bool visit(const VirtualStatement& node) { return false; };
    virtual bool visit(const CastStatement& node) { return false; };
    virtual bool visit(const PointerTypeExpr& node) { return false; };
    virtual bool visit(const RefTypeExpr& node) { return false; };
    virtual bool visit(const DefaultArg& node) { return false; };
    virtual bool visit(const TernaryIf& node) { return false; };
    virtual bool visit(const TryExcept& node) { return false; };
    virtual bool visit(const ExpressionTuple& node) { return false; };
    virtual bool visit(const TypeTuple& node) { return false; };
    virtual bool visit(const ExternStatement& node) { return false; };
    virtual bool visit(const SumType& node) { return false; };
    virtual bool visit(const MultipleAssign& node) { return false; };
    virtual bool visit(const AugAssign& node) { return false; };
    virtual bool visit(const MethodDefinition& node) { return false; };
    virtual bool visit(const ExternFuncDef& node) { return false; };
    virtual bool visit(const ExternUnionLiteral& node) { return false; };
    virtual bool visit(const ExternStructLiteral& node) { return false; };
    virtual bool visit(const VarArgTypeExpr& node) { return false; };
    virtual bool visit(const EllipsesTypeExpr& node) { return false; };
    virtual bool visit(const VarKwargTypeExpr& node) { return false; };
    virtual bool visit(const CompileTimeExpression& node) { return false; };
    virtual bool visit(const TernaryFor& node) { return false; };
    virtual bool visit(const PrivateDef& node) { return false; };
    virtual bool visit(const InlineAsm& node) { return false; };
    virtual bool visit(const LambdaDefinition& node) { return false; };
    virtual bool visit(const GenericCall& node) { return false; };

};

} // namespace ast

#endif
