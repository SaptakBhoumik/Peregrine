#include "codegen.hpp"

#include "ast/ast.hpp"
#include "errors/error.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

namespace cpp {

Codegen::Codegen(std::string outputFilename) { m_file.open(outputFilename); }

std::shared_ptr<SymbolTable<ast::AstNodePtr>> Codegen::createEnv(std::shared_ptr<SymbolTable<ast::AstNodePtr>> parent) {
    return std::make_shared<SymbolTable<ast::AstNodePtr>>(parent);
}

void Codegen::flush(std::string_view code) {
    m_file << code;
    m_file.close(); // this will be used only once so we can close it
}

std::string Codegen::mangleName(ast::AstNodePtr astNode) {
    return std::string("");
}

std::string Codegen::generate(ast::AstNodePtr astNode, std::shared_ptr<SymbolTable<ast::AstNodePtr>> env) {
    std::string res;
    
    switch (astNode->type()) {
        case ast::KAstProgram: {
            auto node = std::dynamic_pointer_cast<ast::Program>(astNode);

            for (auto& stmt : node->statements()) {
                res += generate(stmt, env) + ";\n";
            }
            break;
        }

        case ast::KAstInteger: {
            auto node = std::dynamic_pointer_cast<ast::IntegerLiteral>(astNode);
            res += node->value();
            break;
        }
        case ast::KAstLamda:{
            auto node = std::dynamic_pointer_cast<ast::LamdaDefine>(astNode);
            res+="std::function<";
            if (node->fn_return_types().size()==0){
                res+="void(";
            }
            else{
                res+=generate(node->fn_return_types()[0], env)+"(";
            }
            auto x=node->fn_arg_types();
            if(x.size()>0){
                for(size_t i=0;i<x.size();++i){
                    res+=generate(x[i], env);
                    if (i!=x.size()-1){
                        res+=",";
                    }
                }
            }
            res+=")>";
            break;
        }
        case ast::KAstTypeDefinition:{
            auto node = std::dynamic_pointer_cast<ast::TypeDefinition>(astNode);
            res+="typedef "+generate(node->baseType(),env)+" "+generate(node->name(),env);
            break;
        }
        case ast::KAstDecimal: {
            auto node = std::dynamic_pointer_cast<ast::DecimalLiteral>(astNode);
            res += node->value();
            break;
        }
        case ast::KAstString: {
            auto node = std::dynamic_pointer_cast<ast::StringLiteral>(astNode);
            // todo- do this in type check

            res += "\"";
            res += node->value();
            res += "\"";
            break;
        }
        case ast::KAstBool: {
            auto node = std::dynamic_pointer_cast<ast::BoolLiteral>(astNode);
            res += ((node->value() == "True") ? "true" : "false");
            break;
        }
        case ast::KAstPrefixExpr: {
            auto node = std::dynamic_pointer_cast<ast::PrefixExpression>(astNode);
            res += "(" + node->prefix().keyword + " " +
                   generate(node->right(), env) + ")";
            break;
        }
        case ast::KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<ast::BinaryOperation>(astNode);
            auto operation = node->op();
            if (operation.keyword == "**") {
                res += "_PEREGRINE_POWER(" + generate(node->left(), env) + "," +
                       generate(node->right(), env) + ")";
            } else if (operation.keyword == "//") {
                res += "_PEREGRINE_FLOOR(" + generate(node->left(), env) + "/" +
                       generate(node->right(), env) + ")";
            } else {
                res += "(" + generate(node->left(), env) + " " + node->op().keyword +
                       " " + generate(node->right(), env) + ")";
            }
            break;
        }
        case ast::KAstBlockStmt: {
            auto node = std::dynamic_pointer_cast<ast::BlockStatement>(astNode);
            auto body = node->statements();
            for (auto& stmp : body) {
                res += generate(stmp, env) + ";\n";
            }
            break;
        }
        case ast::KAstIfStmt: {
            auto node = std::dynamic_pointer_cast<ast::IfStatement>(astNode);

            res += "if(" + generate(node->condition(), env) + "){\n" +
                   generate(node->ifBody(), createEnv(env)) + "}";

            auto elifNode = node->elifs();
            if (elifNode.size() != 0) {
                res += "\n";
                for (auto& body : elifNode) { // making sure that elif exists
                    res += "else if(" + generate(body.first, env) + "){\n" +
                           generate(body.second, createEnv(env)) + "}";
                }
            }
            auto elseNode = node->elseBody();
            if (elseNode->type() ==
                ast::KAstBlockStmt) { // making sure that else exists
                res += "\nelse{\n" + generate(elseNode, createEnv(env)) + "}";
            }
            break;
        }
        case ast::KAstContinueStatement: {
            res += "continue";
            break;
        }
        case ast::KAstBreakStatement: {
            res += "break";
            break;
        }
        case ast::KAstNone: {
            res += "NULL";
            break;
        }
        case ast::KAstPassStatement: {
            res += "\n//pass";// we are making it a comment because ; is added to
                           // each node at the end. we dont want that to happen
                           // because it will result in ;; which is an error
            break;
        }
        case ast::KAstVariableStmt: {
            auto node = std::dynamic_pointer_cast<ast::VariableStatement>(astNode);
            std::string type;
            if (node->varType()->type() != ast::KAstNoLiteral){
                type=generate(node->varType(), env)+" ";
            }
            std::string name = generate(node->name(), env);
            std::string value;
            if (node->value()->type() != ast::KAstNoLiteral){
                value=" = "+generate(node->value(), env);
            }
            res+=type+name+value;
            break;
        }
        case ast::KAstCpp: {
            auto node = std::dynamic_pointer_cast<ast::CppStatement>(astNode);
            res += node->value() +
                   "\n//"; // we are making it a comment because ; is added to
                           // each node at the end. we dont want that to happen
                           // because it will result in ;; which is an error
            break;
        }
        case ast::KAstWhileStmt: {
            auto node = std::dynamic_pointer_cast<ast::WhileStatement>(astNode);
            res += "while(" + generate(node->condition(), env) + "){\n" +
                   generate(node->body(), createEnv(env)) + "}";
            break;
        }
        case ast::KAstIdentifier: {
            auto node =
                std::dynamic_pointer_cast<ast::IdentifierExpression>(astNode);
            res += node->value();
            break;
        }
        case ast::KAstScopeStmt: {
            auto node = std::dynamic_pointer_cast<ast::ScopeStatement>(astNode);
            res += "{\n" + generate(node->body(), createEnv(env)) + "\n}";
            break;
        }
        case ast::KAstReturnStatement: {
            auto node = std::dynamic_pointer_cast<ast::ReturnStatement>(astNode);
            res += "return ";
            auto value = node->returnValue();
            if (value->type() != ast::KAstNoLiteral) {
                res += generate(value, env);
            }
            break;
        }
        case ast::KAstFunctionCall: {
            auto node = std::dynamic_pointer_cast<ast::FunctionCall>(astNode);
            auto funcName = generate(node->name(), env);
            std::string arg;
            auto x = node->arguments();
            if (x.size() != 0) {
                for (size_t i = 0; i < x.size(); ++i) {
                    arg += generate(x[i], env) + " ";
                    if (i == x.size() - 1) {
                    } else {
                        arg += ",";
                    }
                }
            }
            res += funcName + "(" + arg + ")";
            break;
        }
        case ast::KAstTypeExpr:{
            auto node = std::dynamic_pointer_cast<ast::TypeExpression>(astNode);
            res+=node->value();
            break;
        }
        case ast::KAstFunctionDef: {
            auto node = std::dynamic_pointer_cast<ast::FunctionDefinition>(astNode);
            auto functionName = generate(node->name(), env);
            auto returnType = generate(node->returnType(), env);
            std::string param;
            if (node->parameters().size() > 0) {
                auto x = node->parameters();
                for (size_t i = 0; i < x.size(); ++i) {
                    param +=
                        generate(x[i].p_type, env) + " " + generate(x[i].p_name, env);
                    if (i == x.size() - 1) {
                    } else {
                        param += ",";
                    }
                }
            }
            if (functionName == "main" && returnType == "void") {
                // we want the main function to always return 0 if success
                res += "int main(" + param + "){\n" + generate(node->body(), createEnv(env)) +
                       "return 0;\n}";
            } else {
                res += returnType + " " + functionName + "(" + param + "){\n" +
                       generate(node->body(), createEnv(env)) + "\n}";
            }

            break;
        }

        //TODO: check if we need to createEnv() at some point
        case ast::KAstMatchStmt:{
            auto node = std::dynamic_pointer_cast<ast::MatchStatement>(astNode);
            auto to_match=node->matchItem();
            auto cases=node->caseBody();
            auto defaultbody=node->defaultBody();
            res+="\nwhile (true){\n";
            for (size_t i=0;i<cases.size();++i){
                auto x=cases[i];
                if (x.first.size()==1 && x.first[0]->type() == ast::KAstNoLiteral){
                    if (i==0){
                        res+=generate(x.second, env)+"\n";
                    }
                    else{
                        res+="else{\n"+generate(x.second, env)+"\n}\n";
                    }
                }
                else if (i==0){
                    res+="if ("+matchArg(to_match,x.first)+"){\n"+generate(x.second, env)+"\n}\n";
                }
                else{
                    res+="else if ("+matchArg(to_match,x.first)+"){\n"+generate(x.second, env)+"\n}\n";
                }
            }
            if (defaultbody->type() != ast::KAstNoLiteral){
                res+=generate(defaultbody, env);
            }
            res+="\nbreak;\n}";
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

}