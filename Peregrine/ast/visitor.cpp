#include "visitor.hpp"
#include "ast.hpp"

#include <iostream>

namespace ast {

// TODO: have a default behaviour in case the visitor does not provide a visit
// method for the node
void Program::accept(AstVisitor& visitor) const {
    if (!visitor.visit(*this))
        for (auto& stmt : m_statements)
            stmt->accept(visitor);
}

void IntegerLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void DecimalLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void StringLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void BoolLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void NoneLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void NoLiteral::accept(AstVisitor& visitor) const {
    visitor.visit(*this); // TODO: maybe we shouldnt do anything instead??
}

void IdentifierExpression::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void TypeExpression::accept(AstVisitor& visitor) const { visitor.visit(*this); }
void UnionLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }
void EnumLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }
void ListTypeExpr::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void DictTypeExpr::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void FunctionTypeExpr::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void ListLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void DictLiteral::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void BinaryOperation::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void PrefixExpression::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}
void PostfixExpression::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}
void ListOrDictAccess::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void ImportStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void VariableStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void ConstDeclaration::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void BlockStatement::accept(AstVisitor& visitor) const {
    if (!visitor.visit(*this))
        for (auto& stmt : m_statements)
            stmt->accept(visitor);
}

void ClassDefinition::accept(AstVisitor& visitor) const {
    if (!visitor.visit(*this)) {
        for (auto& stmt : m_attributes)
            stmt->accept(visitor);
        for (auto& stmt : m_methods)
            stmt->accept(visitor);
        for (auto& stmt : m_other)
            stmt->accept(visitor);
    }
}

void FunctionDefinition::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void ReturnStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void FunctionCall::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void DotExpression::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void ArrowExpression::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void IfStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void AssertStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void StaticStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}
void ExportStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}
void InlineStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void RaiseStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void WhileStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void ForStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void BreakStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void DecoratorStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

void PassStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void ContinueStatement::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}


void ScopeStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void TypeDefinition::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void MatchStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void WithStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void VirtualStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void CastStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void PointerTypeExpr::accept(AstVisitor& visitor) const { visitor.visit(*this); }
void RefTypeExpr::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void DefaultArg::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void TernaryIf::accept(AstVisitor& visitor) const { visitor.visit(*this); }
void TryExcept::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void ExpressionTuple::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void TypeTuple::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void ExternStatement::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void SumType::accept(AstVisitor& visitor) const { visitor.visit(*this); }


void MultipleAssign::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void AugAssign::accept(AstVisitor& visitor) const { visitor.visit(*this); }

void MethodDefinition::accept(AstVisitor &visitor) const {visitor.visit(*this);}

void ExternFuncDef::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void ExternUnionLiteral::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void ExternStructLiteral::accept(AstVisitor &visitor) const {visitor.visit(*this);}

void VarArgTypeExpr::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void VarKwargTypeExpr::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void EllipsesTypeExpr::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void CompileTimeExpression::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void TernaryFor::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void PrivateDef::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void InlineAsm::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void LambdaDefinition::accept(AstVisitor &visitor) const {visitor.visit(*this);}
void GenericCall::accept(AstVisitor& visitor) const { visitor.visit(*this); }
} // namespace ast
