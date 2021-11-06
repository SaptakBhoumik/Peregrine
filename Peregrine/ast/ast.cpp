#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <string>
#include <vector>

Program::Program(std::vector<AstNode> statements) { m_statements = statements; }

AstKind Program::type() { return KAstProgram; }

std::string Program::stringify() {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt.stringify();
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

StringLiteral::StringLiteral(std::string_view value, bool formatted) { 
    m_value = value; 
    m_formatted = formatted;
}

std::string StringLiteral::value() { return m_value; }

bool StringLiteral::formatted() { return m_formatted; }

AstKind StringLiteral::type() { return KAstString; }

std::string StringLiteral::stringify() { return m_value; }

BoolLiteral::BoolLiteral(std::string_view value) { m_value = value; }

std::string BoolLiteral::value() { return m_value; }

AstKind BoolLiteral::type() { return KAstBool; }

std::string BoolLiteral::stringify() { return m_value; }

NoneLiteral::NoneLiteral() {}

AstKind NoneLiteral::type() { return KAstNone; }

std::string NoneLiteral::stringify() { return "None"; }

ListLiteral::ListLiteral(AstNode type, std::vector<AstNode> elements) {
    m_type = type;
    m_elements = elements;
}

AstNode ListLiteral::list_type() { return m_type; }

std::vector<AstNode> ListLiteral::elements() { return m_elements; }

AstKind ListLiteral::type() { return KAstList; }

std::string ListLiteral::stringify() {
    std::string res = "";

    for (auto& elem : m_elements) {
        res += elem.stringify();
    }

    return res;
}

BinaryOperation::BinaryOperation(AstNode left, std::string_view operator,
                                 AstNode right) {
    m_left = left;
    m_operator = op;
    m_right = right;
}

AstNode BinaryOperation::left() { return m_left; }

std::string BinaryOperation::op() { return m_operator; }

AstNode BinaryOperation::right() { return m_right; }

AstKind BinaryOperation::type() { return KAstBinaryOp; }

// TODO: this can surely be better
std::string BinaryOperation::stringify() {
    std::string res = "(";

    res += m_left.stringify() res += " ";
    res += m_operator;
    res += " ";
    res += m_right.stringify();
    res += ")";

    return res;
}

PrefixExpression::PrefixExpression(std::string_view prefix, AstNode right) {
    m_prefix = prefix;
    m_right = right;
}

std::string PrefixExpression::prefix() { return m_prefix; }

AstNode PrefixExpression::right() { return m_right; }

AstKind PrefixExpression::type() { return KAstPrefixExpr; }

std::string PrefixExpression::stringify() {
    std::string res = m_prefix;

    res += m_right.stringify();

    return res;
}

VariableStatement::VariableStatement(AstNode type = NoneLiteral(), AstNode name,
                                     AstNode value = NoneLiteral()) {
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNode VariableStatement::var_type() { return m_type; }

AstNode VariableStatement::name() { return m_name; }

AstNode VariableStatement::value() { return m_value; }

AstKind VariableStatement::type() { return KAstVariableStmt; }

std::string VariableStatement::stringify() {
    std::string res = "";

    if (m_type.type() != KAstNone) {
        res += m_type.stringify();
        res += " ";
    }

    res += m_name.stringify();

    if (m_value.type() != KAstNone) {
        res += " = ";
        res += m_value.stringify();
    }

    return res;
}

ConstDeclaration::ConstDeclaration(AstNode type = NoneLiteral(), AstNode name,
                                   AstNode value) {
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNode ConstDeclaration::const_type() { return m_type; }

AstNode ConstDeclaration::name() { return m_name; }

AstNode ConstDeclaration::value() { return m_value; }

AstKind ConstDeclaration::type() { return KAstConstDecl; }

std::string ConstDeclaration::stringify() {
    std::string res = "const ";

    if (m_type.type() != KAstNone) {
        res += m_type.stringify();
        res += " ";
    }

    res += m_name.stringify();

    res += " = ";
    res += m_value.stringify();

    return res;
}

BlockStatement::BlockStatement(std::vector<AstNode> statements) {
    m_statements = statements;
}

std::vector<AstNode> BlockStatement::statements() { return m_statements; }

AstKind BlockStatement::type() { return KAstBlockStmt; }

std::string BlockStatement::stringify() {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt.stringify();
        res += "\n";
    }

    return res;
}

FunctionDefinition::FunctionDefinition(
    AstNode return_type, AstNode name,
    std::vector<parameter> parameters = std::vector<parameter>(),
    AstNode body) {
    m_return_type = return_type;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
}

AstNode FunctionDefinition::return_type() { return m_return_type; }

AstNode FunctionDefinition::name() { return m_name; }

std::vector<parameter> FunctionDefinition::parameters() { return m_parameters; }

AstNode FunctionDefinition::body() { return m_body; }

AstKind FunctionDefinition::type() { return KAstFunctionDef; }

std::string FunctionDefinition::stringify() {
    std::string res = "def ";

    res += m_name;
    res += "(";

    if (!m_parameters.empty()) {
        for (size_t i = 0; i < m_parameters.size(); i++) {
            parameter param = m_parameters[i];

            if (i) {
                res += ", ";
            }

            res += param.p_type.stringify();
            res += " ";
            res += param.p_name.stringify();
        }
    }

    res += ") -> ";
    res += m_return_type.stringify();
    res += ":\n";

    res += m_body.stringify();

    return res;
}

FunctionCall::FunctionCall(
    AstNode name, std::vector<AstNode> arguments = std::vector<AstNode>()) {
    m_name = name;
    m_arguments = arguments;
}

AstNode FunctionCall::name() { return m_name; }

std::vector<AstNode> FunctionCall::arguments() { return m_arguments; }

AstKind FunctionCall::type() { return KAstFunctionCall; }

std::string FunctionCall::stringify() {
    std::string res = "";

    res += m_name.stringify();
    res += "(";

    for (size_t i = 0; i < m_arguments.size(); i++) {
        if (i)
            res += ", ";

        res += m_arguments[i].stringify();
    }

    res += ")";

    return res;
}

IfStatement::IfStatement(AstNode condition, AstNode if_body,
                         AstNode else_body = NoneLiteral(),
                         std::vector<std::pair<AstNode, AstNode>> elifs =
                             std::vector<std::pair<AstNode, AstNode>>()) {
    m_condition = condition;
    m_if_body = if_body;
    m_else_body = else_body;
    m_elifs = elifs;
}

AstNode IfStatement::condition() { return m_condition; }

AstNode IfStatement::if_body() { return m_if_body; }

std::vector<std::pair<AstNode, AstNode>> IfStatement::elifs() {
    return m_elifs;
}

AstNode IfStatement::else_body() { return m_else_body; }

AstKind IfStatement::type() { return KAstIfStmt; }

std::string IfStatement::stringify() {
    std::string res = "if ";

    res += m_condition.stringify();
    res += ":\n";

    res += m_if_body.stringify();
    res += "\n";

    for (auto& elif : m_elifs) {
        res += "elif ";
        res += elif.first.stringify();
        res += ":\n";

        res += elif.second.stringify();
        res += "\n";
    }

    if (m_else_body.type() != KAstNone) {
        res += "else:\n";
        res += m_else_body.stringify();
        res += "\n";
    }

    return res;
}

WhileStatement::WhileStatement(AstNode condition, AstNode body) {
    m_condition = condition;
    m_body = body;
}

AstNode WhileStatement::condition() { return m_condition; }

AstNode WhileStatement::body() { return m_body; }

AstKind WhileStatement::type() { return KAstWhileStmt; }

std::string WhileStatement::stringify() {
    std::string = "while ";

    res += m_condition.stringify();
    res += ":\n";

    res += m_body.stringify();
    res += "\n";

    return res;
}
