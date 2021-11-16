#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include "analyzer/typeChecker.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {
    std::ifstream file("./Peregrine/test.pe");
    std::stringstream buf;
    buf << file.rdbuf();

    std::vector<Token> tokens = lexer(buf.str(), "test");
    
    for (auto& token : tokens) {
        std::cout <<"Keyword= "<< token.keyword << " Type= "<< token.tkType<< "\n";
    }

    Parser parser(tokens);
    AstNodePtr program = parser.parse();

    TypeChecker typeChecker;
    typeChecker.check(program);

    std::cout << program->stringify() << "\n";

    Codegen codegen("temp.cc");
    auto res=codegen.generate(program);
    std::cout<<res<<"\n";
    codegen.write(res);
    return 0;
}