#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace ast {

Program::Program(std::vector<AstNodePtr> statements) {
    m_statements = statements;
}

std::vector<AstNodePtr> Program::statements() const { return m_statements; }

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
                                   AstNodePtr keyOrIndex) {
    m_token = tok;
    m_container = container;
    m_keyOrIndex = keyOrIndex;
}

AstNodePtr ListOrDictAccess::container() const { return m_container; }

AstNodePtr ListOrDictAccess::keyOrIndex() const { return m_keyOrIndex; }

Token ListOrDictAccess::token() const { return m_token; }

AstKind ListOrDictAccess::type() const { return KAstListOrDictAccess; }

std::string ListOrDictAccess::stringify() const {
    std::string res = "";

    res += m_container->stringify() + "[";
    res += m_keyOrIndex->stringify() + "]";

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

Token VariableStatement::token() const { return m_token; }

AstKind VariableStatement::type() const { return KAstVariableStmt; }

std::string VariableStatement::stringify() const {
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

FunctionDefinition::FunctionDefinition(Token tok, AstNodePtr returnType,
                                       AstNodePtr name,
                                       std::vector<parameter> parameters,
                                       AstNodePtr body) {
    m_token = tok;
    m_returnType = returnType;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
}

AstNodePtr FunctionDefinition::returnType() const { return m_returnType; }

AstNodePtr FunctionDefinition::name() const { return m_name; }

std::vector<parameter> FunctionDefinition::parameters() const {
    return m_parameters;
}

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

ForStatement::ForStatement(Token tok, AstNodePtr variable, AstNodePtr sequence,
                           AstNodePtr body) {
    m_token = tok;
    m_variable = variable;
    m_sequence = sequence;
    m_body = body;
}

AstNodePtr ForStatement::variable() const { return m_variable; }

AstNodePtr ForStatement::sequence() const { return m_sequence; }

AstNodePtr ForStatement::body() const { return m_body; }

Token ForStatement::token() const { return m_token; }

AstKind ForStatement::type() const { return KAstForStatement; }

std::string ForStatement::stringify() const {
    std::string res = "for ";

    res += m_variable->stringify();
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
    Token tok, std::vector<AstNodePtr> to_match,
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases,
    AstNodePtr defaultbody) {
    m_token = tok;
    m_toMatch = to_match;
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

ListTypeExpr::ListTypeExpr(Token tok, AstNodePtr baseType) {
    m_token = tok;
    m_baseType = baseType;
}

AstNodePtr ListTypeExpr::baseType() const { return m_baseType; }

Token ListTypeExpr::token() const { return m_token; }

AstKind ListTypeExpr::type() const { return KAstListTypeExpr; }

std::string ListTypeExpr::stringify() const {
    std::string res = "[]";
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

} // namespace ast