#include "codegen.hpp"

#include "ast/ast.hpp"
#include "errors/error.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>
#include <string>
#include <string_view>

namespace cpp {

Codegen::Codegen(std::string outputFilename) { 
    m_file.open(outputFilename);
    m_file << "#include <cstdio>\n#include <functional>\n";
}

std::shared_ptr<SymbolTable<ast::AstNodePtr>> Codegen::createEnv(std::shared_ptr<SymbolTable<ast::AstNodePtr>> parent) {
    return std::make_shared<SymbolTable<ast::AstNodePtr>>(parent);
}

//TODO: buffer it
void Codegen::write(std::string_view code) {
    m_file << code;
}

std::string Codegen::mangleName(ast::AstNodePtr astNode) {
    return std::string("");
}

void Codegen::generate(ast::AstNodePtr astNode, std::shared_ptr<SymbolTable<ast::AstNodePtr>> env) {
    switch (astNode->type()) {
        case ast::KAstProgram: {
            auto node = std::dynamic_pointer_cast<ast::Program>(astNode);

            for (auto& stmt : node->statements()) {
                generate(stmt, env);
                write(";\n"); //TODO: will this break stuff later?
            }
            break;
        }

        case ast::KAstImportStmt: {
            auto node = std::dynamic_pointer_cast<ast::ImportStatement>(astNode);
            codegenImport(node);
            break;
        }

        case ast::KAstInteger: {
            auto node = std::dynamic_pointer_cast<ast::IntegerLiteral>(astNode);
            write(node->value());
            break;
        }
        case ast::KAstFuncTypeExpr:{
            auto node = std::dynamic_pointer_cast<ast::FunctionTypeExpr>(astNode);
            write("std::function<");
            if (node->returnTypes().size()==0){
                write("void(");
            }
            else{
                generate(node->returnTypes()[0], env);
                write("(");
            }
            auto x=node->argTypes();
            if(x.size()>0){
                for(size_t i=0;i<x.size();++i){
                    generate(x[i], env);
                    if (i!=x.size()-1){
                        write(",");
                    }
                }
            }
            write(")>");
            break;
        }
        case ast::KAstTypeDefinition:{
            auto node = std::dynamic_pointer_cast<ast::TypeDefinition>(astNode);
            write("typedef ");
            generate(node->baseType(), env);
            write(" ");
            generate(node->name(), env);
            break;
        }
        case ast::KAstDecimal: {
            auto node = std::dynamic_pointer_cast<ast::DecimalLiteral>(astNode);
            write(node->value());
            break;
        }
        case ast::KAstString: {
            auto node = std::dynamic_pointer_cast<ast::StringLiteral>(astNode);
            // todo- do this in type check

            write("\""+node->value()+"\"");
            break;
        }
        case ast::KAstBool: {
            auto node = std::dynamic_pointer_cast<ast::BoolLiteral>(astNode);
            write(((node->value() == "True") ? "true" : "false"));
            break;
        }
        case ast::KAstPrefixExpr: {
            auto node = std::dynamic_pointer_cast<ast::PrefixExpression>(astNode);
            write("(" + node->prefix().keyword + " "); 
            generate(node->right(), env);
            write(")");
            break;
        }
        case ast::KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<ast::BinaryOperation>(astNode);
            auto operation = node->op();
            if (operation.keyword == "**") {
                write("_PEREGRINE_POWER(");
                generate(node->left(), env);
                write(",");
                generate(node->right(), env);
                write(")");
            } else if (operation.keyword == "//") {
                write("_PEREGRINE_FLOOR(");
                generate(node->left(), env);
                write("/"); 
                generate(node->right(), env);
                write(")");
            } else {
                write("(");
                generate(node->left(), env);
                write(" " + node->op().keyword + " "); 
                generate(node->right(), env);
                write(")");
            }
            break;
        }
        case ast::KAstBlockStmt: {
            auto node = std::dynamic_pointer_cast<ast::BlockStatement>(astNode);
            auto body = node->statements();
            for (auto& stmt : body) {
                generate(stmt, env);
                write(";\n");
            }
            break;
        }
        case ast::KAstIfStmt: {
            auto node = std::dynamic_pointer_cast<ast::IfStatement>(astNode);

            write("if(");
            generate(node->condition(), env);
            write("){\n"); 
            generate(node->ifBody(), createEnv(env));
            write("}");

            auto elifNode = node->elifs();
            if (elifNode.size() != 0) {
                write("\n");
                for (auto& body : elifNode) { // making sure that elif exists
                    write("else if(");
                    generate(body.first, env);
                    write("){\n"); 
                    generate(body.second, createEnv(env));
                    write("}");
                }
            }
            auto elseNode = node->elseBody();
            if (elseNode->type() ==
                ast::KAstBlockStmt) { // making sure that else exists
                write("\nelse{\n"); 
                generate(elseNode, createEnv(env));
                write("}");
            }
            break;
        }
        case ast::KAstContinueStatement: {
            write("continue");
            break;
        }
        case ast::KAstBreakStatement: {
            write("break");
            break;
        }
        case ast::KAstNone: {
            write("NULL");
            break;
        }
        case ast::KAstPassStatement: {
            write("\n//pass");// we are making it a comment because ; is added to
                           // each node at the end. we dont want that to happen
                           // because it will result in ;; which is an error
            break;
        }
        case ast::KAstVariableStmt: {
            auto node = std::dynamic_pointer_cast<ast::VariableStatement>(astNode);
        
            if (node->varType()->type() != ast::KAstNoLiteral){
                generate(node->varType(), env);
                write(" ");
            }

            generate(node->name(), env);
            
            if (node->value()->type() != ast::KAstNoLiteral){
                write(" = ");
                generate(node->value(), env);
            }

            break;
        }
        case ast::KAstCpp: {
            auto node = std::dynamic_pointer_cast<ast::CppStatement>(astNode);

            // we are making it a comment because ; is added to
            // each node at the end. we dont want that to happen
            // because it will result in ;; which is an error
            write(node->value() + "\n//"); 

            break;
        }
        case ast::KAstWhileStmt: {
            auto node = std::dynamic_pointer_cast<ast::WhileStatement>(astNode);
            write("while(");
            generate(node->condition(), env);
            write("){\n");
            generate(node->body(), createEnv(env));
            write("}");
            break;
        }
        case ast::KAstIdentifier: {
            auto node =
                std::dynamic_pointer_cast<ast::IdentifierExpression>(astNode);
            write(node->value());
            break;
        }
        case ast::KAstScopeStmt: {
            auto node = std::dynamic_pointer_cast<ast::ScopeStatement>(astNode);
            write("{\n");
            generate(node->body(), createEnv(env));
            write("\n}");
            break;
        }
        case ast::KAstReturnStatement: {
            auto node = std::dynamic_pointer_cast<ast::ReturnStatement>(astNode);
            write("return ");
            auto value = node->returnValue();
            if (value->type() != ast::KAstNoLiteral) {
                generate(value, env);
            }
            break;
        }
        case ast::KAstFunctionCall: {
            auto node = std::dynamic_pointer_cast<ast::FunctionCall>(astNode);
            generate(node->name(), env);
            write("(");

            auto args = node->arguments();
            if (args.size()) {
                for (size_t i = 0; i < args.size(); ++i) {
                    if (i)
                        write(", ");
                    generate(args[i], env);
                }
            }

            write(")");
            break;
        }
        case ast::KAstTypeExpr:{
            auto node = std::dynamic_pointer_cast<ast::TypeExpression>(astNode);
            write(node->value());
            break;
        }
        case ast::KAstFunctionDef: {
            auto node = std::dynamic_pointer_cast<ast::FunctionDefinition>(astNode);
            auto functionName = 
                std::dynamic_pointer_cast<ast::IdentifierExpression>(node->name())->value();
            
            if (functionName == "main") {
                // we want the main function to always return 0 if success
                write("int main(");
                codegenFuncParams(node->parameters(), env);
                write("){\n");
                generate(node->body(), createEnv(env));
                write("return 0;\n}");
            } else {
                generate(node->returnType(), env);
                write(" ");
                generate(node->name(), env);
                write("(");
                codegenFuncParams(node->parameters(), env);
                write("){\n");
                generate(node->body(), createEnv(env));
                write("\n}");
            }

            break;
        }

        //TODO: check if we need to createEnv() at some point
        case ast::KAstMatchStmt:{
            auto node = std::dynamic_pointer_cast<ast::MatchStatement>(astNode);
            auto to_match=node->matchItem();
            auto cases=node->caseBody();
            auto defaultbody=node->defaultBody();
            write("\nwhile (true){\n");
            for (size_t i=0;i<cases.size();++i){
                auto x=cases[i];
                if (x.first.size()==1 && x.first[0]->type() == ast::KAstNoLiteral){
                    if (i==0){
                        generate(x.second, env);
                        write("\n");
                    }
                    else{
                        write("else{\n");
                        generate(x.second, env);
                        write("\n}\n");
                    }
                }
                else if (i==0){
                    write("if (");
                    matchArg(to_match,x.first);
                    write("){\n");
                    generate(x.second, env);
                    write("\n}\n");
                }
                else{
                    write("else if (");
                    matchArg(to_match,x.first);
                    write("){\n");
                    generate(x.second, env);
                    write("\n}\n");
                }
            }
            if (defaultbody->type() != ast::KAstNoLiteral){
                generate(defaultbody, env);
            }
            write("\nbreak;\n}");
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
}

std::string Codegen::searchDefaultModule(std::string path, std::string moduleName) {
    for (auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::path(entry.path()).filename() == moduleName) {
            //TODO :ignore extensions?
            if (entry.is_regular_file()) {
                return entry.path();
            } else if (entry.is_directory()) {
                //TODO: avoid deeply nested folders
                searchDefaultModule(entry.path(), moduleName);
            }
        }
    }

    return "";
}

void Codegen::codegenImport(std::shared_ptr<ast::ImportStatement> importNode) {
    auto moduleName = 
        std::dynamic_pointer_cast<ast::IdentifierExpression>(importNode->moduleName().first)->value();
    
    std::string filePath = searchDefaultModule("../Peregrine/lib", moduleName);

    if (!filePath.empty()) {
        // in progress
    }
}

void Codegen::codegenFuncParams(std::vector<ast::parameter> parameters, EnvPtr env) {
    if (parameters.size()) {
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i)
                write(", ");

            generate(parameters[i].p_type, env);
            write(" ");
            generate(parameters[i].p_name, env);
        }
    }
}

}