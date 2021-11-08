#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {
    std::ifstream file("./Peregrine/test.pe");
    std::stringstream buf;
    buf << file.rdbuf();

    std::vector<Token> tokens = lexer(buf.str(), "test");
    
    for (auto& tok : tokens) {
        std::cout << tok.keyword << " " << tok.tk_type << "\n";
    }

    Parser parser(tokens);

    AstNodePtr program = parser.parse();
    
    std::cout << program->stringify() << "\n";

    return 0;
}