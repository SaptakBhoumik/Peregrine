#include "codegen.hpp"
#include <assert.h>

namespace cpp {

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
            write("==");
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
            while(member->type()==ast::KAstDotExpression || member->type()==ast::KAstArrowExpression){
                if(member->type()==ast::KAstArrowExpression){
                  auto curr = std::dynamic_pointer_cast<ast::ArrowExpression>(member);
                  curr->owner()->accept(*this);
                  write("->");
                  var+=res;
                  res="";
                  member=curr->referenced();
                }
                else{
                  auto curr = std::dynamic_pointer_cast<ast::DotExpression>(member);
                  curr->owner()->accept(*this);
                  write(".");
                  var+=res;
                  res="";
                  member=curr->referenced();
                }
            }
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
        case ast::KAstArrowExpression:{
            auto exp = std::dynamic_pointer_cast<ast::ArrowExpression>(item);
            exp->owner()->accept(*this);
            write("->");
            var+=res;
            res="";
            ast::AstNodePtr member=exp->referenced();
            while(member->type()==ast::KAstDotExpression || member->type()==ast::KAstArrowExpression){
                if(member->type()==ast::KAstArrowExpression){
                  auto curr = std::dynamic_pointer_cast<ast::ArrowExpression>(member);
                  curr->owner()->accept(*this);
                  write("->");
                  var+=res;
                  res="";
                  member=curr->referenced();
                }
                else{
                  auto curr = std::dynamic_pointer_cast<ast::DotExpression>(member);
                  curr->owner()->accept(*this);
                  write(".");
                  var+=res;
                  res="";
                  member=curr->referenced();
                }
            }
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
//Based on:- https://docs.python.org/3/reference/datamodel.html
void Codegen::write_name(std::shared_ptr<ast::FunctionDefinition> node,std::string name,std::string virtual_static_inline){
    std::map<std::string,std::string> overloaded_binary_op={
                                                    {"__add__","+"},
                                                    {"__sub__","-"},
                                                    {"__mul__","*"},
                                                    {"__truediv__","/"},
                                                    {"__mod__","%"},
                                                    {"__rshift__",">>"},
                                                    {"__lshift__","<<"},
                                                    {"__and__"," and"},
                                                    {"__or__"," or"},
                                                    {"__bit_and__","&"},
                                                    {"__bit_or__","|"},
                                                    {"__xor__","^"},
                                                    {"__lt__","<"},
                                                    {"__gt__",">"},
                                                    {"__le__","<="},
                                                    {"__ge__",">="},
                                                    {"__eq__","=="},
                                                    {"__ne__","!="},
                                                    {"__isub__","-="},
                                                    {"__iadd__","+="},
                                                    {"__imul__","*="},
                                                    {"__idiv__","/="},
                                                    {"__imod__","%="},
                                                    {"__irshift__",">>="},
                                                    {"__ilshift__","<<="},
                                                    {"__iand__","&="},
                                                    {"__ior__","|="},
                                                    {"__ixor__","^="}
                                                    };
    std::map<std::string,std::string> overloaded_unary_op={
                                                    {"__neg__","-"},
                                                    {"__invert__","+"},
                                                    {"__not__"," not"}
                                                    };
    if (overloaded_binary_op.count(name)>0 && node->parameters().size()==2 && virtual_static_inline!="static" && virtual_static_inline!="static inline"){
        //TODO: Dont declare it twice
        node->returnType()->accept(*this);
        write(" "+name+"(");
        codegenFuncParams(node->parameters(),1);
        write("){\n");
        write("auto& ");
        node->parameters()[0].p_name->accept(*this);
        write("=*this;\n");
        if(not is_func_def){
            is_func_def=true;
            node->body()->accept(*this);
            is_func_def=false;
        }
        else{
            node->body()->accept(*this);
        }
        write("\n};\n");
        write(virtual_static_inline+" ");
        node->returnType()->accept(*this);
        write(" operator"+overloaded_binary_op[name]+"(");
        codegenFuncParams(node->parameters(),1);
        write("){\n");
        write("return "+name+"(");
        node->parameters()[1].p_name->accept(*this);
        write(");");
        write("\n}");
    }
    else if (overloaded_unary_op.count(name)>0 && node->parameters().size()==1 && virtual_static_inline!="static" && virtual_static_inline!="static inline"){
        //TODO: Dont declare it twice
        node->returnType()->accept(*this);
        write(" "+name+"(");
        write("){\n");
        write("auto& ");
        node->parameters()[0].p_name->accept(*this);
        write("=*this;\n");
        if(not is_func_def){
            is_func_def=true;
            node->body()->accept(*this);
            is_func_def=false;
        }
        else{
            node->body()->accept(*this);
        }
        write("\n};\n");
        write(virtual_static_inline+" ");
        node->returnType()->accept(*this);
        write(" operator"+overloaded_unary_op[name]+"(){\n");
        write("return "+name+"();");
        write("\n}");
    }
    else{
        assert(node->parameters().size()>0);
        node->returnType()->accept(*this);
        write(" "+name+"(");
        codegenFuncParams(node->parameters(),1);
        write("){\n");
        write("auto& ");
        node->parameters()[0].p_name->accept(*this);
        write("=*this;\n");
        if(not is_func_def){
            is_func_def=true;
            node->body()->accept(*this);
            is_func_def=false;
        }
        else{
            node->body()->accept(*this);
        }
        write("\n}");
    }
}
void Codegen::magic_methord(ast::AstNodePtr& node,std::string name){
    switch(node->type()){
        case ast::KAstFunctionDef:{
            std::shared_ptr<ast::FunctionDefinition> function =std::dynamic_pointer_cast<ast::FunctionDefinition>(node);
            assert(function->parameters().size()>0);
            auto func_name =std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
            if(func_name=="__init__"){
                write(name+"(");
                codegenFuncParams(function->parameters(),1);
                write("){\n");
                write("auto& ");
                function->parameters()[0].p_name->accept(*this);
                write("=*this;\n");
                if(not is_func_def){
                    is_func_def=true;
                    function->body()->accept(*this);
                    is_func_def=false;
                }
                else{
                    function->body()->accept(*this);
                }
                write("\n}");
            }
            else if (func_name=="__del__"){
                write("~"+name+"(");
                codegenFuncParams(function->parameters(),1);
                write("){\n");
                write("auto& ");
                function->parameters()[0].p_name->accept(*this);
                write("=*this;\n");
                if(not is_func_def){
                    is_func_def=true;
                    function->body()->accept(*this);
                    is_func_def=false;
                }
                else{
                    function->body()->accept(*this);
                }
                write("\n}");
            }
            else{
                write_name(function,func_name);
            }
            break;
        }
        case ast::KAstVirtual:{
            write("virtual ");
            std::shared_ptr<ast::VirtualStatement> virtual_function =std::dynamic_pointer_cast<ast::VirtualStatement>(node);
            std::shared_ptr<ast::FunctionDefinition> function =std::dynamic_pointer_cast<ast::FunctionDefinition>(virtual_function->body());
            auto func_name =std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
            assert(function->parameters().size()>0);
            if(func_name=="__init__"){
                write(name+"(");
                codegenFuncParams(function->parameters(),1);
                write("){\n");
                write("auto& ");
                function->parameters()[0].p_name->accept(*this);
                write("=*this;\n");
                if(not is_func_def){
                    is_func_def=true;
                    function->body()->accept(*this);
                    is_func_def=false;
                }
                else{
                    function->body()->accept(*this);
                }
                write("\n}");
            }
            else if (func_name=="__del__"){
                write("~"+name+"(");
                codegenFuncParams(function->parameters(),1);
                write("){\n");
                write("auto& ");
                function->parameters()[0].p_name->accept(*this);
                write("=*this;\n");
                if(not is_func_def){
                    is_func_def=true;
                    function->body()->accept(*this);
                    is_func_def=false;
                }
                else{
                    function->body()->accept(*this);
                }
                write("\n}");
            }
            else{
                write_name(function,func_name,"virtual");
            }
            break;
        }
        case ast::KAstInline:{
            write("inline ");
            std::shared_ptr<ast::InlineStatement> inline_function =std::dynamic_pointer_cast<ast::InlineStatement>(node);
            std::shared_ptr<ast::FunctionDefinition> function =std::dynamic_pointer_cast<ast::FunctionDefinition>(inline_function->body());
            auto func_name =std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
            assert(function->parameters().size()>0);
            if(func_name=="__init__"){
                write(name+"(");
                codegenFuncParams(function->parameters(),1);
                write("){\n");
                write("auto& ");
                function->parameters()[0].p_name->accept(*this);
                write("=*this;\n");
                if(not is_func_def){
                    is_func_def=true;
                    function->body()->accept(*this);
                    is_func_def=false;
                }
                else{
                    function->body()->accept(*this);
                }
                write("\n}");
            }
            else if (func_name=="__del__"){
                write("~"+name+"(");
                codegenFuncParams(function->parameters(),1);
                write("){\n");
                write("auto& ");
                function->parameters()[0].p_name->accept(*this);
                write("=*this;\n");
                if(not is_func_def){
                    is_func_def=true;
                    function->body()->accept(*this);
                    is_func_def=false;
                }
                else{
                    function->body()->accept(*this);
                }
                write("\n}");
            }
            else{
                write_name(function,func_name,"inline");
            }
            break;
        }
        case ast::KAstStatic:{
            write("static ");
            std::shared_ptr<ast::StaticStatement> static_function =std::dynamic_pointer_cast<ast::StaticStatement>(node);
            if (static_function->body()->type()==ast::KAstFunctionDef){
                std::shared_ptr<ast::FunctionDefinition> function =std::dynamic_pointer_cast<ast::FunctionDefinition>(static_function->body());
                auto func_name =std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
                write_name(function,func_name,"static");
            }
            else if (static_function->body()->type()==ast::KAstInline){
                write("inline ");
                std::shared_ptr<ast::InlineStatement> inline_function =std::dynamic_pointer_cast<ast::InlineStatement>(static_function->body());
                std::shared_ptr<ast::FunctionDefinition> function =std::dynamic_pointer_cast<ast::FunctionDefinition>(inline_function->body());
                auto func_name =std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
                write_name(function,func_name,"static inline");
            }
            break;
        }
        default:{}
    }
}

} // namespace cpp
