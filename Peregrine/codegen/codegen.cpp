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

void Codegen::generate(AstNodePtr ast_node) {
    switch (ast_node->type()) {
        case KAstProgram: {
            auto node = std::dynamic_pointer_cast<Program>(ast_node);

            for (auto& stmt : node->statements()) {
                generate(stmt);
            }

            break;
        }

        case KAstInteger: {
            auto node = std::dynamic_pointer_cast<IntegerLiteral>(ast_node);
            write(node->value());
            break;
        }

        case KAstDecimal: {
            auto node = std::dynamic_pointer_cast<DecimalLiteral>(ast_node);
            write(node->value());
            break;
        }

        case KAstBool: {
            auto node = std::dynamic_pointer_cast<BoolLiteral>(ast_node);
            write((node->value() == "True") ? "true" : "false");
            break;
        }

        case KAstBinaryOp: {
            auto node = std::dynamic_pointer_cast<BinaryOperation>(ast_node);

            generate(node->left());
            write(node->op());
            generate(node->right());

            break;
        }

        default: {
            std::cerr << fg(style("Error: invalid ast node passed to generate(). This should never happen.", bold), light_red) << "\n";
            std::cerr << "This is most likely not an issue with the compiler itself. You can seek help at our discord server: https://discord.gg/CAMgzwDJDM" << "\n";
            exit(1);
        }
    }
}