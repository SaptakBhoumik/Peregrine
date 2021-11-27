#include "codegen.hpp"

namespace cpp {

void Codegen::matchArg(std::vector<ast::AstNodePtr> matchItem, std::vector<ast::AstNodePtr> caseItem){
    ast::AstNodePtr item;
    bool hasMatched = false;

    for (size_t i=0;i<matchItem.size();++i){
        item=matchItem[i];

        if (i>=caseItem.size()){
            break;
        }      
        
        if (caseItem[i]->type() != ast::KAstNoLiteral){
            if (hasMatched){
                write("&&");
            } else {
                hasMatched = true;
            }
            
            write("(");
            generate(item, createEnv());
            write("==");
            generate(caseItem[i], createEnv());
            write(")");
        }
        
    }
}

}