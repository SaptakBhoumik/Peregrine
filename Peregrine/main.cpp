#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<Token> tokens = lexer("(54 + 2) * 34/6", "");

    for (auto token : tokens) {
        std::cout <<"Keyword= "<< token.keyword << " Type= "<< token.tk_type<< "\n";
    }
    Parser parser(tokens);
    AstNodePtr program = parser.parse();

    std::cout << program->stringify() << "\n";
    tokens = lexer("if True:\r\n    a", "test");
    for (auto token : tokens) {
        std::cout <<"Keyword= "<< token.keyword << " Type= "<< token.tk_type<< "\n";
    }
    return 0;
}