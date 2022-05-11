#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace ast {

Program::Program(std::vector<AstNodePtr> statements, std::string comment)
    : m_statements(statements), m_comment(comment) {}

std::vector<AstNodePtr> Program::statements() const { return m_statements; }

std::string Program::comment() const{ return m_comment; }


Token Program::token() const { return Token{}; }

AstKind Program::type() const { return KAstProgram; }

std::string Program::stringify() const {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt->stringify();
        res += "\n";
    }

    return res;
}

IntegerLiteral::IntegerLiteral(Token tok, std::string_view value) {
    m_token = tok;
    m_value = value;
}

std::string IntegerLiteral::value() const { return m_value; }

Token IntegerLiteral::token() const { return m_token; }

AstKind IntegerLiteral::type() const { return KAstInteger; }

std::string IntegerLiteral::stringify() const { return m_value; }

DecimalLiteral::DecimalLiteral(Token tok, std::string_view value) {
    m_token = tok;
    m_value = value;
}

std::string DecimalLiteral::value() const { return m_value; }

Token DecimalLiteral::token() const { return m_token; }

AstKind DecimalLiteral::type() const { return KAstDecimal; }

std::string DecimalLiteral::stringify() const { return m_value; }

StringLiteral::StringLiteral(Token tok, std::string_view value, bool formatted,
                             bool raw) {
    m_token = tok;
    m_value = value;
    m_formatted = formatted;
    m_raw = raw;
}

std::string StringLiteral::value() const { return m_value; }

bool StringLiteral::formatted() const { return m_formatted; }

bool StringLiteral::raw() const { return m_raw; }

Token StringLiteral::token() const { return m_token; }

AstKind StringLiteral::type() const { return KAstString; }

std::string StringLiteral::stringify() const { return m_value; }

BoolLiteral::BoolLiteral(Token tok, std::string_view value) {
    m_token = tok;
    m_value = value;
}

std::string BoolLiteral::value() const { return m_value; }

Token BoolLiteral::token() const { return m_token; }

AstKind BoolLiteral::type() const { return KAstBool; }

std::string BoolLiteral::stringify() const { return m_value; }

NoneLiteral::NoneLiteral(Token tok) { m_token = tok; }

Token NoneLiteral::token() const { return m_token; }

AstKind NoneLiteral::type() const { return KAstNone; }

std::string NoneLiteral::stringify() const { return "None"; }

Token NoLiteral::token() const { return Token{}; }

AstKind NoLiteral::type() const { return KAstNoLiteral; }

std::string NoLiteral::stringify() const { return "None"; }

IdentifierExpression::IdentifierExpression(Token tok, std::string_view value) {
    m_token = tok;
    m_value = value;
}

std::string IdentifierExpression::value() const { return m_value; }

Token IdentifierExpression::token() const { return m_token; }

AstKind IdentifierExpression::type() const { return KAstIdentifier; }

std::string IdentifierExpression::stringify() const { return m_value; }

TypeExpression::TypeExpression(Token tok, std::string_view value,
                               std::vector<AstNodePtr> generic_type) {
    m_token = tok;
    m_value = value;
    m_generic_type = generic_type;
}

std::string TypeExpression::value() const { return m_value; }

std::vector<AstNodePtr> TypeExpression::generic_types() const { return m_generic_type; }

Token TypeExpression::token() const { return m_token; }

AstKind TypeExpression::type() const { return KAstTypeExpr; }

std::string TypeExpression::stringify() const { 
    auto res= m_value;
    if(m_generic_type.size()>0){
        res+="{";
        for(size_t i=0;i<m_generic_type.size();i++){
            res+=m_generic_type[i]->stringify();
            if(i<m_generic_type.size()-1){
                res+=",";
            }
        }
        res+="}";
    } 
    return res;
}

ListLiteral::ListLiteral(Token tok,
                         std::vector<AstNodePtr> elements) {
    m_token = tok;
    m_elements = elements;
}


std::vector<AstNodePtr> ListLiteral::elements() const { return m_elements; }

Token ListLiteral::token() const { return m_token; }

AstKind ListLiteral::type() const { return KAstList; }

std::string ListLiteral::stringify() const {
    std::string res = "[";

    for (size_t i = 0; i < m_elements.size(); i++) {
        if (i)
            res += ", ";

        res += m_elements[i]->stringify();
    }

    res += "]";

    return res;
}

DictLiteral::DictLiteral(
    Token tok, std::vector<std::pair<AstNodePtr, AstNodePtr>> elements) {
    m_token = tok;
    m_elements = elements;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>> DictLiteral::elements() const {
    return m_elements;
}

Token DictLiteral::token() const { return m_token; }

AstKind DictLiteral::type() const { return KAstDict; }

std::string DictLiteral::stringify() const {
    std::string res = "{";

    for (size_t i = 0; i < m_elements.size(); i++) {
        if (i)
            res += ", ";

        res += m_elements[i].first->stringify();
        res += " : ";
        res += m_elements[i].second->stringify();
    }

    res += "}";
    return res;
}

UnionLiteral::UnionLiteral(
    Token tok, std::vector<std::pair<AstNodePtr, AstNodePtr>> elements,
    AstNodePtr name,std::string  comment,std::vector<AstNodePtr> generic) {
    m_token = tok;
    m_name = name;
    m_elements = elements;
    m_comment=comment;
    m_generics=generic;
}
std::vector<AstNodePtr> UnionLiteral::generics() const{
    return m_generics;
}
std::vector<std::pair<AstNodePtr, AstNodePtr>> UnionLiteral::elements() const {
    return m_elements;
}

Token UnionLiteral::token() const { return m_token; }

std::string UnionLiteral::comment() const { return m_comment; }

AstNodePtr UnionLiteral::name() const { return m_name; }

AstKind UnionLiteral::type() const { return KAstUnion; }

std::string UnionLiteral::stringify() const {
    std::string res = "union ";
    res += m_name->stringify();
    if(m_generics.size()>0){
        res+="{";
        for(size_t i=0;i<m_generics.size();i++){
            res+=m_generics[i]->stringify();
            if(i<m_generics.size()-1){
                res+=",";
            }
        }
        res+="}";
    }
    res+=+ ":\n";
    for (size_t i = 0; i < m_elements.size(); i++) {
        if (i)
            res += "\n";
        res += m_elements[i].second->stringify();
        res += ":";
        res += m_elements[i].first->stringify();
    }

    return res;
}

EnumLiteral::EnumLiteral(Token tok,
                         std::vector<std::pair<AstNodePtr, AstNodePtr>> fields,
                         AstNodePtr name,std::string comment) {
    m_token = tok;
    m_fields = fields;
    m_name = name;
    m_comment=comment;

}

std::vector<std::pair<AstNodePtr, AstNodePtr>> EnumLiteral::fields() const {
    return m_fields;
}

std::string EnumLiteral::comment() const {return m_comment;}

Token EnumLiteral::token() const { return m_token; }

AstNodePtr EnumLiteral::name() const { return m_name; }

AstKind EnumLiteral::type() const { return KAstEnum; }

std::string EnumLiteral::stringify() const {
    std::string res = "enum ";
    res += m_name->stringify() + ":\n";
    for (size_t i = 0; i < m_fields.size(); i++) {
        if (i)
            res += "\n";

        res += m_fields[i].first->stringify();
        if (m_fields[i].second->type() == KAstNoLiteral) {
        } else {
            res += " = ";
            res += m_fields[i].second->stringify();
        }
    }

    return res;
}

BinaryOperation::BinaryOperation(Token tok, AstNodePtr left, Token op,
                                 AstNodePtr right) {
    m_token = tok;
    m_left = left;
    m_operator = op;
    m_right = right;
}

AstNodePtr BinaryOperation::left() const { return m_left; }

Token BinaryOperation::op() const { return m_operator; }

AstNodePtr BinaryOperation::right() const { return m_right; }

Token BinaryOperation::token() const { return m_token; }

AstKind BinaryOperation::type() const { return KAstBinaryOp; }

std::string BinaryOperation::stringify() const {
    std::string res = "(";

    res += m_left->stringify();
    res += " ";
    res += m_operator.keyword;
    res += " ";
    res += m_right->stringify();
    res += ")";

    return res;
}

PrefixExpression::PrefixExpression(Token tok, Token prefix, AstNodePtr right) {
    m_token = tok;
    m_prefix = prefix;
    m_right = right;
}

Token PrefixExpression::prefix() const { return m_prefix; }

AstNodePtr PrefixExpression::right() const { return m_right; }

Token PrefixExpression::token() const { return m_token; }

AstKind PrefixExpression::type() const { return KAstPrefixExpr; }

std::string PrefixExpression::stringify() const {
    std::string res = "(" + m_prefix.keyword;

    res += m_right->stringify();

    res += ")";

    return res;
}
PostfixExpression::PostfixExpression(Token tok, Token postfix, AstNodePtr left) {
    m_token = tok;
    m_postfix = postfix;
    m_left = left;
}
Token PostfixExpression::postfix() const { return m_postfix; }

AstNodePtr PostfixExpression::left() const { return m_left; }

Token PostfixExpression::token() const { return m_token; }

AstKind PostfixExpression::type() const { return KAstPostfixExpr; }

std::string PostfixExpression::stringify() const {
    std::string res = "(" + m_left->stringify();

    res += m_postfix.keyword;

    res += ")";

    return res;
}

ListOrDictAccess::ListOrDictAccess(Token tok, AstNodePtr container,
                                   std::vector<AstNodePtr> keyOrIndex) {
    m_token = tok;
    m_container = container;
    m_keyOrIndex = keyOrIndex;
}

AstNodePtr ListOrDictAccess::container() const { return m_container; }

std::vector<AstNodePtr> ListOrDictAccess::keyOrIndex() const { return m_keyOrIndex; }

Token ListOrDictAccess::token() const { return m_token; }

AstKind ListOrDictAccess::type() const { return KAstListOrDictAccess; }

std::string ListOrDictAccess::stringify() const {
    std::string res = "";

    res += m_container->stringify() + "[";
    res += m_keyOrIndex[0]->stringify();
    if(m_keyOrIndex.size()==2){
        res +=":" + m_keyOrIndex[1]->stringify();
    }
    res+="]";

    return res;
}

ImportStatement::ImportStatement(
    Token tok, AstNodePtr moduleName,
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols,bool importAll) {
    m_token = tok;
    m_moduleName = moduleName;
    m_importedSymbols = importedSymbols;
    m_importAll=importAll;
}

AstNodePtr ImportStatement::moduleName() const {
    return m_moduleName;
}
bool ImportStatement::importAll() const {
    return m_importAll;
}
std::vector<std::pair<AstNodePtr, AstNodePtr>>
ImportStatement::importedSymbols() const {
    return m_importedSymbols;
}

Token ImportStatement::token() const { return m_token; }

AstKind ImportStatement::type() const { return KAstImportStmt; }

std::string ImportStatement::stringify() const {
    std::string res = "";

    if(m_moduleName->type()==KAstNoLiteral){
        res += "import ";
    }
    else{
        res += "from ";
        res += m_moduleName->stringify();
        res += " import ";
    }
    if(m_importAll){
        res+="*";
    }
    else{
        for(size_t i=0;i<m_importedSymbols.size();i++){
            res += m_importedSymbols[i].first->stringify();
            if(m_importedSymbols[i].second->type()!=KAstNoLiteral){
                res += " as ";
                res += m_importedSymbols[i].second->stringify();
            }
            if(i!=m_importedSymbols.size()-1){
                res += ", ";
            }
        }
    }
    return res;
}

VariableStatement::VariableStatement(Token tok, AstNodePtr type,
                                     AstNodePtr name, AstNodePtr value) {
    m_token = tok;
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr VariableStatement::varType() const { return m_type; }

AstNodePtr VariableStatement::name() const { return m_name; }

AstNodePtr VariableStatement::value() const { return m_value; }

types::TypePtr VariableStatement::processedType() const {
    return m_processedType;
}

void VariableStatement::setProcessedType(types::TypePtr processedType) {
    m_processedType = processedType;
}

Token VariableStatement::token() const { return m_token; }

AstKind VariableStatement::type() const { return KAstVariableStmt; }

std::string VariableStatement::stringify() const {
    std::string res = "(";
    res += m_name->stringify();
    if (m_type->type() != KAstNoLiteral) {
        res += ":";
        res += m_type->stringify();
        res += " ";
    }
    if (m_value->type() != KAstNoLiteral) {
        res += " = ";
        res += m_value->stringify();
    }
    res+=")";
    return res;
}

ConstDeclaration::ConstDeclaration(Token tok, AstNodePtr type, AstNodePtr name,
                                   AstNodePtr value) {
    m_token = tok;
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr ConstDeclaration::constType() const { return m_type; }

AstNodePtr ConstDeclaration::name() const { return m_name; }

AstNodePtr ConstDeclaration::value() const { return m_value; }

types::TypePtr ConstDeclaration::processedType() const {
    return m_processedType;
}

void ConstDeclaration::setProcessedType(types::TypePtr processedType) {
    m_processedType = processedType;
}

Token ConstDeclaration::token() const { return m_token; }

AstKind ConstDeclaration::type() const { return KAstConstDecl; }

std::string ConstDeclaration::stringify() const {
    std::string res = "const ";

    if (m_type->type() != KAstNoLiteral) {
        res += m_type->stringify();
        res += " ";
    }

    res += m_name->stringify();

    res += " = ";
    res += m_value->stringify();

    return res;
}

BlockStatement::BlockStatement(std::vector<AstNodePtr> statements) {
    m_statements = statements;
}

std::vector<AstNodePtr> BlockStatement::statements() const {
    return m_statements;
}

Token BlockStatement::token() const { return Token{}; }

AstKind BlockStatement::type() const { return KAstBlockStmt; }

std::string BlockStatement::stringify() const {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += "    "+stmt->stringify();
        res += "\n";
    }

    return res;
}

ClassDefinition::ClassDefinition(Token tok, AstNodePtr name,
                                 std::vector<AstNodePtr> parent,
                                 std::vector<AstNodePtr> attributes,
                                 std::vector<AstNodePtr> methods,
                                 std::vector<AstNodePtr> other,
                                 std::string comment,std::vector<AstNodePtr> generic) {
    m_token = tok;
    m_name = name;
    m_parent = parent;
    m_attributes = attributes;
    m_methods = methods;
    m_other = other;
    m_comment=comment;
    m_generics=generic;
}

AstNodePtr ClassDefinition::name() const { return m_name; }
std::vector<AstNodePtr> ClassDefinition::generics() const{
    return m_generics;
}
std::vector<AstNodePtr> ClassDefinition::parent() const { return m_parent; }

std::string ClassDefinition::comment() const { return m_comment; }

std::vector<AstNodePtr> ClassDefinition::other() const { return m_other; }

std::vector<AstNodePtr> ClassDefinition::attributes() const {
    return m_attributes;
}

std::vector<AstNodePtr> ClassDefinition::methods() const { return m_methods; }

AstKind ClassDefinition::type() const { return KAstClassDef; }

Token ClassDefinition::token() const { return m_token; }

std::string ClassDefinition::stringify() const {

    std::string res = "class ";
    res += m_name->stringify();
    if(m_generics.size()>0){
        res+="{";
        for(size_t i=0;i<m_generics.size();i++){
            res+=m_generics[i]->stringify();
            if(i<m_generics.size()-1){
                res+=",";
            }
        }
        res+="}";
    }
    res += "(";
    for (size_t i = 0; i < m_parent.size(); ++i) {
        res += m_parent[i]->stringify();
        if (i < m_parent.size() - 1) {
            res += ",";
        }
    }
    res += ")";

    res += ":\n";

    for (auto& stmt : m_attributes) {
        res += stmt->stringify();
        res += "\n";
    }

    for (auto& stmt : m_methods) {
        res += stmt->stringify();
        res += "\n";
    }
    for (auto& stmt : m_other) {
        res += stmt->stringify();
        res += "\n";
    }
    res += "\n";
    return res;
}

FunctionDefinition::FunctionDefinition(Token tok, AstNodePtr returnType,
                                       AstNodePtr name,
                                       std::vector<parameter> parameters,
                                       AstNodePtr body,std::string comment,std::vector<AstNodePtr> generic) {
    m_token = tok;
    m_returnType = returnType;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
    m_comment=comment;
    m_generics=generic;
}
std::vector<AstNodePtr> FunctionDefinition::generics() const{
    return m_generics;
}
AstNodePtr FunctionDefinition::returnType() const { return m_returnType; }

AstNodePtr FunctionDefinition::name() const { return m_name; }

std::vector<parameter> FunctionDefinition::parameters() const {
    return m_parameters;
}
std::string FunctionDefinition::comment() const { return m_comment; }

AstNodePtr FunctionDefinition::body() const { return m_body; }

Token FunctionDefinition::token() const { return m_token; }

AstKind FunctionDefinition::type() const { return KAstFunctionDef; }

std::string FunctionDefinition::stringify() const {
    std::string res = "def ";

    res += m_name->stringify();
    if(m_generics.size()>0){
        res+="{";
        for(size_t i=0;i<m_generics.size();i++){
            res+=m_generics[i]->stringify();
            if(i<m_generics.size()-1){
                res+=",";
            }
        }
        res+="}";
    }
    res += "(";

    if (!m_parameters.empty()) {
        for (size_t i = 0; i < m_parameters.size(); i++) {
            parameter param = m_parameters[i];

            if (i) {
                res += ", ";
            }
            if(param.is_const){
                res += "const ";
            }
            if(param.p_paramType==VarKwarg){
                res += param.p_type->stringify();
                res += param.p_name->stringify();
            }
            else if(param.p_paramType==VarArg){
                res += param.p_type->stringify();
                res += param.p_name->stringify();
            }
            else if(param.p_paramType==Ellipses){
                res += param.p_type->stringify();
                if(param.p_name->type()!=KAstNoLiteral){
                    res += param.p_name->stringify();
                }
            }
            else{
                res += param.p_name->stringify();
                if (param.p_type->type()!=KAstNoLiteral){
                    res += ":";
                    res += param.p_type->stringify();
                }
                if (param.p_default->type()!=ast::KAstNoLiteral){
                    res+="=";
                    res+=param.p_default->stringify();
                }
            }
        }
    }
    res += ") -> ";
    res += m_returnType->stringify();
    res += ":\n";

    res += m_body->stringify();

    return res;
}

ReturnStatement::ReturnStatement(Token tok, AstNodePtr returnValue) {
    m_token = tok;
    m_returnValue = returnValue;
}

AstNodePtr  ReturnStatement::returnValue() const { return m_returnValue; }

Token ReturnStatement::token() const { return m_token; }

AstKind ReturnStatement::type() const { return KAstReturnStatement; }

std::string ReturnStatement::stringify() const {
    std::string res = "return ";

    res+=m_returnValue->stringify();

    return res;
}

FunctionCall::FunctionCall(Token tok, AstNodePtr name,
                           std::vector<AstNodePtr> arguments) {
    m_token = tok;
    m_name = name;
    m_arguments = arguments;
}

AstNodePtr FunctionCall::name() const { return m_name; }

std::vector<AstNodePtr> FunctionCall::arguments() const { return m_arguments; }

Token FunctionCall::token() const { return m_token; }

AstKind FunctionCall::type() const { return KAstFunctionCall; }

std::string FunctionCall::stringify() const {
    std::string res = "";

    res += m_name->stringify();
    res += "(";

    for (size_t i = 0; i < m_arguments.size(); i++) {
        if (i)
            res += ", ";

        res += m_arguments[i]->stringify();
    }

    res += ")";

    return res;
}

DotExpression::DotExpression(Token tok, AstNodePtr owner,
                             AstNodePtr referenced) {
    m_token = tok;
    m_owner = owner;
    m_referenced = referenced;
}

AstNodePtr DotExpression::owner() const { return m_owner; }

AstNodePtr DotExpression::referenced() const { return m_referenced; }

Token DotExpression::token() const { return m_token; }

AstKind DotExpression::type() const { return KAstDotExpression; }

std::string DotExpression::stringify() const {
    std::string res = "";

    res += m_owner->stringify() + ".";
    res += m_referenced->stringify();

    return res;
}

IfStatement::IfStatement(Token tok, AstNodePtr condition, AstNodePtr ifBody,
                         AstNodePtr elseBody,
                         std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs) {
    m_token = tok;
    m_condition = condition;
    m_ifBody = ifBody;
    m_elseBody = elseBody;
    m_elifs = elifs;
}

AstNodePtr IfStatement::condition() const { return m_condition; }

AstNodePtr IfStatement::ifBody() const { return m_ifBody; }

std::vector<std::pair<AstNodePtr, AstNodePtr>> IfStatement::elifs() const {
    return m_elifs;
}

AstNodePtr IfStatement::elseBody() const { return m_elseBody; }

Token IfStatement::token() const { return m_token; }

AstKind IfStatement::type() const { return KAstIfStmt; }

std::string IfStatement::stringify() const {
    std::string res = "if ";

    res += m_condition->stringify();
    res += ":\n";

    res += m_ifBody->stringify();
    res += "\n";

    for (auto& elif : m_elifs) {
        res += "elif ";
        res += elif.first->stringify();
        res += ":\n";

        res += elif.second->stringify();
        res += "\n";
    }

    if (m_elseBody->type() != KAstNoLiteral) {
        res += "else:\n";
        res += m_elseBody->stringify();
        res += "\n";
    }

    return res;
}

AssertStatement::AssertStatement(Token tok, AstNodePtr condition) {
    m_token = tok;
    m_condition = condition;
}

AstNodePtr AssertStatement::condition() const { return m_condition; }

Token AssertStatement::token() const { return m_token; }

AstKind AssertStatement::type() const { return KAstAssertStmt; }

std::string AssertStatement::stringify() const {
    std::string res = "assert ";
    res += m_condition->stringify();
    return res;
}

StaticStatement::StaticStatement(Token tok, AstNodePtr body) {
    m_token = tok;
    m_body = body;
}

AstNodePtr StaticStatement::body() const { return m_body; }

Token StaticStatement::token() const { return m_token; }

AstKind StaticStatement::type() const { return KAstStatic; }

std::string StaticStatement::stringify() const {
    std::string res = "static ";
    res += m_body->stringify();
    return res;
}

ExportStatement::ExportStatement(Token tok, AstNodePtr body) {
    m_token = tok;
    m_body = body;
}

AstNodePtr ExportStatement::body() const { return m_body; }

Token ExportStatement::token() const { return m_token; }

AstKind ExportStatement::type() const { return KAstExport; }

std::string ExportStatement::stringify() const {
    std::string res = "export ";
    res += m_body->stringify();
    return res;
}

InlineStatement::InlineStatement(Token tok, AstNodePtr body) {
    m_token = tok;
    m_body = body;
}

AstNodePtr InlineStatement::body() const { return m_body; }

Token InlineStatement::token() const { return m_token; }

AstKind InlineStatement::type() const { return KAstInline; }

std::string InlineStatement::stringify() const {
    std::string res = "inline ";
    res += m_body->stringify();
    return res;
}

VirtualStatement::VirtualStatement(Token tok, AstNodePtr body) {
    m_token = tok;
    m_body = body;
}

AstNodePtr VirtualStatement::body() const { return m_body; }

Token VirtualStatement::token() const { return m_token; }

AstKind VirtualStatement::type() const { return KAstVirtual; }

std::string VirtualStatement::stringify() const {
    std::string res = "virtual ";
    res += m_body->stringify();
    return res;
}

RaiseStatement::RaiseStatement(Token tok, AstNodePtr value) {
    m_token = tok;
    m_value = value;
}

AstNodePtr RaiseStatement::value() const { return m_value; }

Token RaiseStatement::token() const { return m_token; }

AstKind RaiseStatement::type() const { return KAstRaiseStmt; }

std::string RaiseStatement::stringify() const {
    std::string res = "raise ";
    res += m_value->stringify();
    return res;
}

WhileStatement::WhileStatement(Token tok, AstNodePtr condition,
                               AstNodePtr body) {
    m_token = tok;
    m_condition = condition;
    m_body = body;
}

AstNodePtr WhileStatement::condition() const { return m_condition; }

AstNodePtr WhileStatement::body() const { return m_body; }

Token WhileStatement::token() const { return m_token; }

AstKind WhileStatement::type() const { return KAstWhileStmt; }

std::string WhileStatement::stringify() const {
    std::string res = "while ";

    res += m_condition->stringify();
    res += ":\n";

    res += m_body->stringify();
    res += "\n";

    return res;
}

ForStatement::ForStatement(Token tok, std::vector<AstNodePtr> variable,
                           AstNodePtr sequence, AstNodePtr body) {
    m_token = tok;
    m_variable = variable;
    m_sequence = sequence;
    m_body = body;
}

std::vector<AstNodePtr> ForStatement::variable() const { return m_variable; }

AstNodePtr ForStatement::sequence() const { return m_sequence; }

AstNodePtr ForStatement::body() const { return m_body; }

Token ForStatement::token() const { return m_token; }

AstKind ForStatement::type() const { return KAstForStatement; }

std::string ForStatement::stringify() const {
    std::string res = "for ";
    for (size_t i = 0; i < m_variable.size(); ++i) {
        res += m_variable[i]->stringify();
        if (i < m_variable.size() - 1) {
            res += ",";
        }
    }
    res += " in ";
    res += m_sequence->stringify();
    res += ":\n";

    res += m_body->stringify();
    res += "\n";

    return res;
}

BreakStatement::BreakStatement(Token tok) { m_token = tok; }

Token BreakStatement::token() const { return m_token; }

AstKind BreakStatement::type() const { return KAstBreakStatement; }

std::string BreakStatement::stringify() const { return "break"; }

PassStatement::PassStatement(Token tok) { m_token = tok; }

Token PassStatement::token() const { return m_token; }

AstKind PassStatement::type() const { return KAstPassStatement; }

std::string PassStatement::stringify() const { return m_token.keyword; }

ContinueStatement::ContinueStatement(Token tok) { m_token = tok; }

Token ContinueStatement::token() const { return m_token; }

AstKind ContinueStatement::type() const { return KAstContinueStatement; }

std::string ContinueStatement::stringify() const { return "continue"; }



ScopeStatement::ScopeStatement(Token tok, AstNodePtr body) {
    m_token = tok;
    m_scopeBody = body;
}

AstNodePtr ScopeStatement::body() const { return m_scopeBody; }

Token ScopeStatement::token() const { return m_token; }

AstKind ScopeStatement::type() const { return KAstScopeStmt; }

std::string ScopeStatement::stringify() const {
    std::string res = "scope:\n ";
    res += m_scopeBody->stringify();
    res += "\n";
    return res;
}

TypeDefinition::TypeDefinition(Token tok, AstNodePtr name, AstNodePtr type,std::vector<AstNodePtr> generic) {
    m_token = tok;
    m_name = name;
    m_type = type;
    m_generics = generic;
}
std::vector<AstNodePtr> TypeDefinition::generics() const{
    return m_generics;
}
AstNodePtr TypeDefinition::name() const { return m_name; }

AstNodePtr TypeDefinition::baseType() const { return m_type; }

Token TypeDefinition::token() const { return m_token; }

AstKind TypeDefinition::type() const { return KAstTypeDefinition; }

std::string TypeDefinition::stringify() const {
    std::string res = "type " + m_name->stringify();
    if(m_generics.size()>0){
        res+="{";
        for(size_t i=0;i<m_generics.size();i++){
            res+=m_generics[i]->stringify();
            if(i<m_generics.size()-1){
                res+=",";
            }
        }
        res+="}";
    }
    res += " = ";

    res += m_type->stringify();

    return res;
}

MatchStatement::MatchStatement(
    Token tok, std::vector<AstNodePtr> toMatch,
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases,
    AstNodePtr defaultbody) {
    m_token = tok;
    m_toMatch = toMatch;
    m_cases = cases;
    m_default = defaultbody;
}

std::vector<AstNodePtr> MatchStatement::matchItem() const { return m_toMatch; }

std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>>
MatchStatement::caseBody() const {
    return m_cases;
}

AstNodePtr MatchStatement::defaultBody() const { return m_default; }

Token MatchStatement::token() const { return m_token; }

AstKind MatchStatement::type() const { return KAstMatchStmt; }

std::string MatchStatement::stringify() const {
    std::string res = "match ";
    for (auto& temp : m_toMatch) {
        res += temp->stringify() + ",";
    }
    res += ":\n";
    for (auto& elif : m_cases) {
        res += "case ";
        auto temp = elif.first;
        for (auto& x : temp) {
            res += x->stringify() + ",";
        }
        res += ":\n";

        res += elif.second->stringify();
        res += "\n";
    }

    if (m_default->type() != KAstNoLiteral) {
        res += "default:\n";
        res += m_default->stringify();
        res += "\n";
    }

    return res;
}

ListTypeExpr::ListTypeExpr(Token tok, AstNodePtr elemType, AstNodePtr size) {
    m_token = tok;
    m_elemType = elemType;
    m_size = size;
}

AstNodePtr ListTypeExpr::elemType() const { return m_elemType; }

AstNodePtr ListTypeExpr::size() const { return m_size; }

Token ListTypeExpr::token() const { return m_token; }

AstKind ListTypeExpr::type() const { return KAstListTypeExpr; }

std::string ListTypeExpr::stringify() const {
    std::string res = "[";

    res += m_size->stringify();

    res += "]";

    res += m_elemType->stringify();
    return res;
}

PointerTypeExpr::PointerTypeExpr(Token tok, AstNodePtr baseType) {
    m_token = tok;
    m_baseType = baseType;
}

AstNodePtr PointerTypeExpr::baseType() const { return m_baseType; }

Token PointerTypeExpr::token() const { return m_token; }

AstKind PointerTypeExpr::type() const { return KAstPointerTypeExpr; }

std::string PointerTypeExpr::stringify() const {
    std::string res = "*";
    res += m_baseType->stringify();
    return res;
}

RefTypeExpr::RefTypeExpr(Token tok, AstNodePtr baseType) {
    m_token = tok;
    m_baseType = baseType;
}

AstNodePtr RefTypeExpr::baseType() const { return m_baseType; }

Token RefTypeExpr::token() const { return m_token; }

AstKind RefTypeExpr::type() const { return KAstRefTypeExpr; }

std::string RefTypeExpr::stringify() const {
    std::string res = "&";
    res += m_baseType->stringify();
    return res;
}

DictTypeExpr::DictTypeExpr(Token tok, AstNodePtr keyType,
                           AstNodePtr valueType) {
    m_token = tok;
    m_keyType = keyType;
    m_valueType = valueType;
}

AstNodePtr DictTypeExpr::keyType() const { return m_keyType; }

AstNodePtr DictTypeExpr::valueType() const { return m_valueType; }

Token DictTypeExpr::token() const { return m_token; }

AstKind DictTypeExpr::type() const { return KAstDictTypeExpr; }

// may change
std::string DictTypeExpr::stringify() const {
    std::string res = "dict[" + m_keyType->stringify();

    res += "]" + m_valueType->stringify();

    return res;
}

FunctionTypeExpr::FunctionTypeExpr(Token tok, std::vector<AstNodePtr> argTypes,
                                   AstNodePtr returnTypes) {
    m_token = tok;
    m_argTypes = argTypes;
    m_returnTypes = returnTypes;
}
AstKind FunctionTypeExpr::type() const { return KAstFuncTypeExpr; }
Token FunctionTypeExpr::token() const { return m_token; }
std::vector<AstNodePtr> FunctionTypeExpr::argTypes() const {
    return m_argTypes;
}
AstNodePtr FunctionTypeExpr::returnTypes() const {
    return m_returnTypes;
}

std::string FunctionTypeExpr::stringify() const {
    std::string res = "def(";
    if (m_argTypes.size() > 0) {
        for(size_t i=0; i<m_argTypes.size(); i++) {
            res += m_argTypes[i]->stringify();
            if(i<m_argTypes.size()-1) {
                res += ",";
            }
        }
    }
    res += ")";
    res += "->";
    res += m_returnTypes->stringify();
    return res;
}

DecoratorStatement::DecoratorStatement(Token tok,
                                       std::vector<AstNodePtr> decorators,
                                       AstNodePtr body) {
    m_token = tok;
    m_decorators = decorators;
    m_body = body;
}
std::vector<AstNodePtr> DecoratorStatement::decoratorItem() const {
    return m_decorators;
}
AstNodePtr DecoratorStatement::body() const { return m_body; }
Token DecoratorStatement::token() const { return m_token; }
AstKind DecoratorStatement::type() const { return KAstDecorator; }
std::string DecoratorStatement::stringify() const {
    std::string res;
    for (auto& x : m_decorators) {
        res += "@" + x->stringify() + "\n";
    }
    res += m_body->stringify();
    return res;
}

WithStatement::WithStatement(Token tok, std::vector<AstNodePtr> variables,
                             std::vector<AstNodePtr> values, AstNodePtr body) {
    m_token = tok;
    m_variables = variables;
    m_values = values;
    m_body = body;
}
std::vector<AstNodePtr> WithStatement::variables() const { return m_variables; }
std::vector<AstNodePtr> WithStatement::values() const { return m_values; }
AstNodePtr WithStatement::body() const { return m_body; }
Token WithStatement::token() const { return m_token; }
AstKind WithStatement::type() const { return KAstWith; }
std::string WithStatement::stringify() const {
    std::string res = "with ";
    for (size_t i = 0; i < m_values.size(); ++i) {
        res += m_values[i]->stringify();
        if(m_variables[i]->type()!=KAstNoLiteral){
            res += " as ";
            res += m_variables[i]->stringify();
        }
        if (i < m_values.size() - 1) {
            res += ",";
        }
    }
    res += ":\n";
    res += m_body->stringify();
    return res;
}
CastStatement::CastStatement(Token token, AstNodePtr type, AstNodePtr value) {
    m_token = token;
    m_type = type;
    m_value = value;
}
AstNodePtr CastStatement::cast_type() const { return m_type; }
AstNodePtr CastStatement::value() const { return m_value; }
Token CastStatement::token() const { return m_token; }
AstKind CastStatement::type() const { return KAstCast; }
std::string CastStatement::stringify() const {
    std::string res = "cast";
    res += "<" + m_type->stringify() + ">(" + m_value->stringify() + ")";
    return res;
}
DefaultArg::DefaultArg(Token tok, AstNodePtr name, AstNodePtr value) {
    m_token = tok;
    m_name = name;
    m_value = value;
}

AstNodePtr DefaultArg::value() const { return m_value; }

AstNodePtr DefaultArg::name() const { return m_name; }

Token DefaultArg::token() const { return m_token; }

AstKind DefaultArg::type() const { return KAstDefaultArg; }

std::string DefaultArg::stringify() const {
    std::string res = "";

    res += m_name->stringify() + "=";
    res += m_value->stringify();

    return res;
}
ArrowExpression::ArrowExpression(Token tok, AstNodePtr owner,
                             AstNodePtr referenced) {
    m_token = tok;
    m_owner = owner;
    m_referenced = referenced;
}

AstNodePtr ArrowExpression::owner() const { return m_owner; }

AstNodePtr ArrowExpression::referenced() const { return m_referenced; }

Token ArrowExpression::token() const { return m_token; }

AstKind ArrowExpression::type() const { return KAstArrowExpression; }

std::string ArrowExpression::stringify() const {
    std::string res = "";

    res += m_owner->stringify() + "->";
    res += m_referenced->stringify();
    return res;
}

TernaryIf::TernaryIf(Token token,AstNodePtr if_value,AstNodePtr if_condition,AstNodePtr else_value){
  m_token=token;
  m_if_value=if_value;
  m_if_condition=if_condition;
  m_else_value=else_value;
}
AstNodePtr TernaryIf::if_value() const{return m_if_value;}
AstNodePtr TernaryIf::if_condition() const{return m_if_condition;}
AstNodePtr TernaryIf::else_value() const{return m_else_value;}
Token TernaryIf::token() const{return m_token;}
AstKind TernaryIf::type() const{return KAstTernaryIf;}
std::string TernaryIf::stringify() const{
  std::string res="("+m_if_value->stringify()+" if("+m_if_condition->stringify()+") else "+m_else_value->stringify()+")";
  return res;
}
TryExcept::TryExcept(Token token,AstNodePtr body,std::vector<except_type>  except_clauses,AstNodePtr else_body){
    m_token=token;
    m_body=body;
    m_except_clauses=except_clauses;
    m_else_body=else_body;
}
AstNodePtr TryExcept::body() const{return m_body;}
std::vector<except_type> TryExcept::except_clauses() const{return m_except_clauses;}
AstNodePtr TryExcept::else_body() const{return m_else_body;}
Token TryExcept::token() const{return m_token;}
AstKind TryExcept::type() const{return KAstTryExcept;}
std::string TryExcept::stringify() const{
    std::string res="try:\n";
    res+=m_body->stringify();
    res+="\n";
    for(size_t i=0;i<m_except_clauses.size();++i){
        except_type x=m_except_clauses[i];
        res+="except ";
        for (size_t i=0;i<x.first.first.size();++i){
            res+=x.first.first[i]->stringify();
            if(i<x.first.first.size()-1){
                res+=",";
            }
        }
        res+=" as ";
        res+=x.first.second->stringify();
        res+=":\n";
        res+=x.second->stringify();
        res+="\n";
    }
    res+="except:\n"+m_else_body->stringify();
    return res;
}
ExpressionTuple::ExpressionTuple(bool multiple_return,std::vector<AstNodePtr> items){
    m_multiple_return=multiple_return;
    m_items=items;
}
std::vector<AstNodePtr> ExpressionTuple::items() const{return m_items;}
bool ExpressionTuple::multiple_return() const{return m_multiple_return;}
AstKind ExpressionTuple::type() const{return KAstExpressionTuple;}
std::string ExpressionTuple::stringify() const{
    std::string res;
    if(!m_multiple_return){
        res+="(";
    }
    for (size_t i=0;i<m_items.size();++i){
        res+=m_items[i]->stringify();
        if(i<m_items.size()-1){
            res+=",";
        }
    }
    if(!m_multiple_return){
        res+=")";
    }
    return res;
}
Token ExpressionTuple::token() const { return m_items[0]->token(); }
TypeTuple::TypeTuple(bool multiple_return,std::vector<AstNodePtr> items){
    m_multiple_return=multiple_return;
    m_items=items;
}
std::vector<AstNodePtr> TypeTuple::items() const{return m_items;}
bool TypeTuple::multiple_return() const{return m_multiple_return;}
AstKind TypeTuple::type() const{return KAstTypeTuple;}
std::string TypeTuple::stringify() const{
    std::string res;
    if(!m_multiple_return){
        res+="(";
    }
    for (size_t i=0;i<m_items.size();++i){
        res+=m_items[i]->stringify();
        if(i<m_items.size()-1){
            res+=",";
        }
    }
    if(!m_multiple_return){
        res+=")";
    }
    return res;
}
Token TypeTuple::token() const { return m_items[0]->token(); }

ExternStatement::ExternStatement(Token token,std::vector<std::string> libs,std::string name){
    m_token=token;
    m_libs=libs;
    m_name=name;
}
std::vector<std::string> ExternStatement::libs() const{return m_libs;}
std::string ExternStatement::name() const{return m_name;}
AstKind ExternStatement::type() const{return KAstExternStatement;}
Token ExternStatement::token() const{return m_token;}
std::string ExternStatement::stringify() const{
    std::string res="extern ";
    res+=m_name+"=import(";
    for (size_t i=0;i<m_libs.size();++i){
        res+=m_libs[i];
        if(i<m_libs.size()-1){
            res+=",";
        }
    }
    res+=")";
    return res;
}
SumType::SumType(std::vector<AstNodePtr> types){
    m_types=types;
}
std::vector<AstNodePtr> SumType::sum_types() const{return m_types;}
AstKind SumType::type() const{return KAstSumType;}
std::string SumType::stringify() const{
    std::string res="(";
    for (size_t i=0;i<m_types.size();++i){
        res+=m_types[i]->stringify();
        if(i<m_types.size()-1){
            res+="|";
        }
    }
    res+=")";
    return res;
}
Token SumType::token() const{return m_types[0]->token();}
MultipleAssign::MultipleAssign(std::vector<AstNodePtr> names,std::vector<AstNodePtr> values){
    m_names=names;
    m_values=values;
}
std::vector<AstNodePtr> MultipleAssign::names() const{return m_names;}
std::vector<AstNodePtr> MultipleAssign::values() const{return m_values;}
AstKind MultipleAssign::type() const{return KAstMultipleAssign;}
std::string MultipleAssign::stringify() const{
    std::string res="((";
    for (size_t i=0;i<m_names.size();++i){
        res+=m_names[i]->stringify();
        if(i<m_names.size()-1){
            res+=",";
        }
    }
    res+=")=(";
    for (size_t i=0;i<m_values.size();++i){
        res+=m_values[i]->stringify();
        if(i<m_values.size()-1){
            res+=",";
        }
    }
    res+="))";
    return res;
}
Token MultipleAssign::token() const{return m_names[0]->token();}
AugAssign::AugAssign(Token tok, AstNodePtr name, AstNodePtr value){
    m_token=tok;
    m_name=name;
    m_value=value;
}
AstKind AugAssign::type() const{return KAstAugAssign;}
Token AugAssign::token() const{return m_token;}
AstNodePtr AugAssign::name() const{return m_name;}
AstNodePtr AugAssign::value() const{return m_value;}
std::string AugAssign::op() const{return m_token.keyword;}
std::string AugAssign::stringify() const{
    std::string res=m_name->stringify();
    res+=m_token.keyword;
    res+=m_value->stringify();
    return res;
}
MethodDefinition::MethodDefinition(Token tok, AstNodePtr returnType, AstNodePtr name,
                       std::vector<parameter> parameters,parameter reciever, AstNodePtr body,std::string comment,std::vector<AstNodePtr> generic) {
    m_token = tok;
    m_returnType = returnType;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
    m_comment=comment;
    m_reciever=reciever;
    m_generics=generic;
}
std::vector<AstNodePtr> MethodDefinition::generics() const{
    return m_generics;
}
AstNodePtr MethodDefinition::returnType() const { return m_returnType; }

parameter MethodDefinition::reciever() const { return m_reciever; }

AstNodePtr MethodDefinition::name() const { return m_name; }

std::vector<parameter> MethodDefinition::parameters() const {
    return m_parameters;
}
std::vector<parameter> MethodDefinition::codegen_parameters() const {
    auto v=m_parameters;
    v.insert(v.begin(), m_reciever);
    return v;
}
std::string MethodDefinition::comment() const { return m_comment; }

AstNodePtr MethodDefinition::body() const { return m_body; }

Token MethodDefinition::token() const { return m_token; }

AstKind MethodDefinition::type() const { return KAstMethodDef; }

std::string MethodDefinition::stringify() const {
    std::string res = "def (";
    if(m_reciever.is_const){
        res+="const ";
    }
    res += m_reciever.p_name->stringify();
    if (m_reciever.p_type->type()!=KAstNoLiteral){
        res += ":";
        res += m_reciever.p_type->stringify();
    }
    if (m_reciever.p_default->type()!=ast::KAstNoLiteral){
        res+="=";
        res+=m_reciever.p_default->stringify();
    }
    res+=")";
    res += m_name->stringify();
    if(m_generics.size()>0){
        res+="{";
        for(size_t i=0;i<m_generics.size();i++){
            res+=m_generics[i]->stringify();
            if(i<m_generics.size()-1){
                res+=",";
            }
        }
        res+="}";
    }
    res += "(";

    if (!m_parameters.empty()) {
        for (size_t i = 0; i < m_parameters.size(); i++) {
            parameter param = m_parameters[i];

            if (i) {
                res += ", ";
            }
            if(param.is_const){
                res += "const ";
            }
            res += param.p_name->stringify();
            if (param.p_type->type()!=KAstNoLiteral){
                res += ":";
                res += param.p_type->stringify();
            }
            if (param.p_default->type()!=ast::KAstNoLiteral){
                res+="=";
                res+=param.p_default->stringify();
            }
        }
    }
    res += ") -> ";
    res += m_returnType->stringify();
    res += ":\n";

    res += m_body->stringify();

    return res;
}
ExternFuncDef::ExternFuncDef(Token tok, AstNodePtr returnType, AstNodePtr name,
                       std::vector<AstNodePtr> parameters,std::string owner){
    m_token=tok;
    m_returnType=returnType;
    m_name=name;
    m_parameters=parameters;
    m_owner=owner;                   
}
AstNodePtr ExternFuncDef::returnType() const{return m_returnType;}
AstNodePtr ExternFuncDef::name() const{return m_name;}
std::string ExternFuncDef::owner() const{return m_owner;}
std::vector<AstNodePtr> ExternFuncDef::parameters() const{return m_parameters;}
Token ExternFuncDef::token() const{return m_token;}
AstKind ExternFuncDef::type() const{return KAstExternFuncDef;}
std::string ExternFuncDef::stringify() const{
    std::string res="def "+m_owner+".";
    res+=m_name->stringify();
    res+="(";
    for (size_t i=0;i<m_parameters.size();++i){
        res+=m_parameters[i]->stringify();
        if(i<m_parameters.size()-1){
            res+=",";
        }
    }
    res+=")->";
    res+=m_returnType->stringify();
    return res;
}

ExternUnionLiteral::ExternUnionLiteral(
    Token tok, std::vector<std::pair<AstNodePtr, AstNodePtr>> elements,
    AstNodePtr name,std::string  owner) {
    m_token = tok;
    m_name = name;
    m_elements = elements;
    m_owner=owner;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>> ExternUnionLiteral::elements() const {
    return m_elements;
}

Token ExternUnionLiteral::token() const { return m_token; }

std::string ExternUnionLiteral::owner() const { return m_owner; }

AstNodePtr ExternUnionLiteral::name() const { return m_name; }

AstKind ExternUnionLiteral::type() const { return KAstExternUnion; }

std::string ExternUnionLiteral::stringify() const {
    std::string res = "union "+m_owner+".";
    res += m_name->stringify() + ":\n";
    for (size_t i = 0; i < m_elements.size(); i++) {
        if (i)
            res += "\n";
        res += m_elements[i].second->stringify();
        res += ":";
        res += m_elements[i].first->stringify();
    }

    return res;
}

ExternStructLiteral::ExternStructLiteral(
    Token tok, std::vector<std::pair<AstNodePtr, AstNodePtr>> elements,
    AstNodePtr name,std::string  owner) {
    m_token = tok;
    m_name = name;
    m_elements = elements;
    m_owner=owner;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>> ExternStructLiteral::elements() const {
    return m_elements;
}

Token ExternStructLiteral::token() const { return m_token; }

std::string ExternStructLiteral::owner() const { return m_owner; }

AstNodePtr ExternStructLiteral::name() const { return m_name; }

AstKind ExternStructLiteral::type() const { return KAstExternStruct; }

std::string ExternStructLiteral::stringify() const {
    std::string res = "class "+m_owner+".";
    res += m_name->stringify() + ":\n";
    for (size_t i = 0; i < m_elements.size(); i++) {
        if (i)
            res += "\n";
        res += m_elements[i].second->stringify();
        res += ":";
        res += m_elements[i].first->stringify();
    }

    return res;
}
EllipsesTypeExpr::EllipsesTypeExpr(Token tok) {
    m_token = tok;
}
Token EllipsesTypeExpr::token() const { return m_token; }
AstKind EllipsesTypeExpr::type() const { return KAstEllipsesTypeExpr; }
std::string EllipsesTypeExpr::stringify() const { return m_token.keyword ; }

VarKwargTypeExpr::VarKwargTypeExpr(Token tok) {
    m_token = tok;
}
Token VarKwargTypeExpr::token() const { return m_token; }
AstKind VarKwargTypeExpr::type() const { return KAstVarKwargTypeExpr; }
std::string VarKwargTypeExpr::stringify() const { return "**" ; }

VarArgTypeExpr::VarArgTypeExpr(Token tok) {
    m_token = tok;
}
Token VarArgTypeExpr::token() const { return m_token; }
AstKind VarArgTypeExpr::type() const { return KAstVarArgTypeExpr; }
std::string VarArgTypeExpr::stringify() const { return m_token.keyword ; }
CompileTimeExpression::CompileTimeExpression(Token tok, AstNodePtr expr_node) {
    m_token = tok;
    m_expr_node = expr_node;
}
AstNodePtr CompileTimeExpression::expression() const{return m_expr_node;}
Token CompileTimeExpression::token() const { return m_token; }
AstKind CompileTimeExpression::type() const { return KAstCompileTimeExpression; }
std::string CompileTimeExpression::stringify() const {
    return"$" + m_expr_node->stringify();
}
TernaryFor::TernaryFor(Token token,AstNodePtr for_value,AstNodePtr for_iterate,std::vector<AstNodePtr> for_variable){
    m_token=token;
    m_for_value=for_value;
    m_for_iterate=for_iterate;
    m_for_variable=for_variable;
}
AstNodePtr TernaryFor::for_value()const{return m_for_value;}
AstNodePtr TernaryFor::for_iterate()const{return m_for_iterate;}
std::vector<AstNodePtr> TernaryFor::for_variable()const{return m_for_variable;}
Token TernaryFor::token()const{return m_token;}
AstKind TernaryFor::type()const{return KAstTernaryFor;}
std::string TernaryFor::stringify()const{
    std::string res;
    res+="("+m_for_value->stringify();
    res+=" for ";
    for(size_t i=0;i<m_for_variable.size();++i){
        res+=m_for_variable[i]->stringify();
        if(i<m_for_variable.size()-1){
            res+=",";
        }
    }
    res+=" in ";
    res+=m_for_iterate->stringify();
    res+=")";
    return res;
}
PrivateDef::PrivateDef(Token tok, AstNodePtr definition) {
    m_token = tok;
    m_definition = definition;
}
AstNodePtr PrivateDef::definition() const{return m_definition;}
Token PrivateDef::token() const { return m_token; }
AstKind PrivateDef::type() const { return KAstPrivate; }
std::string PrivateDef::stringify() const {
    return"private " + m_definition->stringify();
}
InlineAsm::InlineAsm(Token token,std::string assembly,AstNodePtr output,std::vector<std::pair<std::string,AstNodePtr>>inputs){
    m_token=token;
    m_assembly=assembly;
    m_output=output;
    m_inputs=inputs;
}
std::string InlineAsm::assembly() const{return m_assembly;}
AstNodePtr InlineAsm::output() const{return m_output;}
std::vector<std::pair<std::string,AstNodePtr>> InlineAsm::inputs() const{return m_inputs;}
Token InlineAsm::token() const { return m_token; }
AstKind InlineAsm::type() const { return KAstInlineAsm; }
std::string InlineAsm::stringify() const{
    std::string res ="__asm__:\n";
    if(m_output->type()==KAstNoLiteral){
        res+="    "+m_assembly+"\n";
    }
    else{
        res+="    "+m_output->stringify()+" = "+m_assembly+"\n";
    }
    for(size_t i=0;i<m_inputs.size();++i){
        res+="    "+m_inputs[i].first+" = "+m_inputs[i].second->stringify()+"\n";
    }
    return res;
}
LambdaDefinition::LambdaDefinition(Token tok,std::vector<parameter> parameters, AstNodePtr body){
    m_tok=tok;
    m_parameters=parameters;
    m_body=body;
}
std::vector<parameter> LambdaDefinition::parameters() const{
    return m_parameters;
}
AstNodePtr LambdaDefinition::body() const{
    return m_body;
}
Token LambdaDefinition::token() const{
    return m_tok;
}
AstKind LambdaDefinition::type() const{
    return KAstLambda;
}
std::string LambdaDefinition::stringify() const{
    std::string res="def(";
    if (!m_parameters.empty()) {
        for (size_t i = 0; i < m_parameters.size(); i++) {
            parameter param = m_parameters[i];

            if (i) {
                res += ", ";
            }
            if(param.is_const){
                res += "const ";
            }
            if(param.p_paramType==VarKwarg){
                res += param.p_type->stringify();
                res += param.p_name->stringify();
            }
            else if(param.p_paramType==VarArg){
                res += param.p_type->stringify();
                res += param.p_name->stringify();
            }
            else if(param.p_paramType==Ellipses){
                res += param.p_type->stringify();
                if(param.p_name->type()!=KAstNoLiteral){
                    res += param.p_name->stringify();
                }
            }
            else{
                res += param.p_name->stringify();
                if (param.p_type->type()!=KAstNoLiteral){
                    res += ":";
                    res += param.p_type->stringify();
                }
                if (param.p_default->type()!=ast::KAstNoLiteral){
                    res+="=";
                    res+=param.p_default->stringify();
                }
            }
        }
    }
    res+="):"+m_body->stringify();
    return res;
}
GenericCall::GenericCall(Token tok,std::vector<AstNodePtr> generic_types,AstNodePtr identifier){
    m_tok=tok;
    m_generic_types=generic_types;
    m_identifier=identifier;
}
std::vector<AstNodePtr> GenericCall::generic_types() const{
    return m_generic_types;
}
AstNodePtr GenericCall::identifier() const{
    return m_identifier;
}
Token GenericCall::token() const{
    return m_tok;
}
AstKind GenericCall::type() const{
    return KAstGenericCall;
}
std::string GenericCall::stringify() const{
    std::string res=m_identifier->stringify();
    res+="{";
    for(size_t i=0;i<m_generic_types.size();++i){
        res+=m_generic_types[i]->stringify();
        if(i<m_generic_types.size()-1){
            res+=",";
        }
    }
    res+="}";
    return res;
}

} // namespace ast
