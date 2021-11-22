#include "codegen.hpp"
#include <cstdint>

std::string Codegen::match_arg(std::vector<AstNodePtr> match_item,std::vector<AstNodePtr> case_item){
    std::string res;
    AstNodePtr item;
    for (uint64_t i=0;i<match_item.size();++i){
        item=match_item[i];
        auto temp_match_arg = generate(item); 
        if (i>=case_item.size()){
            break;
        }      
        else{
            if (case_item[i]->type()!=KAstNoLiteral){
                if (res!=""){
                    res+="&&";
                }
                res+="("+temp_match_arg+"=="+generate(case_item[i])+")";
            }
        }
    }
    return res;
}