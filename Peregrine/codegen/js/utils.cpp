#include "codegen.hpp"

namespace js {

void Codegen::matchArg(std::vector<ast::AstNodePtr> matchItem,
                       std::vector<ast::AstNodePtr> caseItem) {
    ast::AstNodePtr item;
    bool hasMatched = false;

    for (size_t i = 0; i < matchItem.size(); ++i) {
        item = matchItem[i];

        if (i >= caseItem.size()) {
            break;
        }

        if (caseItem[i]->type() != ast::KAstNoLiteral) {
            if (hasMatched) {
                write("&&");
            } else {
                hasMatched = true;
            }

            write("(");
            item->accept(*this);
            write("===");
            caseItem[i]->accept(*this);
            write(")");
        }
    }
}
std::string Codegen::wrap(ast::AstNodePtr item,std::string contains){
    std::string var;
    switch(item->type()){
        case ast::KAstIdentifier:{
            item->accept(*this);
            var+=res+"("+contains+")";
            res="";
            break;
        }
        case ast::KAstFunctionCall:{
            auto function = std::dynamic_pointer_cast<ast::FunctionCall>(item);
            function->name()->accept(*this);
            var+=res+"("+contains;
            res="";
            auto args = function->arguments();
            if (args.size()) {
                for (size_t i = 0; i < args.size(); ++i) {
                    if (i)
                        write(", ");
                    args[i]->accept(*this);
                }
            }
            var+=res+")";
            res="";
            break;
        }
        case ast::KAstDotExpression:{
            auto exp = std::dynamic_pointer_cast<ast::DotExpression>(item);
            exp->owner()->accept(*this);
            write(".");
            var+=res;
            res="";
            ast::AstNodePtr member=exp->referenced();
//            while(member->type()==ast::KAstDotExpression){
//                auto curr = std::dynamic_pointer_cast<ast::DotExpression>(member);
//                curr->owner()->accept(*this);
//                write(".");
//                var+=res;
//                res="";
//                member=curr->referenced();
//            }
            if (member->type()==ast::KAstIdentifier){
                member->accept(*this);
                var+=res+"("+contains+")";
                res="";
            }
            else if(member->type()==ast::KAstFunctionCall){
                auto function = std::dynamic_pointer_cast<ast::FunctionCall>(member);
                function->name()->accept(*this);
                var+=res+"("+contains;
                res="";
                auto args = function->arguments();
                if (args.size()) {
                    for (size_t i = 0; i < args.size(); ++i) {
                        if (i)
                            write(", ");
                        args[i]->accept(*this);
                    }
                }
                var+=res+")";
                res="";
            }
            break;
        }
        default:{}
    }
    return var;
}
} // namespace js