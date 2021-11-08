#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<Token> tokens = lexer("(54 + 2) * 34 / 4", "");

    Parser parser(tokens);

    AstNodePtr program = parser.parse();
    
    std::cout << program->stringify() << "\n";

    return 0;
}