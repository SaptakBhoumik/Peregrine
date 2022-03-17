#ifndef HTML_PEREGRINE_DOCGEN_HPP
#define HTML_PEREGRINE_DOCGEN_HPP
#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
namespace html {
class Docgen : public ast::AstVisitor {
  public:
    Docgen(std::string outputFilename, ast::AstNodePtr ast,std::string file);
  private:
    size_t id=0;
    std::ofstream m_file;
    std::string res;
    std::string class_name;
    bool is_class=false;
    void funcParams(std::vector<ast::parameter> parameters);
    bool visit(const ast::Program& node);
    bool visit(const ast::ClassDefinition& node);
    bool visit(const ast::DecoratorStatement& node);
    bool visit(const ast::FunctionDefinition& node);
    bool visit(const ast::StaticStatement& node);
    bool visit(const ast::InlineStatement& node);
    bool visit(const ast::VirtualStatement& node);
    bool visit(const ast::EnumLiteral& node);
    bool visit(const ast::UnionLiteral& node);
    bool visit(const ast::MethodDefinition& node);
    void write(std::string_view code);
};
};
#endif
