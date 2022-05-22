#include "error.hpp"
#include <iostream>
#include <map>
#include <string>
std::string add_space(std::string& str,size_t s){
    std::string res;
    if(str.size()>(s+1)){
        for(auto& c:str){
            if(c=='\t'){
                res+='\t';
            }
            else{
                res+=' ';
            }
        }
    }
    else{
        for(size_t i=0;i<(s-1);++i){
            if(str[i]=='\t'){
                res+='\t';
            }
            else{
                res+=' ';
            }
        }
    }
    return res;
};
std::string fg(std::string text, std::string color) {
    return prefix + color + suffix + text + reset;
}

std::string style(std::string text, std::string color) {
    return prefix + color + suffix + text + reset;
}

void display(PEError e) {
    std::cout << "  ╭- "
              << fg(style("Error ---------------------------------------- " +
                              e.loc.file + ":" + std::to_string(e.loc.line) +
                              ":" + std::to_string(e.loc.col),
                          bold),
                    light_red)
              << "\n";
    std::cout << "  | " << fg(style(e.msg, bold), light_red) << "\n";
    std::cout << "  |"
              << "\n";
    std::cout << "  |"
              << "\n";
    std::cout << "  |"
              << "\n";
    std::cout << std::to_string(e.loc.line) << " | "
              << e.loc.code.substr(0, e.loc.code.length()) << "\n";
    std::cout << "  |";
    std::cout << add_space(e.loc.code,e.loc.loc);
    std::cout << fg(style(" ^----- " + e.submsg, bold), light_red) << "\n";
    std::cout << "  |\n  |\n";
    if (e.hint != "") {
        std::cout << "  ├- " << fg(style("Try: ", bold), blue) << e.hint
                  << "\n";
        std::cout << "  |"
                  << "\n";
    }
    if (e.ecode==""){
        std::cout << "  ╰-----------------------------------------\n";
    }
    else{
        std::cout << "  ╰- " << fg(style("Hint: ", bold), cyan)
                << "Use peregrine --explain=" << e.ecode << "\n";
    }
    std::cout<<std::endl;
}
