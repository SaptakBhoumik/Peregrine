#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <string>
#include <vector>

Program::Program(std::vector<AstNodePtr> statements) {
    m_statements = statements;
}

std::vector<AstNodePtr> Program::statements() { return m_statements; }

AstKind Program::type() { return KAstProgram; }

std::string Program::stringify() {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt->stringify();
        res += "\n";
    }

    return res;
}

IntegerLiteral::IntegerLiteral(std::string_view value) { m_value = value; }

std::string IntegerLiteral::value() { return m_value; }

AstKind IntegerLiteral::type() { return KAstInteger; }

std::string IntegerLiteral::stringify() { return m_value; }

DecimalLiteral::DecimalLiteral(std::string_view value) { m_value = value; }

std::string DecimalLiteral::value() { return m_value; }

AstKind DecimalLiteral::type() { return KAstDecimal; }

std::string DecimalLiteral::stringify() { return m_value; }

StringLiteral::StringLiteral(std::string_view value, bool formatted, bool raw) {
    m_value = value;
    m_formatted = formatted;
    m_raw = raw;
}

std::string StringLiteral::value() { return m_value; }

bool StringLiteral::formatted() { return m_formatted; }

bool StringLiteral::raw() { return m_raw; }

AstKind StringLiteral::type() { return KAstString; }

std::string StringLiteral::stringify() { return m_value; }

BoolLiteral::BoolLiteral(std::string_view value) { m_value = value; }

std::string BoolLiteral::value() { return m_value; }

AstKind BoolLiteral::type() { return KAstBool; }

std::string BoolLiteral::stringify() { return m_value; }

NoneLiteral::NoneLiteral() {}

AstKind NoneLiteral::type() { return KAstNone; }

std::string NoneLiteral::stringify() { return "None"; }

NoLiteral::NoLiteral() {}

AstKind NoLiteral::type() { return KAstNoLiteral; }

std::string NoLiteral::stringify() { return ""; }

IdentifierExpression::IdentifierExpression(std::string_view value) {
    m_value = value;
}

std::string IdentifierExpression::value() { return m_value; }

AstKind IdentifierExpression::type() { return KAstIdentifier; }

std::string IdentifierExpression::stringify() { return m_value; }

TypeExpression::TypeExpression(std::string_view value) { m_value = value; }

std::string TypeExpression::value() { return m_value; }

AstKind TypeExpression::type() { return KAstTypeExpr; }

std::string TypeExpression::stringify() { return m_value; }

ListLiteral::ListLiteral(AstNodePtr type, std::vector<AstNodePtr> elements) {
    m_type = type;
    m_elements = elements;
}

AstNodePtr ListLiteral::listType() { return m_type; }

std::vector<AstNodePtr> ListLiteral::elements() { return m_elements; }

AstKind ListLiteral::type() { return KAstList; }

std::string ListLiteral::stringify() {
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
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements) {
    m_elements = elements;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>> DictLiteral::elements() {
    return m_elements;
}

AstKind DictLiteral::type() { return KAstDict; }

std::string DictLiteral::stringify() {
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

BinaryOperation::BinaryOperation(AstNodePtr left, Token op, AstNodePtr right) {
    m_left = left;
    m_operator = op;
    m_right = right;
}

AstNodePtr BinaryOperation::left() { return m_left; }

Token BinaryOperation::op() { return m_operator; }

AstNodePtr BinaryOperation::right() { return m_right; }

AstKind BinaryOperation::type() { return KAstBinaryOp; }

// TODO: this can surely be better
std::string BinaryOperation::stringify() {
    std::string res = "(";

    res += m_left->stringify();
    res += " ";
    res += m_operator.keyword;
    res += " ";
    res += m_right->stringify();
    res += ")";

    return res;
}

PrefixExpression::PrefixExpression(Token prefix, AstNodePtr right) {
    m_prefix = prefix;
    m_right = right;
}

Token PrefixExpression::prefix() { return m_prefix; }

AstNodePtr PrefixExpression::right() { return m_right; }

AstKind PrefixExpression::type() { return KAstPrefixExpr; }

std::string PrefixExpression::stringify() {
    std::string res = "(" + m_prefix.keyword;

    res += m_right->stringify();

    res += ")";

    return res;
}

VariableStatement::VariableStatement(AstNodePtr type, AstNodePtr name,
                                     AstNodePtr value) {
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr VariableStatement::varType() { return m_type; }

AstNodePtr VariableStatement::name() { return m_name; }

AstNodePtr VariableStatement::value() { return m_value; }

AstKind VariableStatement::type() { return KAstVariableStmt; }

std::string VariableStatement::stringify() {
    std::string res = "";

    if (m_type->type() != KAstNoLiteral) {
        res += m_type->stringify();
        res += " ";
    }

    res += m_name->stringify();

    if (m_value->type() != KAstNoLiteral) {
        res += " = ";
        res += m_value->stringify();
    }

    return res;
}

ConstDeclaration::ConstDeclaration(AstNodePtr type, AstNodePtr name,
                                   AstNodePtr value) {
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr ConstDeclaration::constType() { return m_type; }

AstNodePtr ConstDeclaration::name() { return m_name; }

AstNodePtr ConstDeclaration::value() { return m_value; }

AstKind ConstDeclaration::type() { return KAstConstDecl; }

std::string ConstDeclaration::stringify() {
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

std::vector<AstNodePtr> BlockStatement::statements() { return m_statements; }

AstKind BlockStatement::type() { return KAstBlockStmt; }

std::string BlockStatement::stringify() {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt->stringify();
        res += "\n";
    }

    return res;
}

FunctionDefinition::FunctionDefinition(AstNodePtr returnType, AstNodePtr name,
                                       std::vector<parameter> parameters,
                                       AstNodePtr body) {
    m_returnType = returnType;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
}

AstNodePtr FunctionDefinition::returnType() { return m_returnType; }

AstNodePtr FunctionDefinition::name() { return m_name; }

std::vector<parameter> FunctionDefinition::parameters() { return m_parameters; }

AstNodePtr FunctionDefinition::body() { return m_body; }

AstKind FunctionDefinition::type() { return KAstFunctionDef; }

std::string FunctionDefinition::stringify() {
    std::string res = "def ";

    res += m_name->stringify();
    res += "(";

    if (!m_parameters.empty()) {
        for (size_t i = 0; i < m_parameters.size(); i++) {
            parameter param = m_parameters[i];

            if (i) {
                res += ", ";
            }

            res += param.p_type->stringify();
            res += " ";
            res += param.p_name->stringify();
        }
    }

    res += ") -> ";
    res += m_returnType->stringify();
    res += ":\n";

    res += m_body->stringify();

    return res;
}

ReturnStatement::ReturnStatement(AstNodePtr returnValue) {
    m_returnValue = returnValue;
}

AstNodePtr ReturnStatement::returnValue() { return m_returnValue; }

AstKind ReturnStatement::type() { return KAstReturnStatement; }

std::string ReturnStatement::stringify() {
    std::string res = "return";

    if (m_returnValue->type() != KAstNoLiteral) {
        res += " " + m_returnValue->stringify();
    }

    return res;
}

FunctionCall::FunctionCall(AstNodePtr name, std::vector<AstNodePtr> arguments) {
    m_name = name;
    m_arguments = arguments;
}

AstNodePtr FunctionCall::name() { return m_name; }

std::vector<AstNodePtr> FunctionCall::arguments() { return m_arguments; }

AstKind FunctionCall::type() { return KAstFunctionCall; }

std::string FunctionCall::stringify() {
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

IfStatement::IfStatement(AstNodePtr condition, AstNodePtr ifBody,
                         AstNodePtr elseBody,
                         std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs) {
    m_condition = condition;
    m_ifBody = ifBody;
    m_elseBody = elseBody;
    m_elifs = elifs;
}

AstNodePtr IfStatement::condition() { return m_condition; }

AstNodePtr IfStatement::ifBody() { return m_ifBody; }

std::vector<std::pair<AstNodePtr, AstNodePtr>> IfStatement::elifs() {
    return m_elifs;
}

AstNodePtr IfStatement::elseBody() { return m_elseBody; }

AstKind IfStatement::type() { return KAstIfStmt; }

std::string IfStatement::stringify() {
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

WhileStatement::WhileStatement(AstNodePtr condition, AstNodePtr body) {
    m_condition = condition;
    m_body = body;
}

AstNodePtr WhileStatement::condition() { return m_condition; }

AstNodePtr WhileStatement::body() { return m_body; }

AstKind WhileStatement::type() { return KAstWhileStmt; }

std::string WhileStatement::stringify() {
    std::string res = "while ";

    res += m_condition->stringify();
    res += ":\n";

    res += m_body->stringify();
    res += "\n";

    return res;
}

ForStatement::ForStatement(AstNodePtr variable, AstNodePtr sequence,
                           AstNodePtr body) {
    m_variable = variable;
    m_sequence = sequence;
    m_body = body;
}

AstNodePtr ForStatement::variable() { return m_variable; }

AstNodePtr ForStatement::sequence() { return m_sequence; }

AstNodePtr ForStatement::body() { return m_body; }

AstKind ForStatement::type() { return KAstForStatement; }

std::string ForStatement::stringify() {
    std::string res = "for ";

    res += m_variable->stringify();
    res += " in ";
    res += m_sequence->stringify();
    res += ":\n";

    res += m_body->stringify();
    res += "\n";

    return res;
}

AstKind BreakStatement::type() { return KAstBreakStatement; }

std::string BreakStatement::stringify() { return "break"; }

AstKind PassStatement::type() { return KAstPassStatement; }

std::string PassStatement::stringify() { return "pass"; }

AstKind ContinueStatement::type() { return KAstContinueStatement; }

std::string ContinueStatement::stringify() { return "continue"; }

CppStatement::CppStatement(std::string cpp_code) { m_cppCode = cpp_code; }

std::string CppStatement::value() { return m_cppCode; }

AstKind CppStatement::type() { return KAstCpp; }

std::string CppStatement::stringify() {
    std::string res = "Cppcode";

    res += "(" + m_cppCode + ")";

    return res;
}

ScopeStatement::ScopeStatement(AstNodePtr body) { m_scopeBody = body; }

AstNodePtr ScopeStatement::body() { return m_scopeBody; }

AstKind ScopeStatement::type() { return KAstScopeStmt; }

std::string ScopeStatement::stringify() {
    std::string res = "scope:\n ";
    res += m_scopeBody->stringify();
    res += "\n";
    return res;
}
