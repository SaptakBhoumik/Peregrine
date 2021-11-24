#include "codegen/cpp/codegen.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"
#include "analyzer/typeChecker.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 3) {//the cli is still not complete
        std::ifstream file("../Peregrine/test.pe");
        std::stringstream buf;
        buf << file.rdbuf();

        std::vector<Token> tokens = lexer(buf.str(), "test");

        for (auto& token : tokens) {
            std::cout << "Keyword= " << token.keyword
                      << " Type= " << token.tkType << "\n";
        }

        Parser parser(tokens);
        AstNodePtr program = parser.parse();

        std::cout << program->stringify() << "\n";

        TypeChecker typeChecker;
        typeChecker.check(program);

        cpp::Codegen codegen("temp.cc");
        auto res = codegen.generate(program, codegen.createEnv());
        std::cout << res << "\n";
        codegen.flush(res);
    }
    else{
        std::ifstream file(argv[2]);
        std::stringstream buf;
        buf << file.rdbuf();

        std::vector<Token> tokens = lexer(buf.str(), "test");
        Parser parser(tokens);
        AstNodePtr program = parser.parse();

        // std::cout << program->stringify() << "\n";

        // TypeChecker typeChecker;
        // typeChecker.check(program);

        cpp::Codegen codegen("temp.cc");
        auto res = "#include  <cstdio>\n"+codegen.generate(program, codegen.createEnv());
        codegen.flush(res);
        system("g++ temp.cc");
    }
    return 0;
}