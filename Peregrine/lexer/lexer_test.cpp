#include "lexer.hpp"
#include <iostream>
int main() {
  LEXEME test;
  test=lexer("code=[\"some ' code\"]\n    a=","file name");
  for (INTEGER i=0;i < test.size();++i){
      std::cout <<"Statement = "<< test.at(i).statement << " Type = " << test.at(i).tk_type << " Keyword = " << test.at(i).keyword << " Start = " << test.at(i).start << " End = " << test.at(i).end << std::endl;
  }
  return 0;
}
