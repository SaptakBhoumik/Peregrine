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
            auto type = m_env->get(node->value());

            if (!type) 
                //panic, undefined identifier

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

        case KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<BinaryOperation>(astNode);

            TypePtr type1 = check(node->left());
            TypePtr type2 = check(node->right());

            TypePtr result = type1->infixOperatorResult(node->op(), type2);

            if (!result) {
                error(node->op().keyword + " can not be used with types " + type1->stringify() + " and" + type2->stringify());
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

        default:
            break;
    }

    return nullptr;
}