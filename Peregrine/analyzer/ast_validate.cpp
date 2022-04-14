#include "errors/error.hpp"
#include "lexer/tokens.hpp"
#include "ast_validate.hpp"
#include <map>
#include <filesystem>
#include <iostream>
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
Validator::Validator(AstNodePtr ast,std::string filename,bool is_js,bool should_contain_main){
    m_is_js=is_js;
    m_filename = filename;
    m_should_contain_main=should_contain_main;
    ast->accept(*this);
    if(m_errors.size()>0||(m_should_contain_main && !m_has_main)){
        for(auto& e:m_errors){
            display(e);
        }
        if(m_should_contain_main){
            if(!m_has_main){
                std::cout<<"\e[91m"<<"Error: "<<m_filename<<" does not contain a main function"<<"\e[0m"<<std::endl;
            }
        }
        exit(1);
    }
}
bool Validator::visit(const Program& node){
    //TODO: check for more cases
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
            case KAstDict:
            case KAstList:
            case KAstIdentifier:
            case KAstNone:
            case KAstBool:
            case KAstBinaryOp:
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
    //TODO: check for more cases
    auto statements = node.statements();
    for (size_t i = 0; i < statements.size(); i++) {
        auto stmt=statements[i];
        switch(stmt->type()){
            case KAstExpressionTuple:
            case KAstPrefixExpr:
            case KAstTernaryIf:
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
            case KAstBinaryOp:{
                if(stmt->token().tkType!=tk_pipeline){
                    add_error(stmt->token(), "SyntaxError: Expression result unused","Assign the value to a variable ");
                }
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
            case KAstExternFuncDef:{
                add_error(stmt->token(), "Syntax error: External function definition is not allowed inside a function");
                break;
            }
            case KAstExternStruct:{
                add_error(stmt->token(), "Syntax error: External struct definition is not allowed inside a function");
                break;
            }
            case KAstExternUnion:{
                add_error(stmt->token(), "Syntax error: External union definition is not allowed inside a function");
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
            case KAstPrivate:{
                add_error(stmt->token(), "SyntaxError: Any kind of definition within block statement are already private");
                break;
            }
            //It should be at the last
            case KAstContinueStatement:
            case KAstBreakStatement:
            case KAstPassStatement:
            case KAstReturnStatement:{
                //it is not the last statement
                if(i<(statements.size()-1)){
                    add_error(statements[i+1]->token(), "SyntaxError: Anything after "+keyword[stmt->type()]+"statement is not executed","Remove it");
                    break;
                }
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
        switch(x->type()){
            case KAstExternStruct:{
                add_error(x->token(), "Syntax error: External struct definition is not allowed inside a function");
                break;
            }
            case KAstExternUnion:{
                add_error(x->token(), "Syntax error: External union definition is not allowed inside a function");
                break;
            }
            default:{
                x->accept(*this);
            }
        }
    }
    for (auto& x : node.attributes()){
        x->accept(*this);
    }
    for (auto& x : node.methods()){
        switch (x->type()) {
            case KAstMethodDef:{
                add_error(x->token(), "Syntax error: method definition is not allowed inside a class");
                break;
            }
            case KAstExternFuncDef:{
                add_error(x->token(), "Syntax error: External function definition is not allowed inside a class");
                break;
            }
            default:{
                is_class=true;
                x->accept(*this);
                is_class=false;
            }
        }
    }
    return true;
}
bool Validator::visit(const ImportStatement& node){
    //TODO:complete it
    auto module=node.moduleName();
    auto symbols=node.importedSymbols();
    switch (module->type()) {
        case KAstIdentifier:{
            auto name=std::dynamic_pointer_cast<IdentifierExpression>(module)->value();
            std::cout<<"Importing module "<<name<<std::endl;
            break;
        }
        case KAstDotExpression:{
            break;
        }
        default:{/*not possible*/}
    }
    return true;
}
bool Validator::visit(const FunctionDefinition& node){
    node.returnType()->accept(*this);
    auto name=std::dynamic_pointer_cast<IdentifierExpression>(node.name())->value();
    if(name=="main"){
        m_has_main=true;
    }
    node.name()->accept(*this);
    node.body()->accept(*this);
    if(is_class){
        if(node.parameters().size()==0){
            add_error(node.name()->token(),"Error: Methods defined in a class must have atleast one parameter to take in the instance of the object");
        }
        else{
            if(node.parameters()[0].p_type->type()!=KAstNoLiteral){
                add_error(node.parameters()[0].p_type->token(),"Error: The first parameter of methods defined in a class takes in the instance of the object so it is not necessary of specify the type");
            }
            if(node.parameters()[0].p_default->type()!=KAstNoLiteral){
                add_error(node.parameters()[0].p_default->token(),"Error: The first parameter of methods defined in a class takes in the instance of the object so it cant have default value");
            }
        }
        is_class=false;
    }
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
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Static statement is not allowed in javascript");
    }
    node.body()->accept(*this);
    return true;
}
bool Validator::visit(const InlineStatement& node){
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Inline statement is not allowed in javascript");
    }
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
    if(m_is_js){ 
        if(node.prefix().tkType==tk_ampersand||node.prefix().tkType==tk_multiply){
            add_error(node.prefix(),"SyntaxError: Pointers are not allowed in javascript");
        }
    }
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
    validate_parameters(node.argTypes());
    return true;
}
bool Validator::visit(const NoLiteral& node){return true;}
bool Validator::visit(const StringLiteral& node){return true;}
bool Validator::visit(const IntegerLiteral& node){return true;}
bool Validator::visit(const DecimalLiteral& node){return true;}
bool Validator::visit(const BoolLiteral& node){return true;}
bool Validator::visit(const NoneLiteral& node){return true;}
bool Validator::visit(const UnionLiteral& node){
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Union literal is not allowed in javascript");
    }
    else{
        for (auto& x:node.elements()){
            x.first->accept(*this);
            x.second->accept(*this);
        }
        node.name()->accept(*this);
    }
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
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Virtual statement is not allowed in javascript");
    }
    switch (node.body()->type()) {
        case KAstMethodDef:{
            add_error(node.body()->token(), "Syntax error: method definition is not allowed inside a class");
            break;
        }
        case KAstExternFuncDef:{
            add_error(node.body()->token(), "Syntax error: External function definition is not allowed inside a class");
            break;
        }
        default:{
            node.body()->accept(*this);
        }
    }
    return true;
}
bool Validator::visit(const CastStatement& node){
    node.cast_type()->accept(*this);
    node.value()->accept(*this);
    return true;
}
bool Validator::visit(const PointerTypeExpr& node){
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Pointer type is not allowed in javascript");
    }
    else{
        node.baseType()->accept(*this);
    }
    return true;
}
bool Validator::visit(const RefTypeExpr& node){
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Reference type is not allowed in javascript");
    }
    else{
        node.baseType()->accept(*this);
    }
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
void Validator::validate_parameters(std::vector<parameter> param){
    bool has_vargs = false;
    for (size_t i=0;i<param.size();i++){
        auto x=param[i];
        x.p_default->accept(*this);
        x.p_name->accept(*this);
        if(x.p_type->type()==KAstVarArgTypeExpr){
            if(i<param.size()-1){
                if(param[i+1].p_type->type()!=KAstVarKwargTypeExpr){
                    add_error(x.p_type->token(),"Either there should not be any parameter after VarArg or it has to be VarKwarg");
                }
            }
            //def x(*,**,*)
            else if(has_vargs){
                add_error(x.p_type->token(), "More than one '*' parameter is not allowed");
            }
            has_vargs = true;
        }
        else if(x.p_type->type()==KAstVarKwargTypeExpr){
            if(i<param.size()-1){
                add_error(x.p_type->token(),"'**' has to be the last parameter of the function");
            }
        }
        else if(x.p_type->type()==KAstEllipsesTypeExpr){
            if(i<param.size()-1){
                add_error(x.p_type->token(),"'...' has to be the last parameter of the function");
            }
        }
        x.p_type->accept(*this);
    }
}
void Validator::validate_parameters(std::vector<AstNodePtr> param){
    bool has_vargs = false;
    for (size_t i=0;i<param.size();i++){
        auto x=param[i];
        // stf::cout<<"validating parameter "<<x->to_string()<<"\n";
        if(x->type()==KAstVarArgTypeExpr){            
            if(i<param.size()-1){
                if(param[i+1]->type()!=KAstVarKwargTypeExpr){
                    add_error(param[i+1]->token(),"Either there should not be any parameter after VarArg or it has to be VarKwarg");
                }
            }
            //def x(*,**,*)
            else if(has_vargs){
                add_error(x->token(), "More than one '*' parameter is not allowed");
            }
            has_vargs = true;
        }
        else if(x->type()==KAstVarKwargTypeExpr){
            if(i<param.size()-1){
                add_error(param[i+1]->token(),"'**' has to be the last parameter of the function");
            }
        }
        else if(x->type()==KAstEllipsesTypeExpr){
            if(i<param.size()-1){
                add_error(param[i+1]->token(),"'...' has to be the last parameter of the function");
            }
        }
        x->accept(*this);
    }
}
bool Validator::visit(const MethodDefinition& node){
    node.returnType()->accept(*this);
    node.name()->accept(*this);
    node.body()->accept(*this);
    node.reciever().p_type->accept(*this);
    node.reciever().p_default->accept(*this);
    node.reciever().p_name->accept(*this);
    validate_parameters(node.parameters());
    return true;
}
bool Validator::visit(const ExternFuncDef& node){
    node.returnType()->accept(*this);
    node.name()->accept(*this);
    validate_parameters(node.parameters());
    return true;
}
bool Validator::visit(const ExternUnionLiteral& node){
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Union type is not allowed in javascript");
    }
    else{    
        node.name()->accept(*this);
        for(auto& x:node.elements()){
            x.first->accept(*this);
            x.second->accept(*this);
        }
    }
    return true;
}
bool Validator::visit(const ExternStructLiteral& node){
    node.name()->accept(*this);
    for(auto& x:node.elements()){
        x.first->accept(*this);
        x.second->accept(*this);
    }
    return true;
}
bool Validator::visit(const VarArgTypeExpr& node){
    return true;
}
bool Validator::visit(const EllipsesTypeExpr& node){
    return true;
}
bool Validator::visit(const VarKwargTypeExpr& node){
    return true;
}
bool Validator::visit(const CompileTimeExpression& node){
    auto exp=node.expression();
    auto token=exp->token();
    switch(exp->type()){
        case KAstList:
        case KAstExpressionTuple:
        case KAstDict:
        case KAstInteger:
        case KAstDecimal:
        case KAstString:
        case KAstBool:
        case KAstNone:{
            add_error(token, "SyntaxError: Compile time expression cannot be a literal");
            break;
        }
        case KAstImportStmt:{
            add_error(token, "SyntaxError: Imports are already done at compile time so they can't be used in compile time expressions");
            break;
        }
        case KAstConstDecl:
        case KAstAugAssign:
        case KAstMultipleAssign:
        case KAstVariableStmt:{
            //TODO: Maybe allow this
            add_error(token, "SyntaxError: Compile time defination or reassignment are not allowed");
            break;
        }
        case KAstFunctionDef:{
            //TODO: Maybe allow this
            add_error(token, "SyntaxError: Compile time functions are not allowed");
            break;
        }
        case KAstClassDef:{
            add_error(token, "SyntaxError: Compile time classes are not allowed");
            break;
        }
        case KAstReturnStatement:{
            add_error(token, "SyntaxError: Compile time return statements are not allowed","Just use return instead of $return");
            break;
        }
        case KAstScopeStmt:{
            add_error(token, "SyntaxError: Compile time scope statements are not allowed");
            break;
        }
        case KAstTypeDefinition:{
            add_error(token, "SyntaxError: Compile time type definitions are not allowed");
            break;
        }
        case KAstDecorator:{
            add_error(token, "SyntaxError: Compile time decorators are not allowed");
            break;
        }
        case KAstPassStatement:{
            add_error(token, "SyntaxError: Compile time pass statements are not allowed","You can just use pass instead of $pass");
            break;
        }
        
        case KAstUnion:{
            add_error(token, "SyntaxError: Compile time union types are not allowed");
            break;
        }
        case KAstStatic:
        case KAstInline:{
            add_error(token, "SyntaxError: Compile time static and inline functions are not allowed");
            break;
        }
        case KAstExternStatement:{
            add_error(token, "SyntaxError: Compile time extern statements are not allowed");
            break;
        }
        case KAstEnum:{
            add_error(token, "SyntaxError: Compile time enums are not allowed");
            break;
        }
        case KAstCast:{
            add_error(token, "SyntaxError: Can't cast at compile time");
            break;
        }
        case KAstExport:{
            add_error(token, "SyntaxError: Can't export compile time statements");
            break;
        }
        case KAstRaiseStmt:{
            add_error(token, "SyntaxError: Can't raise at compile time");
            break;
        }
        case KAstTryExcept:{
            add_error(token, "SyntaxError: Try except block not allowed at compile time");
            break;
        }
        case KAstExternStruct:
        case KAstExternUnion:
        case KAstExternFuncDef:{
            add_error(token, "SyntaxError: Can't define external function or struct or union at compile time");
            break;
        }
        case KAstCompileTimeExpression:{
            add_error(token, "SyntaxError: Unexpected token $");
            break;
        }
        case KAstPrivate:{
            add_error(token, "SyntaxError: Unexpected token private");
            break;
        }
        case KAstInlineAsm:{
            add_error(token, "SyntaxError: Inline assembly not allowed at compile time");
            break;
        }
        default:{
            exp->accept(*this);
        }
    }
    return true;
}
bool Validator::visit(const PrivateDef& node){
    node.definition()->accept(*this); 
    return true;
}
bool Validator::visit(const InlineAsm& node){
    if(m_is_js){
        add_error(node.token(), "SyntaxError: Inline assembly is not allowed in javascript");
    }
    else{
        node.output()->accept(*this);
        auto inputs=node.inputs();
        for(auto& x:inputs){
            x.second->accept(*this);
        }
    }
    return true;
} 
bool Validator::visit(const TernaryFor& node){
    node.for_value()->accept(*this);
    node.for_iterate()->accept(*this);
    auto variable=node.for_variable();
    for(auto& x:variable){
        x->accept(*this);
    }
    return true;
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
