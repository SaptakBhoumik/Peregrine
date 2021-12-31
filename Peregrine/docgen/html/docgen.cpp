#include "docgen.hpp"
std::string module_name(std::string path){
    std::vector<std::string> split_path;
    std::string character;
    std::string temp;
    uint64_t index = 0;
    while (index < path.length()) {
        character = path.at(index);
        if (character == "/" || character == "\\") {
            split_path.push_back(temp);
            temp = "";
        } else {
            temp += character;
        }
        index++;
    }
    if (temp != "") {
        split_path.push_back(temp);
    }
    return split_path.back();
}
namespace html {
using namespace ast;
void Docgen::funcParams(std::vector<ast::parameter> parameters) {
    if (parameters.size()) {
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i)
                res+=",";
            res+=parameters[i].p_name->stringify();
            if (parameters[i].p_type->type()!=KAstNoLiteral){
              res+=":<font color=#45a4a0>"+parameters[i].p_type->stringify()+"</font>";
            }
            if (parameters[i].p_default->type()!=KAstNoLiteral){
                res+="=";
                res+=parameters[i].p_default->stringify();
            }
        }
    }
}

Docgen::Docgen(std::string outputFilename, ast::AstNodePtr ast,std::string file){
    m_file.open(outputFilename);
    std::string style="body {background: black;color:white } .local{color:#2d3748} a{text-decoration: none;}";
    style+="h1 {font-weight: bold;font-size: 40px;} h2 {font-weight: bold;font-size: 35px;} h3 {font-size: 25px;} h4 {font-size: 20px;}";
    style+=".code {background:#2d3748;padding: 10px}";
    style+="hr {border: none; height: 1px;background-color: #333;}";
    m_file<<"<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>DOCS</title> <style type=\"text/css\">"+style+"</style></head><body>\n";
    res+="<h1 id=\"content\">"+module_name(file);
    ast->accept(*this);
    write(res);
    m_file<<"</body></html>";
    m_file.close();
}
void Docgen::write(std::string_view code) {
    m_file << code;
}
bool Docgen::visit(const ast::Program& node) {
    if(node.comment()!=""){
      res+=" - "+node.comment();
    }
    res+="<a href=\"#content\" class=\"local\"> #</a></h1><hr>";
    for (auto& stmt : node.statements()) {
      switch (stmt->type()) {
        case KAstStatic:
        case KAstInline:
        case KAstFunctionDef:
        case KAstUnion:
        case KAstEnum:
        case KAstDecorator:
        case KAstClassDef:{
          stmt->accept(*this);
          break;
        }
        default:{}
      }
    }
    return true;
}
bool Docgen::visit(const ClassDefinition& node) {
    is_class=true;
    class_name=node.name()->stringify();
    res+="<h2 id=\""+class_name+"\">"+"class "+node.name()->stringify();
    res+="<a href=\"#"+class_name+"\" class=\"local\"> #</a></h2><hr>";
    res+="<h3><div class=\"code\"><font color=#63b3ed>class</font> ";
    res+="<font color=#45a4a0>"+class_name+"</font>(";
    auto parents=node.parent();
    for (size_t i=0;i<parents.size();++i){
        res+="<font color=#45a4a0>"+parents[i]->stringify()+"</font>";
        if(i<parents.size()-1){res+=",";}
    }
    res+=")";
    res+="</div></h3>";
    if(node.comment()!=""){
      res+=node.comment();
    }
    else{
      res+="None";
    }
    for (auto& x:node.methods()){
      x->accept(*this);
    }
    if(node.methods().size()==0){
      res+="<hr>";      
    }
    is_class=false;
    class_name="";
    return true;
}
bool Docgen::visit(const DecoratorStatement& node) {

    return true;
}
bool Docgen::visit(const FunctionDefinition& node) {
    std::string larger;
    std::string smaller;
    if(is_class){
      larger="h3";
      smaller="h4";
    }
    else{
      larger="h2";
      smaller="h3";
    }
    res+="<"+larger +" id=\""+node.name()->stringify()+"_"+class_name+"\">"+"def "+node.name()->stringify();
    res+="<a href=\"#"+node.name()->stringify()+"_"+class_name+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+node.name()->stringify()+"</font>(";
    funcParams(node.parameters());
    res+=")";
    res+="</div>"+std::string("</")+smaller+">";
    if(node.comment()!=""){
      res+=node.comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const StaticStatement& node) {

    return true;
}
bool Docgen::visit(const InlineStatement& node) {

    return true;
}
bool Docgen::visit(const VirtualStatement& node) {

    return true;
}
bool Docgen::visit(const EnumLiteral& node) {

    return true;
}
};
