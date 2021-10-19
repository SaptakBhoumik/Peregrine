#include "lexer.hpp"
#include <iostream>
int main() {
  std::vector<Token> test;
  test=lexer("code=0");
  for (INTEGER i=0;i < test.size();++i){
      std::cout <<"Statement = "<< test.at(i).statement << " Type = " << test.at(i).tk_type << " Keyword = " << test.at(i).keyword << std::endl;
  }
  return 0;
}
