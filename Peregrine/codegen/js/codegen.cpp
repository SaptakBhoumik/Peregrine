#include "codegen.hpp"

#include "ast/ast.hpp"
#include "errors/error.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

namespace js {

Codegen::Codegen(std::string outputFilename, ast::AstNodePtr ast, bool html, std::string  filename) {
    m_filename = filename;
    m_file.open(outputFilename);
    if (html){
        m_file<<"<!DOCTYPE html><html><body id='body'><script>";
    }
    m_file << "function render(code){document.write(code);}error___AssertionError=0;error___ZeroDivisionError=1\n";
    m_env = createEnv();
    ast->accept(*this);
    m_file<<"\nmain();";
    if(html){
        m_file<<"</script></body></html>";
    }
    m_file.close();
}

std::shared_ptr<SymbolTable<ast::AstNodePtr>>
Codegen::createEnv(std::shared_ptr<SymbolTable<ast::AstNodePtr>> parent) {
    return std::make_shared<SymbolTable<ast::AstNodePtr>>(parent);
}

// TODO: buffer it
std::string Codegen::write(std::string_view code) {
    if(save){
        res+=code;
    }
    else{
        m_file << code; 
    }
    return res;
}
std::string Codegen::searchDefaultModule(std::string path,
                                         std::string moduleName) {
    for (auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::path(entry.path()).filename() == moduleName) {
            // TODO :ignore extensions?
            if (entry.is_regular_file()) {
                return entry.path();
            } else if (entry.is_directory()) {
                // TODO: avoid deeply nested folders
                searchDefaultModule(entry.path(), moduleName);
            }
        }
    }

    return "";
}

void Codegen::codegenFuncParams(std::vector<ast::parameter> parameters) {
    if (parameters.size()) {
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i)
                write(", ");
            parameters[i].p_name->accept(*this);
        }
    }
}

bool Codegen::visit(const ast::Program& node) {
    for (auto& stmt : node.statements()) {
        stmt->accept(*this);
        write(";\n"); // TODO: will this break stuff later?
                     // no 
    }
    return true;
}

bool Codegen::visit(const ast::BlockStatement& node) {
    for (auto& stmt : node.statements()) {
        stmt->accept(*this);
        write(";\n");
    }
    return true;
}

bool Codegen::visit(const ast::ImportStatement& node) { return true; }

bool Codegen::visit(const ast::FunctionDefinition& node) {
    auto functionName =
        std::dynamic_pointer_cast<ast::IdentifierExpression>(node.name())
            ->value();
    if (not is_func_def){
        is_func_def = true;
        if (functionName == "main") {
            // we want the main function to always return 0 if success
            write("function main (");
            codegenFuncParams(node.parameters());
            write(") {\n");
            node.body()->accept(*this);
            write("return 0;\n}");
        } else {
            write("function ");
            node.name()->accept(*this);
            write("(");
            codegenFuncParams(node.parameters());
            write(") {\n");
            node.body()->accept(*this);
            write("\n}");
        }
        is_func_def=false;
    }
    else{
        node.name()->accept(*this);
        write("=function(");
        codegenFuncParams(node.parameters());
        write(")");
        write("{\n");
        node.body()->accept(*this);
        write("\n}");
    }
    return true;
}

bool Codegen::visit(const ast::VariableStatement& node) {
    if (node.varType()->type() != ast::KAstNoLiteral) {
        write("let ");
    }
    node.name()->accept(*this);
    if (node.value()->type() != ast::KAstNoLiteral) {
        write(" = ");
        node.value()->accept(*this);
    }
    return true;
}

bool Codegen::visit(const ast::ConstDeclaration& node) { 
    write("const "); 
    write(" ");
    node.name()->accept(*this);
    write("=");
    node.value()->accept(*this);
    return true; 
    }

bool Codegen::visit(const ast::TypeDefinition& node) {
    //no typedef in js
    write("\n//");
    // we are making it a comment because ; is added
    // to each node at the end. we dont want that to
    // happen because it will result in ;; which is
    // an error
    return true;
}

bool Codegen::visit(const ast::PassStatement& node) {
    // we are making it a comment because ; is added
    // to each node at the end. we dont want that to
    // happen because it will result in ;; which is
    // an error
    write("\n//pass");
    return true;
}

bool Codegen::visit(const ast::IfStatement& node) {
    write("if (");
    node.condition()->accept(*this);
    write(") {\n");
    node.ifBody()->accept(*this);
    write("}");

    auto elifNode = node.elifs();
    if (elifNode.size() != 0) {
        write("\n");
        for (auto& body : elifNode) { // making sure that elif exists
            write("else if (");
            body.first->accept(*this);
            write(") {\n");
            body.second->accept(*this);
            write("}");
        }
    }

    auto elseNode = node.elseBody();
    if (elseNode->type() ==
        ast::KAstBlockStmt) { // making sure that else exists
        write("\nelse {\n");
        elseNode->accept(*this);
        write("}");
    }
    return true;
}

bool Codegen::visit(const ast::WhileStatement& node) {
    write("while (");
    node.condition()->accept(*this);
    write(") {\n");
    node.body()->accept(*this);
    write("}");
    return true;
}

bool Codegen::visit(const ast::ForStatement& node) { return true; }

bool Codegen::visit(const ast::MatchStatement& node) {
    auto toMatch = node.matchItem();
    auto cases = node.caseBody();
    auto defaultBody = node.defaultBody();
    write("\nwhile (true) {\n");
    for (size_t i = 0; i < cases.size(); ++i) {
        auto currCase = cases[i];
        if (currCase.first.size() == 1 &&
            currCase.first[0]->type() == ast::KAstNoLiteral) {
            if (i == 0) {
                currCase.second->accept(*this);
                write("\n");
            } else {
                write("else {\n");
                currCase.second->accept(*this);
                write("\n}\n");
            }
        } else if (i == 0) {
            write("if (");
            matchArg(toMatch, currCase.first);
            write(") {\n");
            currCase.second->accept(*this);
            write("\n}\n");
        } else {
            write("else if (");
            matchArg(toMatch, currCase.first);
            write(") {\n");
            currCase.second->accept(*this);
            write("\n}\n");
        }
    }

    if (defaultBody->type() != ast::KAstNoLiteral) {
        defaultBody->accept(*this);
    }
    write("\nbreak;\n}");
    return true;
}

bool Codegen::visit(const ast::ScopeStatement& node) {
    write("{\n");
    node.body()->accept(*this);
    write("\n}");
    return true;
}

bool Codegen::visit(const ast::CppStatement& node) {
    //not in js
    return true;
}

bool Codegen::visit(const ast::ReturnStatement& node) {
    write("return ");
    node.returnValue()->accept(*this);
    return true;
}

bool Codegen::visit(const ast::ContinueStatement& node) {
    write("continue");
    return true;
}

bool Codegen::visit(const ast::BreakStatement& node) {
    write("break");
    return true;
}

bool Codegen::visit(const ast::DecoratorStatement& node) {
    auto items = node.decoratorItem();
    auto body = node.body();
    std::string contains;
    std::string x;
    std::string prev;
    save=true;
    if (res!=""){
        prev=res;
        res="";
    }
    if(body->type()==ast::KAstFunctionDef){
        auto function = std::dynamic_pointer_cast<ast::FunctionDefinition>(body);
        write("let ");
        function->name()->accept(*this);
        write("=");
        x+=res;
        res="";
        write("function(");
        codegenFuncParams(function->parameters());
        write(")");
        write("{\n");
        if(not is_func_def){
            is_func_def=true;
            function->body()->accept(*this);
            is_func_def=false;
        }
        else{
            function->body()->accept(*this);
        }
        write("\n}");
        contains=res;
        res="";
    }
    for (size_t i = items.size() - 1; i != (size_t)-1; i--){
        ast::AstNodePtr item=items[i];
        contains=wrap(item,contains);
    }
    if (prev==""){
        save=false;
        write(x+contains);
    }
    else{
        write(prev+x+contains);
    }
    return true;
}


bool Codegen::visit(const ast::ListLiteral& node) { 
    write("[");
    auto elements=node.elements();
    if (elements.size()>0){
        for (size_t i=0;i<elements.size();++i){
            elements[i]->accept(*this);
            if (i<elements.size()-1){
                write(",");
            }
        }
    }
    write("]");
    return true;
}

bool Codegen::visit(const ast::DictLiteral& node) { 
    auto elements=node.elements();
    write("{");
    if (elements.size()>0){
        for (size_t i=0;i<elements.size();++i){
            elements[i].first->accept(*this);
            write(":");
            elements[i].second->accept(*this);
            if (i<elements.size()-1){
                write(",");
            }
        }
    }
    write("}");
    return true; 
}

bool Codegen::visit(const ast::ListOrDictAccess& node) { 
    node.container()->accept(*this);
    write("[");
    node.keyOrIndex()[0]->accept(*this);//TODO: Wont work in the future
    write("]");
    return true;
}

bool Codegen::visit(const ast::BinaryOperation& node) {
    if (node.op().keyword == "**") {
        write("_PEREGRINE_POWER(");
        node.left()->accept(*this);
        write(",");
        node.right()->accept(*this);
        write(")");
    } else if (node.op().keyword == "//") {
        write("_PEREGRINE_FLOOR(");
        node.left()->accept(*this);
        write("/");
        node.right()->accept(*this);
        write(")");
    } else {
        write("(");
        node.left()->accept(*this);
        if (node.op().keyword=="=="){
            write(" === ");
        }
        else{
            write(" " + node.op().keyword + " ");
        }
        node.right()->accept(*this);
        write(")");
    }
    return true;
}

bool Codegen::visit(const ast::PrefixExpression& node) {
    write("(" + node.prefix().keyword + " ");
    node.right()->accept(*this);
    write(")");
    return true;
}

bool Codegen::visit(const ast::FunctionCall& node) {
    auto functionName =
        std::dynamic_pointer_cast<ast::IdentifierExpression>(node.name())
            ->value();
    if (functionName=="print"||functionName=="printf"){
        write("console.log");
    }
    else{
        node.name()->accept(*this);
    }
    write("(");

    auto args = node.arguments();
    if (args.size()) {
        for (size_t i = 0; i < args.size(); ++i) {
            if (i)
                write(", ");
            args[i]->accept(*this);
        }
    }

    write(")");
    return true;
}

bool Codegen::visit(const ast::DotExpression& node) { 
    //FIXME: Not very elegent
    if (not is_dot_exp){
        is_dot_exp=true;
        if (node.owner()->type()==ast::KAstIdentifier){
            std::string name = std::dynamic_pointer_cast<ast::IdentifierExpression>(node.owner())->value();
            if(std::count(enum_name.begin(), enum_name.end(), name)){
                write(name+"___");
                node.referenced()->accept(*this); 
            }
            else{
                node.owner()->accept(*this);
                write(".");
                node.referenced()->accept(*this);   
            }
        is_dot_exp=false;
        }
        else {
            node.owner()->accept(*this);
            write(".");
            node.referenced()->accept(*this); 
        }
        is_dot_exp=false;
    }
    else{
        node.owner()->accept(*this);
        write(".");
        node.referenced()->accept(*this);
    }  
    return true; 
}

bool Codegen::visit(const ast::IdentifierExpression& node) {
    if (is_enum){
        write(enum_name.back()+"___");
    }
    write(node.value());
    return true;
}

bool Codegen::visit(const ast::TypeExpression& node) {
    //no types in js
    return true;
}

bool Codegen::visit(const ast::ListTypeExpr& node) { return true; }

bool Codegen::visit(const ast::DictTypeExpr& node) { return true; }

bool Codegen::visit(const ast::FunctionTypeExpr& node) {
    //no types in js
    return true;
}

bool Codegen::visit(const ast::NoLiteral& node) {
    // nothing ig
    return true;
}

bool Codegen::visit(const ast::IntegerLiteral& node) {
    write(node.value());
    return true;
}

bool Codegen::visit(const ast::DecimalLiteral& node) {
    write(node.value());
    return true;
}

bool Codegen::visit(const ast::StringLiteral& node) {
    write("\"" + node.value() + "\"");
    return true;
}

bool Codegen::visit(const ast::BoolLiteral& node) {
    write((node.value() == "True") ? "true" : "false");
    return true;
}

bool Codegen::visit(const ast::NoneLiteral& node) {
    write("null");
    return true;
}
bool Codegen::visit(const ast::AssertStatement& node){
    write("if(! ");
    node.condition()->accept(*this);
    write("){\n");
    write("console.log(\"AssertionError : in line "+std::to_string(node.token().line)+" in file "+m_filename+"\\n   "+node.token().statement+"\");throw error___AssertionError;");
    write("\n}");
    return true;
}
bool Codegen::visit(const ast::RaiseStatement& node){
    write("throw ");
    node.value()->accept(*this);
    return true;
}
bool Codegen::visit(const ast::EnumLiteral& node){
    auto fields=node.fields();
    std::string name=std::dynamic_pointer_cast<ast::IdentifierExpression>(node.name())->value();
    enum_name.push_back(name);
    ast::AstNodePtr prev_element;
    for (size_t i=0;i<fields.size();++i){
        auto field=fields[i];
        write(name+"___");
        field.first->accept(*this);
        write(" = ");
        if (field.second->type()!=ast::KAstNoLiteral){
            is_enum=true;
            field.second->accept(*this);
            is_enum=false;
        }
        else{
            if (i==0){
                write("0");
            }
            else{
                write(name+"___");
                prev_element->accept(*this);
                write("+1");
            }
        }
        prev_element=field.first;
        write(";\n");
    }
    return true;
}
} // namespace js
