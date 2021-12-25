#include "doctest.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <lexer/lexer.hpp>
#include <lexer/tokens.hpp>
#include <parser/parser.hpp>
#include <ast/ast.hpp>

std::vector<Token> lexFile(const char* filename) {
    std::ifstream file(filename);
    std::stringstream buf;
    buf << file.rdbuf();

    return lexer(buf.str(), filename);
}

TEST_SUITE_BEGIN("Parser");

TEST_CASE("Parse binary expressions") {
    Parser parser(lexFile("../bin_expr.pe"));
    AstNodePtr program = parser.parse();

    auto programNode = std::dynamic_pointer_cast<Program>(program);

    //TODO: implement it
}

TEST_SUITE_END();




