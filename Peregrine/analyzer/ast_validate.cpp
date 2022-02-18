#include "errors/error.hpp"
#include "lexer/tokens.hpp"
#include "ast_validate.hpp"
#include <map>
#include <iostream>
//TODO: Dont visit unnecessary nodes
namespace astValidator{
std::map<AstKind, std::string> keyword={
                                            {KAstPassStatement,"'pass'"},
                                            {KAstIfStmt,"'if'"},
                                            {KAstReturnStatement,"'return'"},
                                            {KAstWhileStmt,"'while'"},
                                            {KAstForStatement,"'for'"},
                                            {KAstMatchStmt,"'match'"},
                                            {KAstScopeStmt ,"'scope'"},
                                            {KAstWith,"'with'"},
                                            {KAstAssertStmt,"'assert'"},
                                            {KAstBreakStatement,"'break'"},
                                            {KAstContinueStatement,"'continue'"},
                                            {KAstRaiseStmt,"'raise'"},
                                            {KAstTryExcept,"'try"}
                                            };
Validator::Validator(AstNodePtr ast,std::string filename){
    m_filename = filename;
    ast->accept(*this);
    if(m_errors.size()>0){
        for(auto& e:m_errors){
            display(e);
        }
        exit(1);
    }
}
bool Validator::visit(const Program& node){
    for (auto& stmt : node.statements()) {
        switch(stmt->type()){
            case KAstTryExcept:
            case KAstRaiseStmt:
            case KAstScopeStmt:
            case KAstWith:
            case KAstAssertStmt:
            case KAstContinueStatement:
            case KAstMatchStmt:
            case KAstForStatement:
            case KAstWhileStmt:
            case KAstReturnStatement:
            case KAstIfStmt:
            case KAstBreakStatement:
            case KAstPassStatement:{
                add_error(stmt->token(), "SyntaxError: "+keyword[stmt->type()]+" statement outside function");
                break;
            }
            case KAstExpressionTuple:
            case KAstDotExpression:
            case KAstArrowExpression:
            case KAstListOrDictAccess:
            case KAstPrefixExpr:
            case KAstCast:
            case KAstTernaryIf:
            case KAstBinaryOp:
            case KAstDict:
            case KAstList:
            case KAstIdentifier:
            case KAstNone:
            case KAstBool:
            case KAstDecimal:
            case KAstInteger:{
                add_error(stmt->token(), "SyntaxError: Expression result unused","Assign the value to a variable ");
                break;
            }
            case KAstFunctionCall:{
                add_error(stmt->token(), "SyntaxError: Function call outside function", "Either assign the value to a variable or call it inside a function");
                break;
            }
            case KAstAugAssign:
            case KAstMultipleAssign:{
                // std::cout<<"AugAssign"<<std::endl;
                add_error(stmt->token(), "SyntaxError: Reassignment outside function", "Use it inside a function");
                break;
            }
            default:{
                stmt->accept(*this);
            }
        }
    }
    return true;
}
bool Validator::visit(const BlockStatement& node){
    for (auto& stmt : node.statements()) {
        switch(stmt->type()){
            case KAstExpressionTuple:
            case KAstPrefixExpr:
            case KAstTernaryIf:
            case KAstBinaryOp:
            case KAstDict:
            case KAstList:
            case KAstIdentifier:
            case KAstNone:
            case KAstBool:
            case KAstDecimal:
            case KAstInteger:{
                add_error(stmt->token(), "SyntaxError: Expression result unused","Assign the value to a variable ");
                break;
            }
            case KAstImportStmt:{
                add_error(stmt->token(), "SyntaxError: Import statement cant be inside function");
                break;
            }
            case KAstStatic:{
                add_error(stmt->token(), "SyntaxError: Nested static function are not allowed");
                break;
            }
            case KAstInline:{
                add_error(stmt->token(), "SyntaxError: Nested inline function are not allowed");
                break;
            }
            case KAstExport:{
                add_error(stmt->token(), "SyntaxError: Can't export nested function");
                break;
            }
            case KAstExternStatement:{
                add_error(stmt->token(), "SyntaxError: Extern statement cant be inside function");
                break;
            }

            default:{
                stmt->accept(*this);
            }
        }
    }
    return true;
}
bool Validator::visit(const ClassDefinition& node){
    node.name()->accept(*this);
    auto parents=node.parent();
    for (size_t i=0;i<parents.size();++i){
        parents[i]->accept(*this);
    }
    for (auto& x : node.other()){
        x->accept(*this);
    }
    for (auto& x : node.attributes()){
        x->accept(*this);
    }
    for (auto& x : node.methods()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const ImportStatement& node){return true;}//nothing to check
bool Validator::visit(const FunctionDefinition& node){
    node.returnType()->accept(*this);
    node.name()->accept(*this);
    node.body()->accept(*this);
    validate_parameters(node.parameters());
    return true;
}
bool Validator::visit(const VariableStatement& node){
    node.name()->accept(*this);
    node.value()->accept(*this);
    node.varType()->accept(*this);
    return true;
}
bool Validator::visit(const ConstDeclaration& node){
    node.name()->accept(*this);
    node.value()->accept(*this);
    node.constType()->accept(*this);
    return true;
}
bool Validator::visit(const TypeDefinition& node){
    node.name()->accept(*this);
    node.baseType()->accept(*this);
    return true;
}
bool Validator::visit(const PassStatement& node){return true;}
bool Validator::visit(const IfStatement& node){
    node.condition()->accept(*this);
    node.ifBody()->accept(*this);
    node.elseBody()->accept(*this);
    auto elifs=node.elifs();
    for (auto& x:elifs){
        x.first->accept(*this);
        x.second->accept(*this);
    }
    return true;
}
bool Validator::visit(const AssertStatement& node){
    node.condition()->accept(*this);
    return true;
}
bool Validator::visit(const StaticStatement& node){
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const InlineStatement& node){
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const ExportStatement& node){
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const RaiseStatement& node){
    node.value()->accept(*this);
    return true;
}
bool Validator::visit(const WhileStatement& node){
    node.condition()->accept(*this);
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const ForStatement& node){
    node.sequence()->accept(*this);
    node.body()->accept(*this);
    auto var=node.variable();
    for(auto& x:var){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const MatchStatement& node){
    for (auto& x:node.matchItem()){
        x->accept(*this);
    }
    node.defaultBody()->accept(*this);
    auto cases=node.caseBody();
    for (auto& x:cases){
        for (auto& y:x.first){
            y->accept(*this);
        }
        x.second->accept(*this);
    }
    return true;
}
bool Validator::visit(const ScopeStatement& node){
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const ReturnStatement& node){
    node.returnValue()->accept(*this);
    return true;
}
bool Validator::visit(const ContinueStatement& node){return true;}
bool Validator::visit(const BreakStatement& node){return true;}
bool Validator::visit(const DecoratorStatement& node){
    node.body()->accept(*this);
    auto x=node.decoratorItem();
    for (auto& y:x){
        y->accept(*this);
    }
    return true;
}
bool Validator::visit(const ListLiteral& node){
    for (auto& x:node.elements()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const DictLiteral& node){
    for (auto& x:node.elements()){
        x.first->accept(*this);
        x.second->accept(*this);
    }
    return true;
}
bool Validator::visit(const ListOrDictAccess& node){
    node.container()->accept(*this);
    for (auto& y:node.keyOrIndex()){
        y->accept(*this);
    }
    return true;
}
bool Validator::visit(const BinaryOperation& node){
    node.left()->accept(*this);
    node.right()->accept(*this);
    return true;
}
bool Validator::visit(const PrefixExpression& node){
    node.right()->accept(*this);
    return true;
}
bool Validator::visit(const PostfixExpression& node){
    node.left()->accept(*this);
    return true;
}
bool Validator::visit(const FunctionCall& node){
    node.name()->accept(*this);
    for (auto& x:node.arguments()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const DotExpression& node){
    node.owner()->accept(*this);
    node.referenced()->accept(*this);
    return true;
}
bool Validator::visit(const ArrowExpression& node){
    node.owner()->accept(*this);
    node.referenced()->accept(*this);
    return true;
}
bool Validator::visit(const IdentifierExpression& node){return true;}
bool Validator::visit(const TypeExpression& node){
    for (auto& x:node.generic_types()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const ListTypeExpr& node){
    node.elemType()->accept(*this);
    node.size()->accept(*this);
    return true;
}
bool Validator::visit(const DictTypeExpr& node){
    //TODO: remove it
    return false;
}
bool Validator::visit(const FunctionTypeExpr& node){
    node.returnTypes()->accept(*this);
    for(auto&x:node.argTypes()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const NoLiteral& node){return true;}
bool Validator::visit(const StringLiteral& node){return true;}
bool Validator::visit(const IntegerLiteral& node){return true;}
bool Validator::visit(const DecimalLiteral& node){return true;}
bool Validator::visit(const BoolLiteral& node){return true;}
bool Validator::visit(const NoneLiteral& node){return true;}
bool Validator::visit(const UnionLiteral& node){
    for (auto& x:node.elements()){
        x.first->accept(*this);
        x.second->accept(*this);
    }
    node.name()->accept(*this);
    return true;
}
bool Validator::visit(const EnumLiteral& node){
    for (auto& x:node.fields()){
        x.first->accept(*this);
        x.second->accept(*this);
    }
    node.name()->accept(*this);
    return true;
}
bool Validator::visit(const WithStatement& node){
    node.body()->accept(*this);
    for(auto& x:node.values()){
        x->accept(*this);
    }
    for(auto& x:node.variables()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const VirtualStatement& node){
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const CastStatement& node){
    node.cast_type()->accept(*this);
    node.value()->accept(*this);
    return true;
}
bool Validator::visit(const PointerTypeExpr& node){
    node.baseType()->accept(*this);
    return true;
}
bool Validator::visit(const RefTypeExpr& node){
    node.baseType()->accept(*this);
    return true;
}
bool Validator::visit(const DefaultArg& node){
    node.value()->accept(*this);
    node.name()->accept(*this);
    return true;
}
bool Validator::visit(const TernaryIf& node){
    node.if_value()->accept(*this);
    node.if_condition()->accept(*this);
    node.else_value()->accept(*this);
    return true;
}
bool Validator::visit(const TryExcept& node){
    node.body()->accept(*this);
    for(auto& x:node.except_clauses()){
        for(auto& y:x.first.first){
            y->accept(*this);
        }
        x.first.second->accept(*this);
        x.second->accept(*this);
    }
    return true;
}
bool Validator::visit(const ExpressionTuple& node){
    for(auto& x:node.items()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const TypeTuple& node){
    for(auto& x:node.items()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const ExternStatement& node){return true;}
bool Validator::visit(const SumType& node){
    for(auto& x:node.sum_types()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const MultipleAssign& node){
    for(auto& x:node.values()){
        x->accept(*this);
    }
    for(auto& x:node.names()){
        x->accept(*this);
    }
    return true;
}
bool Validator::visit(const AugAssign& node){
    node.name()->accept(*this);
    node.value()->accept(*this);
    return true;
}
void Validator::validate_parameters(std::vector<parameter> items){
    for(auto& x:items){
        x.p_type->accept(*this);
        x.p_default->accept(*this);
        x.p_name->accept(*this);
    }
}
void Validator::add_error(Token tok, std::string msg,
                std::string submsg,std::string hint,
                std::string ecode){
        PEError err = {{tok.line, tok.start,tok.location, m_filename, tok.statement},
                   msg,
                   submsg,
                   hint,
                   ecode};
        m_errors.push_back(err);
}
}