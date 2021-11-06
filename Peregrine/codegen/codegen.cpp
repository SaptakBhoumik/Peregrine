#include "codegen.hpp"

std::string CodeGen::codegen() {
    std::string res;
    for (auto node : ast.root) {
        res += gen(node);
    }
    return res;
}

std::string CodeGen::gen(AstNode node) {
    std::string res;
    switch (node.kind) {
        case AST_OPERATOR: {
            res = gen_operator(node);
            break;
        }
        case AST_STRING: {
            // TODE: Check if formatted or raw string
            res = node.token.keyword;
            break;
        }
        case AST_NUMBER: {
            res = node.token.keyword;
            break;
        }
        case AST_VAR: {
            res = node.token.keyword;
            break;
        }
        case AST_BOOL: {
            res = node.token.keyword;
            break;
        }
        case AST_NONE: {
            res = "NULL";
            break;
        }
        case AST_CPP: {
            res = node.token.keyword;
            break;
        }
        case AST_INFIX: {
            res = gen_infix(node);
            break;
        }
        default: {
            // do something
        }
    }
    return res;
}