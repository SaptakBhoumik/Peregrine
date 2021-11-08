#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <string>
#include <vector>

Program::Program(std::vector<AstNodePtr> statements) {
    m_statements = statements;
}

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

StringLiteral::StringLiteral(std::string_view value, bool formatted,bool raw) {
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

IdentifierExpression::IdentifierExpression(std::string_view value) {
    m_value = value;
}

std::string IdentifierExpression::value() { return m_value; }

AstKind IdentifierExpression::type() { return KAstIdentifier; }

std::string IdentifierExpression::stringify() { return m_value; }

ListLiteral::ListLiteral(AstNodePtr type, std::vector<AstNodePtr> elements) {
    m_type = type;
    m_elements = elements;
}

AstNodePtr ListLiteral::list_type() { return m_type; }

std::vector<AstNodePtr> ListLiteral::elements() { return m_elements; }

AstKind ListLiteral::type() { return KAstList; }

std::string ListLiteral::stringify() {
    std::string res = "";

    for (auto& elem : m_elements) {
        res += elem->stringify();
    }

    return res;
}

BinaryOperation::BinaryOperation(AstNodePtr left, std::string_view op,
                                 AstNodePtr right) {
    m_left = left;
    m_operator = op;
    m_right = right;
}

AstNodePtr BinaryOperation::left() { return m_left; }

std::string BinaryOperation::op() { return m_operator; }

AstNodePtr BinaryOperation::right() { return m_right; }

AstKind BinaryOperation::type() { return KAstBinaryOp; }

// TODO: this can surely be better
std::string BinaryOperation::stringify() {
    std::string res = "(";

    res += m_left->stringify();
    res += " ";
    res += m_operator;
    res += " ";
    res += m_right->stringify();
    res += ")";

    return res;
}

PrefixExpression::PrefixExpression(std::string_view prefix, AstNodePtr right) {
    m_prefix = prefix;
    m_right = right;
}

std::string PrefixExpression::prefix() { return m_prefix; }

AstNodePtr PrefixExpression::right() { return m_right; }

AstKind PrefixExpression::type() { return KAstPrefixExpr; }

std::string PrefixExpression::stringify() {
    std::string res = m_prefix;

    res += m_right->stringify();

    return res;
}

VariableStatement::VariableStatement(AstNodePtr type, AstNodePtr name,
                                     AstNodePtr value) {
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr VariableStatement::var_type() { return m_type; }

AstNodePtr VariableStatement::name() { return m_name; }

AstNodePtr VariableStatement::value() { return m_value; }

AstKind VariableStatement::type() { return KAstVariableStmt; }

std::string VariableStatement::stringify() {
    std::string res = "";

    if (m_type->type() != KAstNone) {
        res += m_type->stringify();
        res += " ";
    }

    res += m_name->stringify();

    if (m_value->type() != KAstNone) {
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

AstNodePtr ConstDeclaration::const_type() { return m_type; }

AstNodePtr ConstDeclaration::name() { return m_name; }

AstNodePtr ConstDeclaration::value() { return m_value; }

AstKind ConstDeclaration::type() { return KAstConstDecl; }

std::string ConstDeclaration::stringify() {
    std::string res = "const ";

    if (m_type->type() != KAstNone) {
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

FunctionDefinition::FunctionDefinition(AstNodePtr return_type, AstNodePtr name,
                                       std::vector<parameter> parameters,
                                       AstNodePtr body) {
    m_return_type = return_type;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
}

AstNodePtr FunctionDefinition::return_type() { return m_return_type; }

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
    res += m_return_type->stringify();
    res += ":\n";

    res += m_body->stringify();

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

IfStatement::IfStatement(AstNodePtr condition, AstNodePtr if_body,
                         AstNodePtr else_body,
                         std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs) {
    m_condition = condition;
    m_if_body = if_body;
    m_else_body = else_body;
    m_elifs = elifs;
}

AstNodePtr IfStatement::condition() { return m_condition; }

AstNodePtr IfStatement::if_body() { return m_if_body; }

std::vector<std::pair<AstNodePtr, AstNodePtr>> IfStatement::elifs() {
    return m_elifs;
}

AstNodePtr IfStatement::else_body() { return m_else_body; }

AstKind IfStatement::type() { return KAstIfStmt; }

std::string IfStatement::stringify() {
    std::string res = "if ";

    res += m_condition->stringify();
    res += ":\n";

    res += m_if_body->stringify();
    res += "\n";

    for (auto& elif : m_elifs) {
        res += "elif ";
        res += elif.first->stringify();
        res += ":\n";

        res += elif.second->stringify();
        res += "\n";
    }

    if (m_else_body->type() != KAstNone) {
        res += "else:\n";
        res += m_else_body->stringify();
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
