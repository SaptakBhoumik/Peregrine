#include "docgen.hpp"
//TODO:method defination
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
    style+=".code {background:#2d3748;padding: 10px;font-size:20px}";
    style+="hr {border: none; height: 1px;background-color: #333;}";
    m_file<<"<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>DOCS</title> <style type=\"text/css\">"+style+"</style></head><body>";
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
        case KAstMethodDef:
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
    id++;
    auto str="id"+std::to_string(id);
    is_class=true;
    class_name=node.name()->stringify();
    res+="<h2 id=\""+str+"\">"+"class "+node.name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a></h2><hr>";
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
    std::string larger;
    std::string smaller;
    std::shared_ptr<FunctionDefinition> body;
    std::string prefix="";
    if (node.body()->type()==KAstStatic){
      prefix="static ";
      body=std::dynamic_pointer_cast<FunctionDefinition>(
        std::dynamic_pointer_cast<StaticStatement>(node.body())->body()
      );
    }
    else{
      body=std::dynamic_pointer_cast<FunctionDefinition>(node.body());
    }
    id++;
    auto str="id"+std::to_string(id);
    if(is_class){
      larger="h3";
      smaller="h4";
    }
    else{
      larger="h2";
      smaller="h3";
    }
    res+="<"+larger +" id=\""+str+"\">"+"def "+body->name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\">";
    for (auto&x : node.decoratorItem()){
      res+="<font color=#45a4a0>@"+x->stringify()+"</font>";
      res+="<br>";
    }
    res+="<font color=#cf222e>"+prefix+"</font> <font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+body->name()->stringify()+"</font>(";
    funcParams(body->parameters());
    res+=")";
    if(body->returnType()->stringify()!="void"){
      res+="-><font color=#45a4a0>"+body->returnType()->stringify()+"</font>";
    }
    res+="</div></"+smaller+">";
    if(body->comment()!=""){
      res+=body->comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const FunctionDefinition& node) {
    id++;
    auto str="id"+std::to_string(id);
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
    res+="<"+larger +" id=\""+str+"\">"+"def "+node.name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+node.name()->stringify()+"</font>(";
    funcParams(node.parameters());
    res+=")";
    if(node.returnType()->stringify()!="void"){
      res+="-><font color=#45a4a0>"+node.returnType()->stringify()+"</font>";
    }
    res+="</div></"+smaller+">";
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
    std::string larger;
    std::string smaller;
    std::shared_ptr<FunctionDefinition> body;
    std::string prefix="static";
    if (node.body()->type()==KAstInline){
      prefix="static inline";
      body=std::dynamic_pointer_cast<FunctionDefinition>(
        std::dynamic_pointer_cast<InlineStatement>(node.body())->body()
      );
    }
    else if(node.body()->type()==KAstFunctionDef){
      body=std::dynamic_pointer_cast<FunctionDefinition>(node.body());
    }
    else{
      return true;
    }
    id++;
    auto str="id"+std::to_string(id);
    if(is_class){
      larger="h3";
      smaller="h4";
    }
    else{
      larger="h2";
      smaller="h3";
    }
    res+="<"+larger +" id=\""+str+"\">"+"def "+body->name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\">";
    res+="<font color=#cf222e>"+prefix+"</font> <font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+body->name()->stringify()+"</font>(";
    funcParams(body->parameters());
    res+=")";
    if(body->returnType()->stringify()!="void"){
      res+="-><font color=#45a4a0>"+body->returnType()->stringify()+"</font>";
    }
    res+="</div></"+smaller+">";
    if(body->comment()!=""){
      res+=body->comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const InlineStatement& node) {
    std::string larger;
    std::string smaller;
    auto body=std::dynamic_pointer_cast<FunctionDefinition>(node.body());
    id++;
    auto str="id"+std::to_string(id);
    if(is_class){
      larger="h3";
      smaller="h4";
    }
    else{
      larger="h2";
      smaller="h3";
    }
    res+="<"+larger +" id=\""+str+"\">"+"def "+body->name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#cf222e>inline</font> <font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+body->name()->stringify()+"</font>(";
    funcParams(body->parameters());
    res+=")";
    if(body->returnType()->stringify()!="void"){
      res+="-><font color=#45a4a0>"+body->returnType()->stringify()+"</font>";
    }
    res+="</div></"+smaller+">";
    if(body->comment()!=""){
      res+=body->comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const VirtualStatement& node) {
    std::string larger;
    std::string smaller;
    auto body=std::dynamic_pointer_cast<FunctionDefinition>(node.body());
    id++;
    auto str="id"+std::to_string(id);
    if(is_class){
      larger="h3";
      smaller="h4";
    }
    else{
      larger="h2";
      smaller="h3";
    }
    res+="<"+larger +" id=\""+str+"\">"+"def "+body->name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#cf222e>virtual</font> <font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+body->name()->stringify()+"</font>(";
    funcParams(body->parameters());
    res+=")";
    if(body->returnType()->stringify()!="void"){
      res+="-><font color=#45a4a0>"+body->returnType()->stringify()+"</font>";
    }
    res+="</div></"+smaller+">";
    if(body->comment()!=""){
      res+=body->comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const EnumLiteral& node) {
    id++;
    auto str="id"+std::to_string(id);
    std::string larger="h2";
    std::string smaller="h3";
    res+="<"+larger +" id=\""+str+"\">"+"enum "+node.name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#63b3ed>enum</font> ";
    res+="<font color=#45a4a0>"+node.name()->stringify()+"</font><br>";
    for(auto&x:node.fields()){
      res+="<font color=#45a4a0> &emsp; "+x.first->stringify()+"</font>";
      if(x.second->type()!=KAstNoLiteral){
        res+="=<font color=#45a4a0>"+x.second->stringify()+"</font>";
      }
      res+="<br>";
    }
    res+="</div></"+smaller+">";
    if(node.comment()!=""){
      res+=node.comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const UnionLiteral& node) {
    id++;
    auto str="id"+std::to_string(id);
    std::string larger="h2";
    std::string smaller="h3";
    res+="<"+larger +" id=\""+str+"\">"+"union "+node.name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#63b3ed>union</font> ";
    res+="<font color=#45a4a0>"+node.name()->stringify()+"</font><br>";
    for(auto&x:node.elements()){
      res+="<font color=#45a4a0>&emsp;"+x.second->stringify()+"</font>:";
      res+="<font color=#45a4a0> "+x.first->stringify()+"</font>";
      res+="<br>";
    }
    res+="</div></"+smaller+">";
    if(node.comment()!=""){
      res+=node.comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
bool Docgen::visit(const MethodDefinition& node) {
    id++;
    auto str="id"+std::to_string(id);
    std::string larger;
    std::string smaller;
    larger="h2";
    smaller="h3";
    res+="<"+larger +" id=\""+str+"\">"+"def ("+node.reciever().p_name->stringify();
    if(node.reciever().p_type->type()!=KAstNoLiteral){
      res+=":";
      res+=node.reciever().p_type->stringify();
    }
    res+=")"+node.name()->stringify();
    res+="<a href=\"#"+str+"\" class=\"local\"> #</a>"+"</"+smaller+">"+"<hr>";
    res+="<"+smaller+"><div class=\"code\"><font color=#63b3ed>def</font> ";
    res+="<font color=#45a4a0>"+node.name()->stringify()+"</font>(";
    funcParams(node.parameters());
    res+=")";
    if(node.returnType()->stringify()!="void"){
      res+="-><font color=#45a4a0>"+node.returnType()->stringify()+"</font>";
    }
    res+="</div></"+smaller+">";
    if(node.comment()!=""){
      res+=node.comment();
    }
    else{
      res+="None";
    }
    res+="<hr>";
    return true;
}
};
