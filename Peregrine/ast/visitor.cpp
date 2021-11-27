#include "visitor.hpp"

namespace ast {

//TODO: have a default behaviour in case the visitor does not provide a visit method for the node

void Program::accept(const AstVisitor& visitor) {
    for (auto& stmt : m_statements) 
        stmt->accept(visitor);
}

void IntegerLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void DecimalLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void StringLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void BoolLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void NoneLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void NoLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this); //TODO: maybe we shouldnt do anything instead??
}

void IdentifierExpression::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void TypeExpression::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ListTypeExpr::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void DictTypeExpr::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void FunctonTypeExpr::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ListLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void DictLiteral::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void BinaryOperation::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void PrefixExpression::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ListOrDictAccess::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ImportStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void VariableStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ConstDeclaration::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void BlockStatement::accept(const AstVisitor& visitor) {
    for (auto& stmt : m_statements)
        stmt->accept(visitor);
}

void FunctionDefinition::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ReturnStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void FunctionCall::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void IfStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void WhileStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ForStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void BreakStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void PassStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void ContinueStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

//TODO: cppstatement

void ScopeStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void TypeDefinition::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}

void MatchStatement::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}