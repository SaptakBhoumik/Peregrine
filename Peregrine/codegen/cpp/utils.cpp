#include "codegen.hpp"

namespace cpp {

std::string Codegen::matchArg(std::vector<ast::AstNodePtr> matchItem,std::vector<ast::AstNodePtr> caseItem){
    std::string res;
    ast::AstNodePtr item;
    for (size_t i=0;i<matchItem.size();++i){
        item=matchItem[i];
        auto temp_match_arg = generate(item, createEnv()); // not like this
        if (i>=caseItem.size()){
            break;
        }      
        else{
            if (caseItem[i]->type() != ast::KAstNoLiteral){
                if (res!=""){
                    res+="&&";
                }
                res+="("+temp_match_arg+"=="+generate(caseItem[i], createEnv())+")";
            }
        }
    }
    return res;
}

}