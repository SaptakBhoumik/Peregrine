#include <iostream>
#include <vector>
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"

int main() {
    std::vector<Token> res = lexer("\"Hello, world!\"", "");

    for (auto& tok : res) {
        std::cout << "Statement: " << tok.statement << " | Keyword: " << tok.keyword << " | Type: " << tok.tk_type << "\n";
    }

    return 0;
}