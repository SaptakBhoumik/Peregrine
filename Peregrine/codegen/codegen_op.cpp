#include "codegen.hpp"
#include <iostream>
#include <streambuf>
#include <string>

std::string CodeGen::gen_operator(AstNode curr_node) {
    std::string res;
    auto curr_operator = curr_node.token;
    switch (curr_operator.tk_type) {
        case tk_and: {
            curr_operator.keyword = "&&";
            break;
        }
        case tk_or: {
            curr_operator.keyword = "||";
            break;
        }
        default: {
            // do nothing
        }
    }
    static std::string left;
    static std::string right;
    left = gen(*curr_node.children.operator_op.left);
    right = gen(*curr_node.children.operator_op.right);
    switch (curr_operator.tk_type) {
        case tk_assign: {
            res = left + curr_operator.keyword + right;
            break;
        }
        case tk_exponent: {
            res = "_PEREGRINE_POWER(" + left + "," + right + ")";
            break;
        }
        default: {
            res = "(" + left + curr_operator.keyword + right + ")";
        }
    }
    return res;
}

std::string CodeGen::gen_infix(AstNode curr_node) {
    std::string res;
    auto curr_operator = curr_node.token;
    static std::string child;
    child = gen(*curr_node.children.infix.child);
    if (curr_operator.tk_type == tk_not) {
        res = "(!" + child + ")";
    } else {
        res = "(" + curr_operator.keyword + child + ")";
    }
    return res;
}