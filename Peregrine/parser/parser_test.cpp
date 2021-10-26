#include "parser.hpp"
#include "../lexer/lexer.hpp"
int main(){
 auto  test=lexer("a=9","file name");
 Parser parse_obj;
 parse_obj.tokens=test;
 parse_obj.parse();
    return 0;
}