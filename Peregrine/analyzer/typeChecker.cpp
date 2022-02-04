#include "typeChecker.hpp"
#include "ast/ast.hpp"
#include "ast/types.hpp"
#include "errors/error.hpp"

#include <cassert>
#include <iostream>
#include <memory>

using namespace types;

TypeChecker::TypeChecker(ast::AstNodePtr ast) {
    m_env = createEnv();
    m_currentFunction = nullptr;
    ast->accept(*this);
}

void TypeChecker::error(Token tok, std::string_view msg) {
    PEError err = {
        {tok.line, tok.start, tok.location, m_filename, tok.statement},
        std::string(msg),
        "TypeError",
        "",
        ""};
    display(err);
    exit(1);
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

void TypeChecker::check(ast::AstNodePtr expr, const Type& expectedType) {
    expr->accept(*this);
    const Type& exprType = *m_result;

    if (exprType != expectedType) {
        if (!exprType.isConvertibleTo(expectedType) &&
            !expectedType.isConvertibleTo(exprType)) {
            error(expr->token(), "expected type " + expectedType.stringify() +
                                     ", got " + exprType.stringify() +
                                     " instead");
        }

        // TODO: convert one type to another
    }
}

std::string TypeChecker::identifierName(ast::AstNodePtr identifier) {
    assert(identifier->type() == ast::KAstIdentifier);

    return std::dynamic_pointer_cast<ast::IdentifierExpression>(identifier)
        ->value();
}

bool TypeChecker::visit(const ast::ClassDefinition& node) { return true; }

bool TypeChecker::visit(const ast::ImportStatement& node) { return true; }

// TODO: default args
bool TypeChecker::visit(const ast::FunctionDefinition& node) {
    EnvPtr oldEnv = m_env;
    m_env = createEnv();

    std::vector<TypePtr> parameterTypes;
    parameterTypes.reserve(node.parameters().size());

    for (auto& param : node.parameters()) {
        if (param.p_default->type() != ast::KAstNoLiteral) {
            if (param.p_type->type() != ast::KAstNoLiteral) {
                param.p_type->accept(*this);
                check(param.p_default, *m_result);
            }

            param.p_default->accept(*this);
            parameterTypes.push_back(m_result);
            m_env->set(identifierName(param.p_name), m_result);
            continue;
        }

        param.p_type->accept(*this);
        parameterTypes.push_back(m_result);
        m_env->set(identifierName(param.p_name), m_result);
    }
    node.returnType()->accept(*this);

    auto functionType =
        std::make_shared<FunctionType>(parameterTypes, m_result);

    auto oldFunction = m_currentFunction;
    m_currentFunction = functionType;
    node.body()->accept(*this);
    m_currentFunction = oldFunction;

    m_env = oldEnv;
    m_env->set(identifierName(node.name()), functionType);
    return true;
}

bool TypeChecker::visit(const ast::VariableStatement& node) {
    // not very proud of this
    auto nonConstNode = const_cast<ast::VariableStatement&>(node);

    node.varType()->accept(*this);
    TypePtr varType = m_result;

    if (varType->category() == TypeCategory::Void) {
        // inferring the type of the variable
        node.value()->accept(*this);
        nonConstNode.setProcessedType(m_result);
        varType = m_result;
    } else {
        check(node.value(), *varType);
        nonConstNode.setProcessedType(varType);
    }

    m_env->set(identifierName(node.name()), varType);
    return true;
}

bool TypeChecker::visit(const ast::ConstDeclaration& node) {
    auto nonConstNode = const_cast<ast::ConstDeclaration&>(node);

    node.constType()->accept(*this);
    TypePtr constType = m_result;

    if (constType->category() == TypeCategory::Void) {
        // inferring the type of the constant
        node.value()->accept(*this);
        nonConstNode.setProcessedType(m_result);
        constType = m_result;
    } else {
        check(node.value(), *constType);
        nonConstNode.setProcessedType(constType);
    }

    m_env->set(identifierName(node.name()), constType);
    return true;
}

bool TypeChecker::visit(const ast::TypeDefinition& node) {
    node.baseType()->accept(*this);
    TypePtr userDefinedType = std::make_shared<UserDefinedType>(m_result);

    m_env->set(identifierName(node.name()), userDefinedType);
    return true;
}

bool TypeChecker::visit(const ast::IfStatement& node) {
    check(node.condition(), *TypeProducer::boolean());
    checkBody(node.ifBody());

    for (auto& elif : node.elifs()) {
        check(elif.first, *TypeProducer::boolean());
        checkBody(elif.second);
    }

    if (node.elseBody()->type() != ast::KAstNoLiteral)
        checkBody(node.elseBody());
    return true;
}

bool TypeChecker::visit(const ast::AssertStatement& node) { return true; }

bool TypeChecker::visit(const ast::StaticStatement& node) { return true; }

bool TypeChecker::visit(const ast::ExportStatement& node) { return true; }

bool TypeChecker::visit(const ast::InlineStatement& node) { return true; }

bool TypeChecker::visit(const ast::RaiseStatement& node) { return true; }

bool TypeChecker::visit(const ast::WhileStatement& node) {
    check(node.condition(), *TypeProducer::boolean());
    checkBody(node.body());
    return true;
}

bool TypeChecker::visit(const ast::ForStatement& node) {
    // check(node.sequence(), *TypeProducer::list());
    EnvPtr oldEnv = m_env;
    m_env = createEnv();
    // m_env->set(identifierName(node.variable()), m_result); // result may not
    // be correct here

    node.body()->accept(*this);
    return true;
}

bool TypeChecker::visit(const ast::MatchStatement& node) { return true; }

bool TypeChecker::visit(const ast::ScopeStatement& node) {
    checkBody(node.body());
    return true;
}

bool TypeChecker::visit(const ast::ReturnStatement& node) {
    if (!m_currentFunction) {
        error(node.token(), "can not use return outside of a function");
    }

    node.returnValue()->accept(*this);

    check(node.returnValue(), *m_currentFunction->returnType());
    return true;
}

bool TypeChecker::visit(const ast::DecoratorStatement& node) { return true; }

bool TypeChecker::visit(const ast::ListLiteral& node) {
    node.elements()[0]->accept(*this); // TODO: check to see if its not empty
    TypePtr listType = m_result;

    for (auto& elem : node.elements()) {
        check(elem, *listType);
    }

    m_result =
        TypeProducer::list(listType, std::to_string(node.elements().size()));
    return true;
}

bool TypeChecker::visit(const ast::DictLiteral& node) { return true; }

bool TypeChecker::visit(const ast::ListOrDictAccess& node) { return true; }

bool TypeChecker::visit(const ast::BinaryOperation& node) {
    node.left()->accept(*this);
    TypePtr leftType = m_result;
    node.right()->accept(*this);
    TypePtr result = leftType->infixOperatorResult(node.op(), m_result);

    if (!result) {
        error(node.token(),
              "operator " + node.op().keyword + " can not be used with types " +
                  leftType->stringify() + " and " + m_result->stringify());
    }

    m_result = result;
    return true;
}

bool TypeChecker::visit(const ast::PrefixExpression& node) {
    node.right()->accept(*this);
    TypePtr result = m_result->prefixOperatorResult(node.prefix());

    if (!result) {
        error(node.token(), "operator " + node.prefix().keyword +
                                " can not be used with type " +
                                m_result->stringify());
    }

    m_result = result;
    return true;
}

bool TypeChecker::visit(const ast::PostfixExpression& node) { return true; }

bool TypeChecker::visit(const ast::FunctionCall& node) {
    node.name()->accept(*this);

    if (m_result->category() != TypeCategory::Function)
        error(node.token(), identifierName(node.name()) + " is not a function");

    auto functionType = std::dynamic_pointer_cast<FunctionType>(m_result);

    if (functionType->parameterTypes().size() != node.arguments().size())
        error(node.token(), "invalid number of arguments passed to " +
                                identifierName(node.name()));

    for (size_t i = 0; i < node.arguments().size(); i++) {
        check(node.arguments()[i], *functionType->parameterTypes()[i]);
    }

    m_result = functionType->returnType();
    return true;
}

bool TypeChecker::visit(const ast::DotExpression& node) { return true; }

bool TypeChecker::visit(const ast::ArrowExpression& node) { return true; }

bool TypeChecker::visit(const ast::IdentifierExpression& node) {
    auto identifierType = m_env->get(node.value());

    if (!identifierType ||
        identifierType.value()->category() == TypeCategory::UserDefined) {
        error(node.token(), "undeclared identifier: " + node.value());
    }

    m_result = identifierType.value();
    return true;
}

bool TypeChecker::visit(const ast::TypeExpression& node) {
    if (!identifierToTypeMap.count(node.value())) {
        auto type = m_env->get(node.value());

        if (!type || type.value()->category() != TypeCategory::UserDefined) {
            error(node.token(),
                  node.value() + " is not a type"); // return or not return?
        }

        m_result = type.value();
        return true;
    }

    m_result = identifierToTypeMap[node.value()];
    return true;
}

bool TypeChecker::visit(const ast::ListTypeExpr& node) {
    node.elemType()->accept(*this);
    auto listType = m_result;

    check(node.size(), *TypeProducer::integer());

    m_result = TypeProducer::list(
        listType,
        std::dynamic_pointer_cast<ast::IntegerLiteral>(node.size())->value());
    return true;
}

bool TypeChecker::visit(const ast::DictTypeExpr& node) { return true; }

bool TypeChecker::visit(const ast::FunctionTypeExpr& node) { return true; }

bool TypeChecker::visit(const ast::PointerTypeExpr& node) {
    node.baseType()->accept(*this);
    m_result = TypeProducer::pointer(m_result);
    return true;
}

bool TypeChecker::visit(const ast::NoLiteral& node) {
    m_result = TypeProducer::voidT();
    return true;
}

bool TypeChecker::visit(const ast::IntegerLiteral& node) {
    m_result = TypeProducer::integer();
    return true;
}

bool TypeChecker::visit(const ast::DecimalLiteral& node) {
    m_result = TypeProducer::decimal();
    return true;
}

bool TypeChecker::visit(const ast::StringLiteral& node) {
    m_result = TypeProducer::string();
    return true;
}

bool TypeChecker::visit(const ast::BoolLiteral& node) {
    m_result = TypeProducer::boolean();
    return true;
}

bool TypeChecker::visit(const ast::NoneLiteral& node) {
    m_result = TypeProducer::voidT();
    return true;
}

bool TypeChecker::visit(const ast::UnionLiteral& node) { return true; }

bool TypeChecker::visit(const ast::EnumLiteral& node) { return true; }

bool TypeChecker::visit(const ast::WithStatement& node) { return true; }

bool TypeChecker::visit(const ast::VirtualStatement& node) { return true; }

bool TypeChecker::visit(const ast::CastStatement& node) {
    node.cast_type()->accept(*this);
    TypePtr castType = m_result;
    node.value()->accept(*this);

    if (!m_result->isCastableTo(*castType)) {
        error(node.token(), m_result->stringify() + " can not be casted to " +
                                castType->stringify());
    }

    return true;
}

bool TypeChecker::visit(const ast::DefaultArg& node) { return true; }

bool TypeChecker::visit(const ast::TernaryIf& node) { return true; }

bool TypeChecker::visit(const ast::TryExcept& node) { return true; }
