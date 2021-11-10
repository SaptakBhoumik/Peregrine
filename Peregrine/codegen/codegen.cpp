#include "codegen.hpp"

#include "ast/ast.hpp"
#include "errors/error.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

Codegen::Codegen(std::string output_filename) {
    m_file = std::ofstream(output_filename);
}

void Codegen::write(std::string_view code) { m_file << code; }

std::string Codegen::generate(AstNodePtr ast_node) {
    std::string res;
    switch (ast_node->type()) {
        case KAstProgram: {
            auto node = std::dynamic_pointer_cast<Program>(ast_node);

            for (auto& stmt : node->statements()) {
                res+=generate(stmt)+";\n";
            }
            break;
        }

        case KAstInteger: {
            auto node = std::dynamic_pointer_cast<IntegerLiteral>(ast_node);
            res+=node->value();
            break;
        }

        case KAstDecimal: {
            auto node = std::dynamic_pointer_cast<DecimalLiteral>(ast_node);
            res+=node->value();
            break;
        }

        case KAstBool: {
            auto node = std::dynamic_pointer_cast<BoolLiteral>(ast_node);
            res+=((node->value() == "True") ? "true" : "false");
            break;
        }

        case KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<BinaryOperation>(ast_node);
            res+="("+generate(node->left())+node->op()+generate(node->right())+")";
            break;
        }
        case KAstBlockStmt:{
            auto node = std::dynamic_pointer_cast<BlockStatement>(ast_node);
            auto body = node->statements();
            for (auto& stmp : body){
                res+=generate(stmp)+";\n";
            }
            break;
        }
        case KAstIfStmt:{
            auto node = std::dynamic_pointer_cast<IfStatement>(ast_node);
            res += "if("+generate(node->condition())+"){\n"+generate(node->if_body())+"}";
            auto elif_node=node->elifs();
            if (elif_node.size()!=0){
                res+="\n";
                for (auto& body: elif_node){//making sure that elif exists
                    res+="else if("+generate(body.first)+"){\n"+generate(body.second)+"}";
                }
            }
            auto else_node=node->else_body();
            if (else_node->type()==KAstBlockStmt){//making sure that else exists
                res+="\nelse{\n"+generate(else_node)+"}";
            }
            break;
        }
        case KAstWhileStmt:{
            auto node = std::dynamic_pointer_cast<WhileStatement>(ast_node);
            res+="while("+generate(node->condition())+"){\n"+generate(node->body())+"}";
            break;
        }
        default: {
            std::cerr << fg(style("Error: invalid ast node passed to generate(). This should never happen.", bold), light_red) << "\n";
            std::cerr << "This is most likely not an issue with the compiler itself. You can seek help at our discord server: https://discord.gg/CAMgzwDJDM" << "\n";
            exit(1);
        }
    }
    return res;
}