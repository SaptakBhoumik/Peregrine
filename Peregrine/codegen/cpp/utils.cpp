#include "codegen.hpp"
#include <memory>
#include <assert.h>
#define local_mangle_start() bool curr_state=local;\
                             local=true; \
                             auto symbol_map=m_symbolMap;

#define local_mangle_end() local=curr_state;\
                           m_symbolMap=symbol_map;
                           
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
            var+=res+"("+contains;
            if(is_func_def){
                var+=",____Pexception_handlers)";
            }
            else{
                var+=",NULL)";
            }
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
                write(", ");
                for (size_t i = 0; i < args.size(); ++i) {
                    if (i)
                        write(", ");
                    args[i]->accept(*this);
                }
            }
            var+=res;
            if(is_func_def){
                var+=",____Pexception_handlers)";
            }
            else{
                var+=",NULL)";
            }
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

            if (member->type()==ast::KAstIdentifier){
                auto attribute=std::dynamic_pointer_cast<ast::IdentifierExpression>(member)->value();
                write("____mem____P____P____"+attribute);
                var+=res+"("+contains;
                if(is_func_def){
                    var+=",____Pexception_handlers)";
                }
                else{
                    var+=",NULL)";
                }
                res="";
            }
            else if(member->type()==ast::KAstFunctionCall){
                auto function = std::dynamic_pointer_cast<ast::FunctionCall>(member);
                auto attribute=std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
                write("____mem____P____P____"+attribute);
                var+=res+"("+contains;
                res="";
                auto args = function->arguments();
                if (args.size()) {
                    write(", ");
                    for (size_t i = 0; i < args.size(); ++i) {
                        if (i)
                            write(", ");
                        args[i]->accept(*this);
                    }
                }
                var+=res;
                if(is_func_def){
                    var+=",____Pexception_handlers)";
                }
                else{
                    var+=",NULL)";
                }
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

            if (member->type()==ast::KAstIdentifier){
                auto attribute=std::dynamic_pointer_cast<ast::IdentifierExpression>(member)->value();
                write("____mem____P____P____"+attribute);
                var+=res+"("+contains;
                if(is_func_def){
                    var+=",____Pexception_handlers)";
                }
                else{
                    var+=",NULL)";
                }
                res="";
            }
            else if(member->type()==ast::KAstFunctionCall){
                auto function = std::dynamic_pointer_cast<ast::FunctionCall>(member);
                auto attribute=std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
                write("____mem____P____P____"+attribute);
                var+=res+"("+contains;
                res="";
                auto args = function->arguments();
                if (args.size()) {
                    write(", ");
                    for (size_t i = 0; i < args.size(); ++i) {
                        if (i)
                            write(", ");
                        args[i]->accept(*this);
                    }
                }
                var+=res;
                if(is_func_def){
                    var+=",____Pexception_handlers)";
                }
                else{
                    var+=",NULL)";
                }
                res="";
            }
            break;
        }
        default:{}
    }
    return var;
}
void Codegen::write_name(std::shared_ptr<ast::FunctionDefinition> node,std::string name,std::string virtual_static_inline){
    auto return_type=TurpleTypes(node->returnType());
    
    assert(node->parameters().size()>0);
    if(return_type.size()==0){
        node->returnType()->accept(*this);
    }
    else{
        write("int");
    }
    write(" ____mem____P____P____"+name+"(");
    local_mangle_start();
    codegenFuncParams(node->parameters(),1);
    if(return_type.size()>0){
        write(",");
    }
    for(size_t i=0;i<return_type.size();i++){
        return_type[i]->accept(*this);
        write("*____P____RETURN____"+std::to_string(i)+"=NULL");
        if(i<return_type.size()-1){
            write(",");
        }
    }
    write(") noexcept {\n");
    write("auto& ");
    is_define=true;
    node->parameters()[0].p_name->accept(*this);
    is_define=false;
    write("=*this;\n");
    if(not is_func_def){
        is_func_def=true;
        node->body()->accept(*this);
        is_func_def=false;
    }
    else{
        node->body()->accept(*this);
    }
    local_mangle_end();
    write("\n}");
}
void Codegen::magic_method(ast::AstNodePtr& node,std::string name){
    switch(node->type()){
        case ast::KAstFunctionDef:{
            std::shared_ptr<ast::FunctionDefinition> function =std::dynamic_pointer_cast<ast::FunctionDefinition>(node);
            assert(function->parameters().size()>0);
            auto func_name =std::dynamic_pointer_cast<ast::IdentifierExpression>(function->name())->value();
            if(func_name=="__init__"){
                write(name+"(");
                local_mangle_start();
                codegenFuncParams(function->parameters(),1);
                write(") noexcept {\n");
                write("auto& ");
                is_define=true;
                function->parameters()[0].p_name->accept(*this);
                is_define=false;
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
                local_mangle_end();
            }
            else if (func_name=="__del__"){
                write("~"+name+"(");
                local_mangle_start();
                write(") noexcept {\n");
                write("static ____P____exception_handler* ____Pexception_handlers=NULL;\n");
                write("auto& ");
                is_define=true;
                function->parameters()[0].p_name->accept(*this);
                is_define=false;
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
                local_mangle_end();
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
                local_mangle_start();
                codegenFuncParams(function->parameters(),1);
                write(") noexcept {\n");
                write("auto& ");
                is_define=true;
                function->parameters()[0].p_name->accept(*this);
                is_define=false;
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
                local_mangle_end();
            }
            else if (func_name=="__del__"){
                write("~"+name+"(");
                local_mangle_start();
                write(") noexcept {\n");
                write("static ____P____exception_handler* ____Pexception_handlers=NULL;\n");
                write("auto& ");
                is_define=true;
                function->parameters()[0].p_name->accept(*this);
                is_define=false;
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
                local_mangle_end();
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
                local_mangle_start();
                codegenFuncParams(function->parameters(),1);
                write(") noexcept {\n");
                write("auto& ");
                is_define=true;
                function->parameters()[0].p_name->accept(*this);
                is_define=false;
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
                local_mangle_end();
            }
            else if (func_name=="__del__"){
                write("~"+name+"(");
                local_mangle_start();
                write(") noexcept {\n");
                write("static ____P____exception_handler* ____Pexception_handlers=NULL;\n");
                write("auto& ");
                is_define=true;
                function->parameters()[0].p_name->accept(*this);
                is_define=false;
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
                local_mangle_end();
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
std::vector<ast::AstNodePtr> Codegen::TurpleTypes(ast::AstNodePtr node){
    std::vector<ast::AstNodePtr> turple_types;
    if(node->type()==ast::KAstTypeTuple){
        std::shared_ptr<ast::TypeTuple> turple =std::dynamic_pointer_cast<ast::TypeTuple>(node);
        if(turple->multiple_return()){
            turple_types=turple->items();
        }
    }
    
    return turple_types;
}
std::vector<ast::AstNodePtr> Codegen::TurpleExpression(ast::AstNodePtr node){
    std::vector<ast::AstNodePtr> turple_exp;
    if(node->type()==ast::KAstExpressionTuple){
        std::shared_ptr<ast::ExpressionTuple> turple =std::dynamic_pointer_cast<ast::ExpressionTuple>(node);
        if(turple->multiple_return()){
            turple_exp=turple->items();
        }
    }
    
    return turple_exp;
}
} // namespace cpp
