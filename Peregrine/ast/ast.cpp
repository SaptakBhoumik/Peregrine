#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace ast {

Program::Program(std::vector<AstNodePtr> statements) {
    m_statements = statements;
}

std::vector<AstNodePtr> Program::statements() { return m_statements; }

Token Program::token() {
    return Token{};
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

IntegerLiteral::IntegerLiteral(Token tok, std::string_view value) { 
    m_token = tok;
    m_value = value;
}

std::string IntegerLiteral::value() { return m_value; }

Token IntegerLiteral::token() {
    return m_token;
}

AstKind IntegerLiteral::type() { return KAstInteger; }

std::string IntegerLiteral::stringify() { return m_value; }

DecimalLiteral::DecimalLiteral(Token tok, std::string_view value) { 
    m_token = tok;
    m_value = value; 
}

std::string DecimalLiteral::value() { return m_value; }

Token DecimalLiteral::token() {
    return m_token;
}

AstKind DecimalLiteral::type() { return KAstDecimal; }

std::string DecimalLiteral::stringify() { return m_value; }

StringLiteral::StringLiteral(Token tok, std::string_view value, bool formatted, bool raw) {
    m_token = tok;
    m_value = value;
    m_formatted = formatted;
    m_raw = raw;
}

std::string StringLiteral::value() { return m_value; }

bool StringLiteral::formatted() { return m_formatted; }

bool StringLiteral::raw() { return m_raw; }

Token StringLiteral::token() {
    return m_token;
}

AstKind StringLiteral::type() { return KAstString; }

std::string StringLiteral::stringify() { return m_value; }

BoolLiteral::BoolLiteral(Token tok, std::string_view value) { 
    m_token = tok;
    m_value = value; 
}

std::string BoolLiteral::value() { return m_value; }

Token BoolLiteral::token() {
    return m_token;
}

AstKind BoolLiteral::type() { return KAstBool; }

std::string BoolLiteral::stringify() { return m_value; }

NoneLiteral::NoneLiteral(Token tok) {
    m_token = tok;
}

Token NoneLiteral::token() {
    return m_token;
}

AstKind NoneLiteral::type() { return KAstNone; }

std::string NoneLiteral::stringify() { return "None"; }

Token NoLiteral::token() {
    return Token{};
} 

AstKind NoLiteral::type() { return KAstNoLiteral; }

std::string NoLiteral::stringify() { return "None"; }

IdentifierExpression::IdentifierExpression(Token tok, std::string_view value) {
    m_token = tok;
    m_value = value;
}

std::string IdentifierExpression::value() { return m_value; }

Token IdentifierExpression::token() {
    return m_token;
}

AstKind IdentifierExpression::type() { return KAstIdentifier; }

std::string IdentifierExpression::stringify() { return m_value; }

TypeExpression::TypeExpression(Token tok, std::string_view value) { 
    m_token = tok;
    m_value = value;
}

std::string TypeExpression::value() { return m_value; }

Token TypeExpression::token() {
    return m_token;
}

AstKind TypeExpression::type() { return KAstTypeExpr; }

std::string TypeExpression::stringify() { return m_value; }

ListLiteral::ListLiteral(Token tok, AstNodePtr type, std::vector<AstNodePtr> elements) {
    m_token = tok;
    m_type = type;
    m_elements = elements;
}

AstNodePtr ListLiteral::listType() { return m_type; }

std::vector<AstNodePtr> ListLiteral::elements() { return m_elements; }

Token ListLiteral::token() {
    return m_token;
}

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

DictLiteral::DictLiteral(Token tok,
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements) {
    m_token = tok;
    m_elements = elements;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>> DictLiteral::elements() {
    return m_elements;
}

Token DictLiteral::token() {
    return m_token;
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

BinaryOperation::BinaryOperation(Token tok, AstNodePtr left, Token op, AstNodePtr right) {
    m_token = tok;
    m_left = left;
    m_operator = op;
    m_right = right;
}

AstNodePtr BinaryOperation::left() { return m_left; }

Token BinaryOperation::op() { return m_operator; }

AstNodePtr BinaryOperation::right() { return m_right; }

Token BinaryOperation::token() {
    return m_token;
}

AstKind BinaryOperation::type() { return KAstBinaryOp; }

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

PrefixExpression::PrefixExpression(Token tok, Token prefix, AstNodePtr right) {
    m_token = tok;
    m_prefix = prefix;
    m_right = right;
}

Token PrefixExpression::prefix() { return m_prefix; }

AstNodePtr PrefixExpression::right() { return m_right; }

Token PrefixExpression::token() {
    return m_token;
}

AstKind PrefixExpression::type() { return KAstPrefixExpr; }

std::string PrefixExpression::stringify() {
    std::string res = "(" + m_prefix.keyword;

    res += m_right->stringify();

    res += ")";

    return res;
}

ListOrDictAccess::ListOrDictAccess(Token tok, AstNodePtr container, AstNodePtr keyOrIndex) {
    m_token = tok;
    m_container = container;
    m_keyOrIndex = keyOrIndex;
}

AstNodePtr ListOrDictAccess::container() {
    return m_container;
}

AstNodePtr ListOrDictAccess::keyOrIndex() {
    return m_keyOrIndex;
}

Token ListOrDictAccess::token() {
    return m_token;
}

AstKind ListOrDictAccess::type() {
    return KAstListOrDictAccess;
}

std::string ListOrDictAccess::stringify() {
    std::string res = "";

    res += m_container->stringify() + "[";
    res += m_keyOrIndex->stringify() + "]";
    
    return res;
}

ImportStatement::ImportStatement(Token tok,
        std::pair<AstNodePtr, AstNodePtr> moduleName,
        std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols) {
            m_token = tok;
            m_moduleName = moduleName;
            m_importedSymbols = importedSymbols;
        }

std::pair<AstNodePtr, AstNodePtr> ImportStatement::moduleName() {
    return m_moduleName;
}

std::vector<std::pair<AstNodePtr, AstNodePtr>> ImportStatement::importedSymbols() {
    return m_importedSymbols;
}

Token ImportStatement::token() {
    return m_token;
}

AstKind ImportStatement::type() {
    return KAstImportStmt;
}

std::string ImportStatement::stringify() {
    std::string res = "";

    res += (!m_importedSymbols.size()) ? "import " : "from ";

    res += m_moduleName.first->stringify();
    //hmm
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

VariableStatement::VariableStatement(Token tok, AstNodePtr type, AstNodePtr name,
                                     AstNodePtr value) {
    m_token = tok;
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr VariableStatement::varType() { return m_type; }

AstNodePtr VariableStatement::name() { return m_name; }

AstNodePtr VariableStatement::value() { return m_value; }

Token VariableStatement::token() {
    return m_token;
}

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

ConstDeclaration::ConstDeclaration(Token tok, AstNodePtr type, AstNodePtr name,
                                   AstNodePtr value) {
    m_token = tok;
    m_type = type;
    m_name = name;
    m_value = value;
}

AstNodePtr ConstDeclaration::constType() { return m_type; }

AstNodePtr ConstDeclaration::name() { return m_name; }

AstNodePtr ConstDeclaration::value() { return m_value; }

Token ConstDeclaration::token() {
    return m_token;
}

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

Token BlockStatement::token() {
    return Token{};
}

AstKind BlockStatement::type() { return KAstBlockStmt; }

std::string BlockStatement::stringify() {
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt->stringify();
        res += "\n";
    }

    return res;
}

FunctionDefinition::FunctionDefinition(Token tok, AstNodePtr returnType, AstNodePtr name,
                                       std::vector<parameter> parameters,
                                       AstNodePtr body) {
    m_token = tok;
    m_returnType = returnType;
    m_name = name;
    m_parameters = parameters;
    m_body = body;
}

AstNodePtr FunctionDefinition::returnType() { return m_returnType; }

AstNodePtr FunctionDefinition::name() { return m_name; }

std::vector<parameter> FunctionDefinition::parameters() { return m_parameters; }

AstNodePtr FunctionDefinition::body() { return m_body; }

Token FunctionDefinition::token() {
    return m_token;
}

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

ReturnStatement::ReturnStatement(Token tok, AstNodePtr returnValue) {
    m_token = tok;
    m_returnValue = returnValue;
}

AstNodePtr ReturnStatement::returnValue() { return m_returnValue; }

Token ReturnStatement::token() {
    return m_token;
}

AstKind ReturnStatement::type() { return KAstReturnStatement; }

std::string ReturnStatement::stringify() {
    std::string res = "return";

    if (m_returnValue->type() != KAstNoLiteral) {
        res += " " + m_returnValue->stringify();
    }

    return res;
}

FunctionCall::FunctionCall(Token tok, AstNodePtr name, std::vector<AstNodePtr> arguments) {
    m_token = tok;
    m_name = name;
    m_arguments = arguments;
}

AstNodePtr FunctionCall::name() { return m_name; }

std::vector<AstNodePtr> FunctionCall::arguments() { return m_arguments; }

Token FunctionCall::token() {
    return m_token;
}

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

DotExpression::DotExpression(Token tok, AstNodePtr owner, AstNodePtr referenced) {
    m_token = tok;
    m_owner = owner;
    m_referenced = referenced;
}

AstNodePtr DotExpression::owner() {
    return m_owner;
}

AstNodePtr DotExpression::referenced() {
    return m_referenced;
}

Token DotExpression::token() {
    return m_token;
}

AstKind DotExpression::type() {
    return KAstDotExpression;
}

std::string DotExpression::stringify() {
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

AstNodePtr IfStatement::condition() { return m_condition; }

AstNodePtr IfStatement::ifBody() { return m_ifBody; }

std::vector<std::pair<AstNodePtr, AstNodePtr>> IfStatement::elifs() {
    return m_elifs;
}

AstNodePtr IfStatement::elseBody() { return m_elseBody; }

Token IfStatement::token() {
    return m_token;
}

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

WhileStatement::WhileStatement(Token tok, AstNodePtr condition, AstNodePtr body) {
    m_token = tok;
    m_condition = condition;
    m_body = body;
}

AstNodePtr WhileStatement::condition() { return m_condition; }

AstNodePtr WhileStatement::body() { return m_body; }

Token WhileStatement::token() {
    return m_token;
}

AstKind WhileStatement::type() { return KAstWhileStmt; }

std::string WhileStatement::stringify() {
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

AstNodePtr ForStatement::variable() { return m_variable; }

AstNodePtr ForStatement::sequence() { return m_sequence; }

AstNodePtr ForStatement::body() { return m_body; }

Token ForStatement::token() {
    return m_token;
}

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

BreakStatement::BreakStatement(Token tok) {
    m_token = tok;
}

Token BreakStatement::token() {
    return m_token;
}

AstKind BreakStatement::type() { return KAstBreakStatement; }

std::string BreakStatement::stringify() { return "break"; }

PassStatement::PassStatement(Token tok) {
    m_token = tok;
}

Token PassStatement::token() {
    return m_token;
}

AstKind PassStatement::type() { return KAstPassStatement; }

std::string PassStatement::stringify() { return "pass"; }

ContinueStatement::ContinueStatement(Token tok) {
    m_token = tok;
}

Token ContinueStatement::token() {
    return m_token;
}

AstKind ContinueStatement::type() { return KAstContinueStatement; }

std::string ContinueStatement::stringify() { return "continue"; }

CppStatement::CppStatement(Token tok, std::string cpp_code) { 
    m_token = tok;
    m_cppCode = cpp_code;
}

std::string CppStatement::value() { return m_cppCode; }

Token CppStatement::token() {
    return m_token;
}

AstKind CppStatement::type() { return KAstCpp; }

std::string CppStatement::stringify() {
    std::string res = "Cppcode";

    res += "(" + m_cppCode + ")";

    return res;
}

ScopeStatement::ScopeStatement(Token tok, AstNodePtr body) { 
    m_token = tok;
    m_scopeBody = body; 
}

AstNodePtr ScopeStatement::body() { return m_scopeBody; }

Token ScopeStatement::token() {
    return m_token;
}

AstKind ScopeStatement::type() { return KAstScopeStmt; }

std::string ScopeStatement::stringify() {
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

AstNodePtr TypeDefinition::name() { return m_name; }

AstNodePtr TypeDefinition::baseType() { return m_type; }

Token TypeDefinition::token() {
    return m_token;
}

AstKind TypeDefinition::type() { return KAstTypeDefinition; }

std::string TypeDefinition::stringify() {
    std::string res = "type " + m_name->stringify();

    res += " = ";

    res += m_type->stringify();

    return res;
}

MatchStatement::MatchStatement(Token tok, std::vector<AstNodePtr> to_match,
                std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases,
                AstNodePtr defaultbody) {
    m_token = tok;
    m_toMatch=to_match;
    m_cases=cases;
    m_default=defaultbody;
}

std::vector<AstNodePtr> MatchStatement::matchItem() { return m_toMatch; }

std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> MatchStatement::caseBody() { return m_cases; }

AstNodePtr MatchStatement::defaultBody() {
    return m_default;
}

Token MatchStatement::token() {
    return m_token;
}

AstKind MatchStatement::type() { return KAstMatchStmt; }

std::string MatchStatement::stringify() {
    std::string res = "match ";
    for(auto& temp:m_toMatch){
        res+=temp->stringify()+",";
    }
    res += ":\n";
    for (auto& elif : m_cases) {
        res += "case ";
        auto temp= elif.first;
        for (auto& x : temp){
            res+=x->stringify()+",";
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

}