#include "analyzer/typeChecker.hpp"
#include "docgen/html/docgen.hpp"
#include "codegen/cpp/codegen.hpp"
#include "codegen/js/codegen.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

int main(int argc, char** argv) {
    if (argc < 3) { // the cli is still not complete
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

        TypeChecker typeChecker(program);

    } else {
        std::ifstream file(argv[2]);
        std::stringstream buf;
        buf << file.rdbuf();

        std::vector<Token> tokens = lexer(buf.str(), "test");
        // for (auto& token : tokens) {
        //     std::cout << "Keyword= " << token.keyword
        //               << " Type= " << token.tkType << "\n";
        // }
        Parser parser(tokens);
        AstNodePtr program = parser.parse();

        // std::cout << program->stringify() << "\n";

        // TypeChecker typeChecker;
        // typeChecker.check(program);
        std::string filename = argv[2];
        if (argc > 3) {
            if (strcmp(argv[3], "-js") == 0) {
                js::Codegen codegen("index.js", program, false, filename);
            } else if (strcmp(argv[3], "-html") == 0) { // embeds js in html
                js::Codegen codegen("index.html", program, true, filename);
            }
            else if (strcmp(argv[3], "-doc_html") == 0) {
                html::Docgen Docgen("index.html", program,filename);
            }else {
                cpp::Codegen codegen("temp.cc", program, filename);
                system("g++ -std=c++20 temp.cc");
            }
        } else {
            cpp::Codegen codegen("temp.cc", program, filename);
            system("g++ -std=c++20  temp.cc");
        }
    }
    return 0;
}
