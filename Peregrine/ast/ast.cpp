#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace ast {

Program::Program(std::vector<AstNodePtr> statements,std::string  comment) {
    m_statements = statements;
    m_comment=comment;
}

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

TypeExpression::TypeExpression(Token tok, std::string_view value) {
    m_token = tok;
    m_value = value;
}

std::string TypeExpression::value() const { return m_value; }

Token TypeExpression::token() const { return m_token; }

AstKind TypeExpression::type() const { return KAstTypeExpr; }

std::string TypeExpression::stringify() const { return m_value; }

ListLiteral::ListLiteral(Token tok, AstNodePtr type,
                         std::vector<AstNodePtr> elements) {
    m_token = tok;
    m_type = type;
    m_elements = elements;
}

AstNodePtr ListLiteral::listType() const { return m_type; }

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
    AstNodePtr name,std::string  comment) {
    m_token = tok;
    m_name = name;
    m_elements = elements;
    m_comment=comment;
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
    Token tok, std::pair<AstNodePtr, AstNodePtr> moduleName,
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols) {
    m_token = tok;
    m_moduleName = moduleName;
    m_importedSymbols = importedSymbols;
}

std::pair<AstNodePtr, AstNodePtr> ImportStatement::moduleName() const {
    return m_moduleName;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>>
ImportStatement::importedSymbols() const {
    return m_importedSymbols;
}

Token ImportStatement::token() const { return m_token; }

AstKind ImportStatement::type() const { return KAstImportStmt; }

std::string ImportStatement::stringify() const {
    std::string res = "";

    res += (!m_importedSymbols.size()) ? "import " : "from ";

    res += m_moduleName.first->stringify();
    // hmm
    if (m_moduleName.second) {
        res += " as " + m_moduleName.second->stringify();
    }

    if (!m_importedSymbols.size())
        return res;

    res += " import ";

    for (size_t i = 0; i < m_importedSymbols.size(); i++) {
        if (i)
            res += ", ";

        res += m_importedSymbols[i].first->stringify();

        if (m_importedSymbols[i].second) {
            res += " as " + m_importedSymbols[i].second->stringify();
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
        res += stmt->stringify();
        res += "\n";
    }

    return res;
}

ClassDefinition::ClassDefinition(Token tok, AstNodePtr name,
                                 std::vector<AstNodePtr> parent,
                                 std::vector<AstNodePtr> attributes,
                                 std::vector<AstNodePtr> methods,
                                 std::vector<AstNodePtr> other,
                                 std::string comment) {
    m_token = tok;
    m_name = name;
    m_parent = parent;
    m_attributes = attributes;
    m_methods = methods;
    m_other = other;
    m_comment=comment;
}

AstNodePtr ClassDefinition::name() const { return m_name; }

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
                                       AstNodePtr body,std::string comment) {
    m_token = tok;
    m_returnType = returnType;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
    m_comment=comment;
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
    res += "(";

    if (!m_parameters.empty()) {
        for (size_t i = 0; i < m_parameters.size(); i++) {
            parameter param = m_parameters[i];

            if (i) {
                res += ", ";
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

ReturnStatement::ReturnStatement(Token tok, AstNodePtr returnValue) {
    m_token = tok;
    m_returnValue = returnValue;
}

AstNodePtr ReturnStatement::returnValue() const { return m_returnValue; }

Token ReturnStatement::token() const { return m_token; }

AstKind ReturnStatement::type() const { return KAstReturnStatement; }

std::string ReturnStatement::stringify() const {
    std::string res = "return";

    if (m_returnValue->type() != KAstNoLiteral) {
        res += " " + m_returnValue->stringify();
    }

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

std::string PassStatement::stringify() const { return "pass"; }

ContinueStatement::ContinueStatement(Token tok) { m_token = tok; }

Token ContinueStatement::token() const { return m_token; }

AstKind ContinueStatement::type() const { return KAstContinueStatement; }

std::string ContinueStatement::stringify() const { return "continue"; }

CppStatement::CppStatement(Token tok, std::string cpp_code) {
    m_token = tok;
    m_cppCode = cpp_code;
}

std::string CppStatement::value() const { return m_cppCode; }

Token CppStatement::token() const { return m_token; }

AstKind CppStatement::type() const { return KAstCpp; }

std::string CppStatement::stringify() const {
    std::string res = "Cppcode";

    res += "(" + m_cppCode + ")";

    return res;
}

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

TypeDefinition::TypeDefinition(Token tok, AstNodePtr name, AstNodePtr type) {
    m_token = tok;
    m_name = name;
    m_type = type;
}

AstNodePtr TypeDefinition::name() const { return m_name; }

AstNodePtr TypeDefinition::baseType() const { return m_type; }

Token TypeDefinition::token() const { return m_token; }

AstKind TypeDefinition::type() const { return KAstTypeDefinition; }

std::string TypeDefinition::stringify() const {
    std::string res = "type " + m_name->stringify();

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
                                   std::vector<AstNodePtr> returnTypes) {
    m_token = tok;
    m_argTypes = argTypes;
    m_returnTypes = returnTypes;
}
AstKind FunctionTypeExpr::type() const { return KAstFuncTypeExpr; }
Token FunctionTypeExpr::token() const { return m_token; }
std::vector<AstNodePtr> FunctionTypeExpr::argTypes() const {
    return m_argTypes;
}
std::vector<AstNodePtr> FunctionTypeExpr::returnTypes() const {
    return m_returnTypes;
}

std::string FunctionTypeExpr::stringify() const {
    std::string res = "def(";
    if (m_argTypes.size() > 0) {
        for (auto& x : m_argTypes) {
            res += x->stringify() + ",";
        }
    }
    res += ")";
    if (m_returnTypes.size() > 0) {
        res += "->";
        for (auto& x : m_returnTypes) {
            res += x->stringify() + ",";
        }
    }
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
        // std::cout<<res<<"\n";
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

} // namespace ast
