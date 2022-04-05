#ifndef PEREGRINE_CLI_HPP
#define PEREGRINE_CLI_HPP
#include <iostream>
#include <string>
#include <vector>
namespace cli{
void help();
class state{
    public:
    std::string cpp_compiler="";
    std::string input_filename="";
    std::string output_filename="";
    std::string cpp_arg="";
    bool has_main=false;
    bool emit_cpp=false;
    bool emit_obj=false;
    bool emit_js=false;
    bool emit_html=false;
    bool doc_html=false;
    bool dev_debug=false;//Will be removed later. It is for debugging the parser
    void validate_state();
};
class CLI{
    std::vector<std::string> args;
    std::string curr_arg;
    state m_state;
    int m_index=0;
    void advance();
    void checkargs(std::string s);
    public:
    CLI(int argc, char** argv);
    state parse();
};

}
#endif