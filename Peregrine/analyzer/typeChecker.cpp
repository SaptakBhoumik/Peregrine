#include "typeChecker.hpp"
#include "ast/ast.hpp"
#include "ast/types.hpp"

#include <cassert>
#include <iostream>
#include <memory>

using namespace types;

TypeChecker::TypeChecker(ast::AstNodePtr ast) {
    m_env = createEnv();
    m_currentFunction = nullptr;
    ast->accept(*this);
}

void TypeChecker::error(std::string_view msg) {
    // PEError err = {{tok.line, tok.start, m_filename, tok.statement},
    //                std::string(msg),
    //                "TypeError",
    //                "",
    //                ""};

    // m_errors.push_back(err);
    std::cerr << "TypeError: " << msg << "\n";
}

EnvPtr TypeChecker::createEnv(EnvPtr parent) {
    return std::make_shared<SymbolTable<TypePtr>>(parent);
}

void TypeChecker::checkBody(ast::AstNodePtr body) {
    EnvPtr previousEnv = m_env;
    m_env = createEnv();
    body->accept(*this);
    m_env = previousEnv;
}

void TypeChecker::check(ast::AstNodePtr expr, TypePtr expectedType) {
    expr->accept(*this);
    TypePtr exprType = m_result;

    if (exprType != expectedType) {
        if (!exprType->isConvertibleTo(expectedType) &&
            expectedType->isConvertibleTo(exprType)) {
            error("expected type " + expectedType->stringify() + ", got " +
                  exprType->stringify() + " instead");
        }
    }
}

std::string TypeChecker::identifierName(ast::AstNodePtr identifier) {
    assert(identifier->type() == ast::KAstIdentifier);

    return std::dynamic_pointer_cast<ast::IdentifierExpression>(identifier)
        ->value();
}

void TypeChecker::visit(const ast::Program& node) {}

void TypeChecker::visit(const ast::BlockStatement& node) {}

void TypeChecker::visit(const ast::ImportStatement& node) {}

void TypeChecker::visit(const ast::FunctionDefinition& node) {
    // EnvPtr oldEnv = m_env;
    // m_env = createEnv();

    // std::vector<TypePtr> parameterTypes;
    // parameterTypes.reserve(node.parameters().size());

    // for (auto& param : node.parameters()) {
    //     TypePtr paramType = infer(m_env, param.p_type);
    //     parameterTypes.push_back(paramType);
    //     m_env->set(identifierName(param.p_name()), paramType);
    // }

    // auto functionType = std::make_shared<FunctionType>(parameterTypes,
    // infer(m_env, node.returnType()); m_currentFunction = functionType;
    // node.body()->accept(*this);
    // m_currentFunction = nullptr;

    // m_env->set(identifierName(node.name()), functionType);
}

void TypeChecker::visit(const ast::VariableStatement& node) {
    node.varType()->accept(*this);
    TypePtr varType = m_result;

    if (varType->category() == TypeCategory::None) {
        // infer it
    }

    check(node.value(), varType);
    m_env->set(identifierName(node.name()), varType);
}

void TypeChecker::visit(const ast::ConstDeclaration& node) {}

void TypeChecker::visit(const ast::TypeDefinition& node) {}

void TypeChecker::visit(const ast::PassStatement& node) {}

void TypeChecker::visit(const ast::IfStatement& node) {}

void TypeChecker::visit(const ast::WhileStatement& node) {}

void TypeChecker::visit(const ast::ForStatement& node) {}

void TypeChecker::visit(const ast::MatchStatement& node) {}

void TypeChecker::visit(const ast::ScopeStatement& node) {}

void TypeChecker::visit(const ast::ReturnStatement& node) {}

void TypeChecker::visit(const ast::ContinueStatement& node) {}

void TypeChecker::visit(const ast::BreakStatement& node) {}

void TypeChecker::visit(const ast::ListLiteral& node) {}

void TypeChecker::visit(const ast::DictLiteral& node) {}

void TypeChecker::visit(const ast::ListOrDictAccess& node) {}

void TypeChecker::visit(const ast::BinaryOperation& node) {
    node.left()->accept(*this);
    TypePtr leftType = m_result;
    node.right()->accept(*this);
    TypePtr result = leftType->infixOperatorResult(node.op(), m_result);

    if (!result) {
        error("operator " + node.op().keyword + " can not be used with types " +
              leftType->stringify() + " and " + m_result->stringify());
    }

    m_result = leftType;
}

void TypeChecker::visit(const ast::PrefixExpression& node) {
    node.right()->accept(*this);
    TypePtr result = m_result->prefixOperatorResult(node.prefix());
    if (!result) {
        error("operator " + node.prefix().keyword +
              " can not be used with type " + m_result->stringify());
    }

    m_result = result;
}

void TypeChecker::visit(const ast::FunctionCall& node) {}

void TypeChecker::visit(const ast::DotExpression& node) {}

void TypeChecker::visit(const ast::IdentifierExpression& node) {
    auto identifierType = m_env->get(node.value());
    if (!identifierType) {
        error("undeclared identifier: " + node.value());
    }

    m_result = *identifierType;
}

void TypeChecker::visit(const ast::TypeExpression& node) {
    m_result = identifierToTypeMap[node.value()];
}

void TypeChecker::visit(const ast::ListTypeExpr& node) {}

void TypeChecker::visit(const ast::DictTypeExpr& node) {}

void TypeChecker::visit(const ast::FunctionTypeExpr& node) {}

void TypeChecker::visit(const ast::NoLiteral& node) {
    m_result = TypeProducer::none();
}

void TypeChecker::visit(const ast::IntegerLiteral& node) {
    m_result = TypeProducer::integer();
}

void TypeChecker::visit(const ast::DecimalLiteral& node) {
    m_result = TypeProducer::decimal();
}

void TypeChecker::visit(const ast::StringLiteral& node) {
    m_result = TypeProducer::string();
}

void TypeChecker::visit(const ast::BoolLiteral& node) {
    m_result = TypeProducer::boolean();
}

void TypeChecker::visit(const ast::NoneLiteral& node) {
    m_result = TypeProducer::none();
}
