#include "typeChecker.hpp"
#include "ast/ast.hpp"
#include "ast/types.hpp"

#include <assert.h>
#include <iostream>
#include <memory>

using namespace types;

TypeChecker::TypeChecker() {
    m_currentFunction = nullptr;
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

void TypeChecker::check(EnvPtr env, ast::AstNodePtr expr, TypePtr expectedType) {
    if (expected->category() != obtained->category()) {
        if (!expected->isConvertibleTo(obtained) &&
            !obtained->isConvertibleTo(expected)) {
            error("expected type " + expected->stringify() + ", got " +
                  obtained->stringify() + " instead");
        }
    }
}

void TypeChecker::enterLocalEnv() {
    m_env = std::make_shared<SymbolTable<TypePtr>>(m_env);
}

void TypeChecker::exitLocalEnv() {
    if (!m_env->parent())
        std::cerr << "no parent bruh"
                  << "\n";
    m_env = m_env->parent();
}

std::string TypeChecker::identifierName(ast::AstNodePtr identifier) {
    assert(identifier->type() == ast::KAstIdentifier);

    return std::dynamic_pointer_cast<ast::IdentifierExpression>(identifier)->value();
}

TypePtr TypeChecker::check(ast::AstNodePtr astNode) {
    std::cout << "check"
              << "\n";
    switch (astNode->type()) {
        case ast::KAstProgram: {
            auto node = std::dynamic_pointer_cast<ast::Program>(astNode);

            for (auto& stmt : node->statements()) {
                check(stmt);
            }

            break;
        }

        case ast::KAstBlockStmt: {
            auto node = std::dynamic_pointer_cast<ast::BlockStatement>(astNode);

            for (auto& stmt : node->statements()) {
                check(stmt);
            }

            break;
        }

        case ast::KAstIdentifier: {
            auto node =
                std::dynamic_pointer_cast<ast::IdentifierExpression>(astNode);
            std::cout << node->value() << "\n";
            auto type = m_env->get(node->value());

            if (!type)
                std::cerr << "no type found err"
                          << "\n";

            return *type;
        }

        case ast::KAstTypeExpr: {
            auto node = std::dynamic_pointer_cast<ast::TypeExpression>(astNode);
            return identifierToTypeMap[node->value()];
        }

        case ast::KAstInteger: {
            return TypeProducer::integer();
        }

        case ast::KAstString: {
            return TypeProducer::string();
        }

        case ast::KAstDecimal: {
            return TypeProducer::decimal();
        }

        case ast::KAstNone: {
            return TypeProducer::none();
        }

        case ast::KAstPrefixExpr: {
            auto node = std::dynamic_pointer_cast<ast::PrefixExpression>(astNode);

            TypePtr type = check(node->right());

            TypePtr result = type->prefixOperatorResult(node->prefix());

            if (!result) {
                error(node->prefix().keyword + " can not be used with type " +
                      type->stringify());
            }

            return result;
        }

        case ast::KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<ast::BinaryOperation>(astNode);

            TypePtr type1 = check(node->left());
            TypePtr type2 = check(node->right());

            TypePtr result = type1->infixOperatorResult(node->op(), type2);

            if (!result) {
                error(node->op().keyword + " can not be used with types " +
                      type1->stringify() + " and " + type2->stringify());
            }

            return result;
        }

        case ast::KAstVariableStmt: {
            auto node = std::dynamic_pointer_cast<ast::VariableStatement>(astNode);

            if (node->value()->type() == ast::KAstNone)
                break;

            if (node->varType()->type() == ast::KAstNone) {
                // infer the type
            }

            TypePtr varType = check(node->varType());
            TypePtr valueType = check(node->value());

            expectType(varType, valueType);

            m_env->set(
                std::dynamic_pointer_cast<ast::IdentifierExpression>(node->name())
                    ->value(),
                varType);

            break;
        }

        case ast::KAstIfStmt: {
            auto node = std::dynamic_pointer_cast<ast::IfStatement>(astNode);

            expectType(check(node->condition()), TypeProducer::boolean());

            enterLocalEnv();
            check(node->ifBody());
            exitLocalEnv();

            for (auto& elif : node->elifs()) {
                expectType(check(elif.first), TypeProducer::boolean());

                enterLocalEnv();
                check(elif.second);
                exitLocalEnv();
            }

            enterLocalEnv();
            check(node->elseBody());
            exitLocalEnv();

            break;
        }

        case ast::KAstWhileStmt: {
            auto node = std::dynamic_pointer_cast<ast::WhileStatement>(astNode);

            expectType(check(node->condition()), TypeProducer::boolean());

            enterLocalEnv();
            check(node->body());
            exitLocalEnv();

            break;
        }

        case ast::KAstFunctionDef: {
            auto node = std::dynamic_pointer_cast<ast::FunctionDefinition>(astNode);

            std::vector<TypePtr> parameterTypes;
            parameterTypes.reserve(node->parameters().size());

            enterLocalEnv();

            for (auto& param : node->parameters()) {
                TypePtr paramType = check(param.p_type);
                parameterTypes.push_back(paramType);
                m_env->set(identifierName(param.p_name), paramType);
            }

            TypePtr returnType = check(node->returnType());

            auto functionType =
                std::make_shared<FunctionType>(parameterTypes, returnType);

            m_currentFunction = functionType;
            check(node->body());
            m_currentFunction = nullptr;

            exitLocalEnv();

            m_env->set(
                std::dynamic_pointer_cast<ast::IdentifierExpression>(node->name())
                    ->value(),
                functionType);

            break;
        }

        case ast::KAstFunctionCall: {
            auto node = std::dynamic_pointer_cast<ast::FunctionCall>(astNode);

            TypePtr nameType = check(node->name());

            if (nameType->category() != TypeCategory::Function) {
                // panic, the name is not a function
                std::cerr << "identifier is not a function"
                          << "\n";
            }

            auto functionType =
                std::dynamic_pointer_cast<FunctionType>(nameType);

            std::vector<TypePtr> argumentTypes;
            argumentTypes.reserve(node->arguments().size());

            for (auto& arg : node->arguments()) {
                argumentTypes.push_back(check(arg));
            }

            if (functionType->parameterTypes().size() != argumentTypes.size()) {
                // panic, invalid number of arguments passed
                std::cerr << "invalid number of args passed"
                          << "\n";
            }

            for (size_t i = 0; i < argumentTypes.size(); i++) {
                expectType(functionType->parameterTypes()[i], argumentTypes[i]);
            }

            return functionType->returnType();
        }

        case ast::KAstReturnStatement: {
            auto node = std::dynamic_pointer_cast<ast::ReturnStatement>(astNode);

            if (!m_currentFunction) {
                error("can not use return outside of a function");
            }

            TypePtr returnType = check(node->returnValue());

            expectType(m_currentFunction->returnType(), returnType);

            break;
        }

        default:
            std::cerr << "enjoy your segfault" << "\n";
            break;
    }

    return nullptr;
}