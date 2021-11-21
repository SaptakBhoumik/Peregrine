#include "codegen.hpp"

#include "ast/ast.hpp"
#include "errors/error.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

Codegen::Codegen(std::string outputFilename) { m_file.open(outputFilename); }

void Codegen::write(std::string_view code) {
    m_file << code;
    m_file.close(); // this will be used only once so we can close it
}

std::string Codegen::generate(AstNodePtr astNode) {
    std::string res;
    switch (astNode->type()) {
        case KAstProgram: {
            auto node = std::dynamic_pointer_cast<Program>(astNode);

            for (auto& stmt : node->statements()) {
                res += generate(stmt) + ";\n";
            }
            break;
        }

        case KAstInteger: {
            auto node = std::dynamic_pointer_cast<IntegerLiteral>(astNode);
            res += node->value();
            break;
        }

        case KAstDecimal: {
            auto node = std::dynamic_pointer_cast<DecimalLiteral>(astNode);
            res += node->value();
            break;
        }
        case KAstString: {
            auto node = std::dynamic_pointer_cast<StringLiteral>(astNode);
            // todo- do this in type check

            res += "\"";
            res += node->value();
            res += "\"";
            break;
        }
        case KAstBool: {
            auto node = std::dynamic_pointer_cast<BoolLiteral>(astNode);
            res += ((node->value() == "True") ? "true" : "false");
            break;
        }
        case KAstPrefixExpr: {
            auto node = std::dynamic_pointer_cast<PrefixExpression>(astNode);
            res += "(" + node->prefix().keyword + " " +
                   generate(node->right()) + ")";
            break;
        }
        case KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<BinaryOperation>(astNode);
            auto operation = node->op();
            if (operation.keyword == "**") {
                res += "_PEREGRINE_POWER(" + generate(node->left()) + "," +
                       generate(node->right()) + ")";
            } else if (operation.keyword == "//") {
                res += "_PEREGRINE_FLOOR(" + generate(node->left()) + "/" +
                       generate(node->right()) + ")";
            } else {
                res += "(" + generate(node->left()) + " " + node->op().keyword +
                       " " + generate(node->right()) + ")";
            }
            break;
        }
        case KAstBlockStmt: {
            auto node = std::dynamic_pointer_cast<BlockStatement>(astNode);
            auto body = node->statements();
            for (auto& stmp : body) {
                res += generate(stmp) + ";\n";
            }
            break;
        }
        case KAstIfStmt: {
            auto node = std::dynamic_pointer_cast<IfStatement>(astNode);
            res += "if(" + generate(node->condition()) + "){\n" +
                   generate(node->ifBody()) + "}";
            auto elifNode = node->elifs();
            if (elifNode.size() != 0) {
                res += "\n";
                for (auto& body : elifNode) { // making sure that elif exists
                    res += "else if(" + generate(body.first) + "){\n" +
                           generate(body.second) + "}";
                }
            }
            auto elseNode = node->elseBody();
            if (elseNode->type() ==
                KAstBlockStmt) { // making sure that else exists
                res += "\nelse{\n" + generate(elseNode) + "}";
            }
            break;
        }
        case KAstContinueStatement: {
            res += "continue";
            break;
        }
        case KAstBreakStatement: {
            res += "break";
            break;
        }
        case KAstVariableStmt: {
            auto node = std::dynamic_pointer_cast<VariableStatement>(astNode);
            std::string type;
            if (node->varType()->type()!=KAstNoLiteral){
                type=generate(node->varType())+" ";
            }
            std::string name = generate(node->name());
            std::string value;
            if (node->value()->type()!=KAstNoLiteral){
                value=" = "+generate(node->value());
            }
            res+=type+name+value;
            break;
        }
        case KAstCpp: {
            auto node = std::dynamic_pointer_cast<CppStatement>(astNode);
            res += node->value() +
                   "\n//"; // we are making it a comment because ; is added to
                           // each node at the end. we dont want that to happen
                           // because it will result in ;; which is an error
            break;
        }
        case KAstWhileStmt: {
            auto node = std::dynamic_pointer_cast<WhileStatement>(astNode);
            res += "while(" + generate(node->condition()) + "){\n" +
                   generate(node->body()) + "}";
            break;
        }
        case KAstIdentifier: {
            auto node =
                std::dynamic_pointer_cast<IdentifierExpression>(astNode);
            res += node->value();
            break;
        }
        case KAstScopeStmt: {
            auto node = std::dynamic_pointer_cast<ScopeStatement>(astNode);
            res += "{\n" + generate(node->body()) + "\n}";
            break;
        }
        case KAstReturnStatement: {
            auto node = std::dynamic_pointer_cast<ReturnStatement>(astNode);
            res += "return ";
            auto value = node->returnValue();
            if (value->type() != KAstNoLiteral) {
                res += generate(value);
            }
            break;
        }
        case KAstFunctionCall: {
            auto node = std::dynamic_pointer_cast<FunctionCall>(astNode);
            auto funcName = generate(node->name());
            std::string arg;
            auto x = node->arguments();
            if (x.size() != 0) {
                for (uint64_t i = 0; i < x.size(); ++i) {
                    arg += generate(x[i]) + " ";
                    if (i == x.size() - 1) {
                    } else {
                        arg += ",";
                    }
                }
            }
            res += funcName + "(" + arg + ")";
            break;
        }
        case KAstTypeExpr:{
            auto node = std::dynamic_pointer_cast<TypeExpression>(astNode);
            res+=node->value();
            break;
        }
        case KAstFunctionDef: {
            auto node = std::dynamic_pointer_cast<FunctionDefinition>(astNode);
            auto functionName = generate(node->name());
            auto returnType = generate(node->returnType());
            std::string param;
            if (node->parameters().size() > 0) {
                auto x = node->parameters();
                for (uint64_t i = 0; i < x.size(); ++i) {
                    param +=
                        generate(x[i].p_type) + " " + generate(x[i].p_name);
                    if (i == x.size() - 1) {
                    } else {
                        param += ",";
                    }
                }
            }
            if (functionName == "main" && returnType == "void") {
                // we want the main function to always return 0 if success
                res += "int main(" + param + "){\n" + generate(node->body()) +
                       "return 0;\n}";
            } else {
                res += returnType + " " + functionName + "(" + param + "){\n" +
                       generate(node->body()) + "\n}";
            }

            break;
        }
        default: {
            std::cout<<astNode->type()<<"\n";
            std::cerr << fg(style("Error: invalid ast node passed to "
                                  "generate(). This should never happen.",
                                  bold),
                            light_red)
                      << "\n";
            std::cerr << "This is most likely an issue with the compiler "
                         "itself. You can seek help at our discord server: "
                         "https://discord.gg/CAMgzwDJDM"
                      << "\n";
            exit(1);
        }
    }
    return res;
}