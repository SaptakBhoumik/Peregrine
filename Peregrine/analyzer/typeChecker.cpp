#include "typeChecker.hpp"
#include "ast/ast.hpp"
#include "ast/types.hpp"

#include <iostream>
#include <memory>

TypeChecker::TypeChecker() {
    m_env = std::make_unique<SymbolTable<TypePtr>>(nullptr);
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

void TypeChecker::expectType(TypePtr expected, TypePtr obtained) {
    if (expected->category() != obtained->category()) {
        if (!expected->isConvertibleTo(obtained) && !obtained->isConvertibleTo(expected)) {
            error("expected type " + expected->stringify() + ", got " + obtained->stringify() + " instead");
        }
    }
}

TypePtr TypeChecker::check(AstNodePtr astNode) {
    std::cout << "check" << "\n";
    switch (astNode->type()) {
        case KAstProgram: {
            auto node = std::dynamic_pointer_cast<Program>(astNode);

            for (auto& stmt : node->statements()) {
                check(stmt);
            }

            break;    
        }

        case KAstBlockStmt: {
            auto node = std::dynamic_pointer_cast<BlockStatement>(astNode);

            for (auto& stmt : node->statements()) {
                check(stmt);
            }

            break;   
        }

        case KAstIdentifier: {
            auto node = std::dynamic_pointer_cast<IdentifierExpression>(astNode);
            std::cout << node->value() << "\n";
            auto type = m_env->get(node->value());
           
            if (!type) 
                std::cerr << "no type found err" << "\n";

            return *type;
        }

        case KAstTypeExpr: {
            auto node = std::dynamic_pointer_cast<TypeExpression>(astNode);
            return identifierToTypeMap[node->value()];
        }

        case KAstInteger: {
            return TypeList::integer32();
        }

        case KAstString: {
            return TypeList::string();
        }

        case KAstDecimal: {
            return TypeList::decimal();
        }

        case KAstPrefixExpr: {
            auto node = std::dynamic_pointer_cast<PrefixExpression>(astNode);

            TypePtr type = check(node->right());

            TypePtr result = type->prefixOperatorResult(node->prefix());

            if (!result) {
                error(node->prefix().keyword + " can not be used with type " + type->stringify());
            }

            return result;
        }

        case KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<BinaryOperation>(astNode);

            TypePtr type1 = check(node->left());
            TypePtr type2 = check(node->right());
            
            TypePtr result = type1->infixOperatorResult(node->op(), type2);

            if (!result) {
                error(node->op().keyword + " can not be used with types " + type1->stringify() + " and " + type2->stringify());
            }

            return result;
        } 
        
        case KAstVariableStmt: {
            auto node = std::dynamic_pointer_cast<VariableStatement>(astNode);

            if (node->value()->type() == KAstNone) 
                break;

            if (node->varType()->type() == KAstNone) {
                //infer the type
            }

            TypePtr varType = check(node->varType());
            TypePtr valueType = check(node->value());

            expectType(varType, valueType);

            m_env->set(std::dynamic_pointer_cast<IdentifierExpression>(node->name())->value(), 
                        varType);

            break;
        }

        case KAstIfStmt: {
            auto node = std::dynamic_pointer_cast<IfStatement>(astNode);

            expectType(check(node->condition()), std::make_shared<BoolType>()); //TODO: improve

            check(node->ifBody());

            for (auto& elif : node->elifs()) {
                expectType(check(elif.first), std::make_shared<BoolType>()); //TODO: improve
                check(elif.second);
            } 

            check(node->elseBody());

            break;
        }

        case KAstWhileStmt: {
            auto node = std::dynamic_pointer_cast<WhileStatement>(astNode);

            expectType(check(node->condition()), std::make_shared<BoolType>()); //TODO: improve

            break;
        }

        case KAstFunctionDef: {
            auto node = std::dynamic_pointer_cast<FunctionDefinition>(astNode);

            check(node->body());

            std::vector<TypePtr> parameterTypes;
            parameterTypes.reserve(node->parameters().size());

            for (auto& param : node->parameters()) {
                parameterTypes.push_back(check(param.p_type));
            }

            TypePtr returnType = check(node->returnType());

            auto functionType = std::make_shared<FunctionType>(parameterTypes, returnType);
            m_env->set(std::dynamic_pointer_cast<IdentifierExpression>(node->name())->value(), 
                        functionType);

            break;
        }

        case KAstFunctionCall: {
            auto node = std::dynamic_pointer_cast<FunctionCall>(astNode);

            TypePtr nameType = check(node->name());

            if (nameType->category() != TypeCategory::Function) {
                //panic, the name is not a function
                std::cerr << "identifier is not a function" << "\n";
            }

            auto functionType = std::dynamic_pointer_cast<FunctionType>(nameType);

            std::vector<TypePtr> argumentTypes;
            argumentTypes.reserve(node->arguments().size());

            for (auto& arg : node->arguments()) {
                argumentTypes.push_back(check(arg));
            }

            if (functionType->parameterTypes().size() != argumentTypes.size()) {
                //panic, invalid number of arguments passed
                std::cerr << "invalid number of args passed" << "\n";
            }

            for (size_t i = 0; i < argumentTypes.size(); i++) {
                expectType(functionType->parameterTypes()[i], argumentTypes[i]);
            }

            return functionType->returnType();
        }

        default:
            break;
    }

    return nullptr;
}