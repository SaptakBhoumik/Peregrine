#ifndef CPP_PEREGRINE_CODEGEN_HPP
#define CPP_PEREGRINE_CODEGEN_HPP

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "utils/symbolTable.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

namespace cpp {

typedef std::shared_ptr<SymbolTable<ast::AstNodePtr>> EnvPtr;

class Codegen : public ast::AstVisitor {
  public:
    Codegen(std::string outputFilename, ast::AstNodePtr ast,std::string filename);


  private:
    MangleName m_symbolMap;
    bool local=false;
    bool is_ref=false;
    bool is_define=false;
    bool is_dot_exp=false;
    std::string m_global_name;
    std::string curr_enum_name="";
    std::vector<std::string> enum_name={"error"};
    std::string res;
    bool save=false;
    std::string m_filename;
    std::ofstream m_file;
    bool is_func_def=false;
    std::string write(std::string_view code);

    std::string searchDefaultModule(std::string path, std::string moduleName);
    std::vector<ast::AstNodePtr> TurpleTypes(ast::AstNodePtr node);
    std::vector<ast::AstNodePtr> TurpleExpression(ast::AstNodePtr node);
    void codegenFuncParams(std::vector<ast::parameter> parameters,size_t start=0);
    void magic_method(ast::AstNodePtr& node,std::string name);
    void write_name(std::shared_ptr<ast::FunctionDefinition> node,std::string name,std::string virtual_static_inline="");
    void matchArg(std::vector<ast::AstNodePtr> matchItem,
                  std::vector<ast::AstNodePtr> caseItem);
    std::string wrap(ast::AstNodePtr item,std::string contains);
    bool visit(const ast::Program& node);
    bool visit(const ast::BlockStatement& node);
    bool visit(const ast::ImportStatement& node);
    bool visit(const ast::FunctionDefinition& node);
    bool visit(const ast::MethodDefinition& node);
    bool visit(const ast::VariableStatement& node);
    bool visit(const ast::ConstDeclaration& node);
    bool visit(const ast::TypeDefinition& node);
    bool visit(const ast::PassStatement& node);
    bool visit(const ast::IfStatement& node);
    bool visit(const ast::AssertStatement& node);
    bool visit(const ast::StaticStatement& node);
    bool visit(const ast::InlineStatement& node);
    bool visit(const ast::WhileStatement& node);
    bool visit(const ast::ForStatement& node);
    bool visit(const ast::MatchStatement& node);
    bool visit(const ast::ScopeStatement& node);
    bool visit(const ast::ReturnStatement& node);
    bool visit(const ast::ContinueStatement& node);
    bool visit(const ast::BreakStatement& node);
    bool visit(const ast::DecoratorStatement& node);
    bool visit(const ast::ListLiteral& node);
    bool visit(const ast::DictLiteral& node);
    bool visit(const ast::ListOrDictAccess& node);
    bool visit(const ast::BinaryOperation& node);
    bool visit(const ast::PrefixExpression& node);
    bool visit(const ast::PostfixExpression& node);
    bool visit(const ast::FunctionCall& node);
    bool visit(const ast::DotExpression& node);
    bool visit(const ast::ArrowExpression& node);
    bool visit(const ast::IdentifierExpression& node);
    bool visit(const ast::TypeExpression& node);
    bool visit(const ast::ListTypeExpr& node);
    bool visit(const ast::DictTypeExpr& node);
    bool visit(const ast::FunctionTypeExpr& node);
    bool visit(const ast::NoLiteral& node);
    bool visit(const ast::IntegerLiteral& node);
    bool visit(const ast::DecimalLiteral& node);
    bool visit(const ast::StringLiteral& node);
    bool visit(const ast::BoolLiteral& node);
    bool visit(const ast::NoneLiteral& node);
    bool visit(const ast::RaiseStatement& node);
    bool visit(const ast::UnionLiteral& node);
    bool visit(const ast::EnumLiteral& node);
    bool visit(const ast::CastStatement& node);
    bool visit(const ast::PointerTypeExpr& node);
    bool visit(const ast::RefTypeExpr& node);
    bool visit(const ast::ClassDefinition& node);
    bool visit(const ast::WithStatement& node);
    bool visit(const ast::DefaultArg& node);
    bool visit(const ast::ExportStatement& node);
    bool visit(const ast::TernaryIf& node);
    bool visit(const ast::TryExcept& node);
    bool visit(const ast::MultipleAssign& node);
    bool visit(const ast::AugAssign& node);
    bool visit(const ast::ExternStructLiteral& node);
    bool visit(const ast::ExternUnionLiteral& node);
    bool visit(const ast::ExternFuncDef& node);
    bool visit(const ast::PrivateDef& node);
    bool visit(const ast::InlineAsm& node);
    bool visit(const ast::LambdaDefinition& node);
    bool pipeline(const ast::BinaryOperation& node);
    EnvPtr m_env;
};

} // namespace cpp

#endif
