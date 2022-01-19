#include "cli.hpp"
#define println(x) std::cout << x << "\n"
namespace cli{ 
    void help() {
        println("Peregrine Compiler (C) The Peregrine Organisation, MPL-2.0");
        println("Usage: peregrine [command] [options] [file] -o [output file]\n");
        println("Peregrine Commands:");
        println("\tcompile          - compiles a given file");
        println("\thelp             - prints out help");
        println("\nPeregrine Options:");
        println("\t-cc              - select the c++ compiler with which you want to compile the resultant code");
        println("\t-cc_flag         - add flags with which you want to compile the generated c++ code");
        println("\t-emit_cpp        - generates C++ code and exits (skips C++ compilation phase)");
        println("\t-obj             - generates object file");
        println("\t-js              - generates javascript code");
        println("\t-html            - generates javascript code and embeds it in html");
        println("\t-doc_html        - generates html docs for a module");
        println("\t-o <output file> - select the output file");
        println("\nExample:");
        println("\tperegrine compile example.pe -o example");
    }
    CLI::CLI(int argc, char** argv){
        for (int i = 1; i < argc; ++i) {
            args.push_back(argv[i]);
        }
        args.push_back("");
        curr_arg = args[m_index];
    }
    void CLI::advance(){
        m_index++;
        curr_arg = args[m_index];
    }
    state CLI::parse(){
        while (curr_arg!=""){
            if (curr_arg=="-cc"){
                advance();
                checkargs("C++ compiler");
                m_state.cpp_compiler = curr_arg;
            }else if (curr_arg=="-cc_flag"){
                advance();
                checkargs("C++ compiler flag");
                m_state.cpp_arg = curr_arg;
            }else if (curr_arg=="-emit_cpp"){
                m_state.emit_cpp = true;
            }else if (curr_arg=="-obj"){
                m_state.emit_obj = true;
            }else if (curr_arg=="-js"){
                m_state.emit_js = true;
            }else if (curr_arg=="-html"){
                m_state.emit_html = true;
            }else if (curr_arg=="-doc_html"){
                m_state.doc_html = true;
            }else if (curr_arg=="-o"){
                advance();
                checkargs("output file");
                m_state.output_filename = curr_arg;
            }else if(curr_arg=="compile"){
                advance();
                checkargs("input file");
                m_state.input_filename = curr_arg;
            }else if(curr_arg=="-dev_debug"){
                m_state.dev_debug = true;
            }else if(curr_arg=="help"){
                help();
                exit(0);
            }else{
                if (curr_arg.size() > 3){
                    if (curr_arg.substr(curr_arg.size()-3, 3)==".pe"){
                        m_state.input_filename = curr_arg;
                    }
                    else{
                        println("Invalid argument: " + curr_arg+"\nUse 'peregrine help' for more information");
                        exit(1);
                    }
                }
                else{
                    println("Invalid argument: " + curr_arg+"\nUse 'peregrine help' for more information");
                    exit(1);
                }
            }
            advance();
        }
        return m_state;
    }
    void CLI::checkargs(std::string s){
        if(curr_arg==""){
            println("No "+s+" specified.\nUse 'peregrine help' for more information");
            exit(1);
        }
    }
    void CLI::validate_state(){
        if (m_state.input_filename==""){
            println("No input file specified.\nUse 'peregrine help' for more information");
            exit(1);
        }
        if(m_state.output_filename==""){
            if(m_state.emit_cpp){
                m_state.output_filename=m_state.input_filename.substr(0, m_state.input_filename.size()-3)+"cpp";
            }
            if(m_state.emit_js){
                m_state.output_filename=m_state.input_filename.substr(0, m_state.input_filename.size()-3)+"o";
            }
        }
    }
}