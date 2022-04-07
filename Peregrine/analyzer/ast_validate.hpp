#ifndef PEREGRINE_AST_VALIDATE_HPP
#define PEREGRINE_AST_VALIDATE_HPP
#include "errors/error.hpp"
#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "lexer/tokens.hpp"
#include <string>
#include <vector>
namespace astValidator{
using namespace ast;
class Validator: public AstVisitor {
        std::vector<PEError> m_errors;
        std::string m_filename;
        bool m_is_js=false;
        bool m_should_contain_main=false;
        bool m_has_main=false;
        bool is_class=false;
        void add_error(Token tok, std::string msg,std::string submsg="",std::string hint="",std::string ecode="");
        void validate_parameters(std::vector<parameter> param);
        void validate_parameters(std::vector<AstNodePtr> param);
        bool visit(const Program& node);
        bool visit(const BlockStatement& node);
        bool visit(const ClassDefinition& node);
        bool visit(const ImportStatement& node);
        bool visit(const FunctionDefinition& node);
        bool visit(const VariableStatement& node);
        bool visit(const ConstDeclaration& node);
        bool visit(const TypeDefinition& node);
        bool visit(const PassStatement& node);
        bool visit(const IfStatement& node);
        bool visit(const AssertStatement& node);
        bool visit(const StaticStatement& node);
        bool visit(const ExportStatement& node);
        bool visit(const InlineStatement& node);
        bool visit(const RaiseStatement& node);
        bool visit(const WhileStatement& node);
        bool visit(const ForStatement& node);
        bool visit(const MatchStatement& node);
        bool visit(const ScopeStatement& node);
        bool visit(const ReturnStatement& node);
        bool visit(const ContinueStatement& node);
        bool visit(const BreakStatement& node);
        bool visit(const DecoratorStatement& node);
        bool visit(const ListLiteral& node);
        bool visit(const DictLiteral& node);
        bool visit(const ListOrDictAccess& node);
        bool visit(const BinaryOperation& node);
        bool visit(const PrefixExpression& node);
        bool visit(const PostfixExpression& node);
        bool visit(const FunctionCall& node);
        bool visit(const DotExpression& node);
        bool visit(const ArrowExpression& node);
        bool visit(const IdentifierExpression& node);
        bool visit(const TypeExpression& node);
        bool visit(const ListTypeExpr& node);
        bool visit(const DictTypeExpr& node);
        bool visit(const FunctionTypeExpr& node);
        bool visit(const NoLiteral& node);
        bool visit(const IntegerLiteral& node);
        bool visit(const DecimalLiteral& node);
        bool visit(const StringLiteral& node);
        bool visit(const BoolLiteral& node);
        bool visit(const NoneLiteral& node);
        bool visit(const UnionLiteral& node);
        bool visit(const EnumLiteral& node);
        bool visit(const WithStatement& node);
        bool visit(const VirtualStatement& node);
        bool visit(const CastStatement& node);
        bool visit(const PointerTypeExpr& node);
        bool visit(const RefTypeExpr& node);
        bool visit(const DefaultArg& node);
        bool visit(const TernaryIf& node);
        bool visit(const TryExcept& node);
        bool visit(const ExpressionTuple& node);
        bool visit(const TypeTuple& node);
        bool visit(const ExternStatement& node);
        bool visit(const SumType& node);
        bool visit(const MultipleAssign& node);
        bool visit(const AugAssign& node);
        bool visit(const MethodDefinition& node);
        bool visit(const ExternFuncDef& node);
        bool visit(const ExternUnionLiteral& node);
        bool visit(const ExternStructLiteral& node);
        bool visit(const VarArgTypeExpr& node);
        bool visit(const EllipsesTypeExpr& node);
        bool visit(const VarKwargTypeExpr& node);
        bool visit(const CompileTimeExpression& node);
        bool visit(const PrivateDef& node);  
        bool visit(const InlineAsm& node);  
        bool visit(const TernaryFor& node);
    public:
        Validator(AstNodePtr ast,std::string filename,bool is_js=false,bool should_contain_main=false);
};
}
#endif