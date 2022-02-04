#include "analyzer/typeChecker.hpp"
#include "docgen/html/docgen.hpp"
#include "codegen/cpp/codegen.hpp"
#include "cli/cli.hpp"
#include "codegen/js/codegen.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include "parser/parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>
void compile(cli::state s){
    if (s.dev_debug){
        std::ifstream file("../Peregrine/test.pe");
        std::stringstream buf;
        buf << file.rdbuf();

        std::vector<Token> tokens = lexer(buf.str(), "test");

        for (auto& token : tokens) {
            std::cout << "Keyword= " << token.keyword
                      << " Type= " << token.tkType <<" Line= "<<token.line<<" Loc="<<token.location<<"\n";
        }
        Parser parser(tokens, "test");
        ast::AstNodePtr program = parser.parse();
        std::cout << program->stringify() << "\n";
        // TypeChecker typeChecker(program);
    }
    else{
        std::ifstream file(s.input_filename);
        std::stringstream buf;
        buf << file.rdbuf();
        std::vector<Token> tokens = lexer(buf.str(), s.input_filename);
        Parser parser(tokens, s.input_filename);
        ast::AstNodePtr program = parser.parse();
        auto output=s.output_filename;
        auto filename=s.input_filename;
        if (s.emit_js){
            js::Codegen codegen(output, program, false, filename);
        }else if(s.emit_html){
            js::Codegen codegen(output, program, true, filename);
        }else if(s.doc_html){
            html::Docgen Docgen(output, program, filename);
        }else if(s.emit_cpp){
            cpp::Codegen codegen(output, program,filename);
        }else if(s.emit_obj){
            cpp::Codegen codegen("temp.cc", program,filename);
            auto cmd=s.cpp_compiler+" -c -std=c++20 temp.cc -w "+s.cpp_arg+" -o "+output;
            system(cmd.c_str());
            system("rm temp.cc");
        }else{
            cpp::Codegen codegen("temp.cc", program,filename);
            auto cmd=s.cpp_compiler+" -std=c++20 temp.cc -w "+s.cpp_arg+" -o "+output;
            system(cmd.c_str());
            system("rm temp.cc");
        }
    }
}
int main(int argc, char** argv) {
    cli::CLI cli(argc, argv);
    cli::state state = cli.parse();
    if (argc== 1) {
        cli::help();
        return 0;
    } else {
        state.validate_state();
        compile(state);
    }
    return 0;
}
