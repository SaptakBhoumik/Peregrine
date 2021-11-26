#ifndef PEREGRINE_AST_HPP
#define PEREGRINE_AST_HPP

#include "lexer/tokens.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace ast {

enum AstKind {
    KAstNoLiteral,
    KAstProgram,
    KAstInteger,
    KAstDecimal,
    KAstString,
    KAstBool,
    KAstNone,
    KAstIdentifier,
    KAstTypeExpr,
    KAstListTypeExpr,
    KAstDictTypeExpr,
    KAstFuncTypeExpr,
    KAstList,
    KAstDict,
    KAstCpp,
    KAstBinaryOp,
    KAstPrefixExpr,
    KAstListOrDictAccess,
    KAstImportStmt,
    KAstVariableStmt,
    KAstConstDecl,
    KAstBlockStmt,
    KAstFunctionDef,
    KAstReturnStatement,
    KAstFunctionCall,
    KAstDotExpression,
    KAstIfStmt,
    KAstMatchStmt,
    KAstScopeStmt,
    KAstWhileStmt,
    KAstForStatement,
    KAstBreakStatement,
    KAstContinueStatement,
    KAstTypeDefinition,
    KAstPassStatement
};

class AstNode {
  public:
    virtual Token token() = 0;
    virtual AstKind type() = 0;
    virtual std::string stringify() = 0;
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class Program : public AstNode {
    std::vector<AstNodePtr> m_statements;

  public:
    Program(std::vector<AstNodePtr> statements);

    std::vector<AstNodePtr> statements();

    Token token();
    AstKind type();
    std::string stringify();
};

class IntegerLiteral : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    IntegerLiteral(Token tok, std::string_view value);

    std::string value();

    Token token();
    AstKind type();
    std::string stringify();
};

class DecimalLiteral : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    DecimalLiteral(Token tok, std::string_view value);

    std::string value();

    Token token();
    AstKind type();
    std::string stringify();
};

class StringLiteral : public AstNode {
    Token m_token;
    std::string m_value;
    bool m_formatted;
    bool m_raw;

  public:
    StringLiteral(Token tok, std::string_view value, bool formatted, bool raw);

    std::string value();
    bool formatted();
    bool raw();

    Token token();
    AstKind type();
    std::string stringify();
};

class BoolLiteral : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    BoolLiteral(Token tok, std::string_view value);

    std::string value();

    Token token();
    AstKind type();
    std::string stringify();
};

class NoneLiteral : public AstNode {
    Token m_token;
  public:
    NoneLiteral(Token tok);

    Token token();
    AstKind type();
    std::string stringify();
};

class NoLiteral : public AstNode {
  public:
    NoLiteral() = default;

    Token token();
    AstKind type();
    std::string stringify();
};

class IdentifierExpression : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    IdentifierExpression(Token tok, std::string_view value);

    std::string value();

    Token token();
    AstKind type();
    std::string stringify();
};

// ------------ TYPES ------------

class TypeExpression : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    TypeExpression(Token tok, std::string_view value);

    std::string value();

    Token token();
    AstKind type();
    std::string stringify();
};

class ListTypeExpr : public AstNode {
    Token m_token;
    AstNodePtr m_baseType;

  public:
    ListTypeExpr(Token tok, AstNodePtr baseType);

    AstNodePtr baseType();

    Token token();
    AstKind type();
    std::string stringify();
};

class DictTypeExpr : public AstNode {
    Token m_token;
    AstNodePtr m_keyType;
    AstNodePtr m_valueType;

  public:
    DictTypeExpr(Token tok, AstNodePtr keyType, AstNodePtr valueType);

    AstNodePtr keyType();
    AstNodePtr valueType();

    Token token();
    AstKind type();
    std::string stringify();
};

class FunctionTypeExpr : public AstNode {
    Token m_token;
    std::vector<AstNodePtr> m_argTypes;
    std::vector<AstNodePtr> m_returnTypes;

  public:
    FunctionTypeExpr(Token tok, std::vector<AstNodePtr> argTypes,std::vector<AstNodePtr> returnTypes);
    std::vector<AstNodePtr> argTypes();
    std::vector<AstNodePtr> returnTypes();

    Token token();
    AstKind type();
    std::string stringify();
};

// ------------------------------

class ListLiteral : public AstNode {
    Token m_token;
    AstNodePtr m_type;
    std::vector<AstNodePtr> m_elements;

  public:
    ListLiteral(Token tok, AstNodePtr type, std::vector<AstNodePtr> elements);

    AstNodePtr listType();
    std::vector<AstNodePtr> elements();

    Token token();
    AstKind type();
    std::string stringify();
};

class DictLiteral : public AstNode {
    Token m_token;
    // first is the key, second is the value
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elements;

  public:
    DictLiteral(Token tok, std::vector<std::pair<AstNodePtr, AstNodePtr>> elements);

    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements();
    
    Token token();
    AstKind type();
    std::string stringify();
};

class BinaryOperation : public AstNode {
    Token m_token;
    AstNodePtr m_left;
    Token m_operator;
    AstNodePtr m_right;

  public:
    BinaryOperation(Token tok, AstNodePtr left, Token op, AstNodePtr right);

    AstNodePtr left();
    Token op();
    AstNodePtr right();

    Token token();
    AstKind type();
    std::string stringify();
};

class PrefixExpression : public AstNode {
    Token m_token;
    Token m_prefix;
    AstNodePtr m_right;

  public:
    PrefixExpression(Token tok, Token prefix, AstNodePtr right);

    Token prefix();
    AstNodePtr right();

    Token token();
    AstKind type();
    std::string stringify();
};

class ListOrDictAccess : public AstNode {
    Token m_token;
    AstNodePtr m_container;
    AstNodePtr m_keyOrIndex;

  public:
    ListOrDictAccess(Token tok, AstNodePtr container, AstNodePtr keyOrIndex);

    AstNodePtr container();
    AstNodePtr keyOrIndex();

    Token token();
    AstKind type();
    std::string stringify();
};

class ImportStatement : public AstNode {
    Token m_token;
    // first is the module name and the second is the alias
    std::pair<AstNodePtr, AstNodePtr> m_moduleName;

    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_importedSymbols;

  public:
    ImportStatement(Token tok,
        std::pair<AstNodePtr, AstNodePtr> moduleName,
        std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols);

    std::pair<AstNodePtr, AstNodePtr> moduleName();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols();

    Token token();
    AstKind type();
    std::string stringify();
};

// variable declaration, assignment and reassignment
class VariableStatement : public AstNode {
    Token m_token;
    AstNodePtr m_type;
    AstNodePtr m_name;
    AstNodePtr m_value;

  public:
    VariableStatement(Token tok, AstNodePtr type, AstNodePtr name, AstNodePtr value);

    AstNodePtr varType();
    AstNodePtr name();
    AstNodePtr value();

    Token token();
    AstKind type();
    std::string stringify();
};

class ConstDeclaration : public AstNode {
    Token m_token;
    AstNodePtr m_type;
    AstNodePtr m_name;
    AstNodePtr m_value;

  public:
    ConstDeclaration(Token tok, AstNodePtr type, AstNodePtr name, AstNodePtr value);

    AstNodePtr constType();
    AstNodePtr name();
    AstNodePtr value();

    Token token();
    AstKind type();
    std::string stringify();
};

class BlockStatement : public AstNode {
    std::vector<AstNodePtr> m_statements;

  public:
    BlockStatement(std::vector<AstNodePtr> statements);

    std::vector<AstNodePtr> statements();

    Token token();
    AstKind type();
    std::string stringify();
};

struct parameter {
    AstNodePtr p_type;
    AstNodePtr p_name;
};

class FunctionDefinition : public AstNode {
    Token m_token;
    AstNodePtr m_returnType;
    AstNodePtr m_name;

    std::vector<parameter> m_parameters;

    AstNodePtr m_body;

  public:
    FunctionDefinition(Token tok, AstNodePtr returnType, AstNodePtr name,
                       std::vector<parameter> parameters, AstNodePtr body);

    AstNodePtr returnType();
    AstNodePtr name();
    std::vector<parameter> parameters();
    AstNodePtr body();

    Token token();
    AstKind type();
    std::string stringify();
};

class ReturnStatement : public AstNode {
    Token m_token;
    AstNodePtr m_returnValue;

  public:
    ReturnStatement(Token tok, AstNodePtr returnValue);

    AstNodePtr returnValue();

    Token token();
    AstKind type();
    std::string stringify();
};

class FunctionCall : public AstNode {
    Token m_token;
    AstNodePtr m_name;
    std::vector<AstNodePtr> m_arguments;

  public:
    FunctionCall(Token tok, AstNodePtr name, std::vector<AstNodePtr> arguments);

    AstNodePtr name();
    std::vector<AstNodePtr> arguments();

    Token token();
    AstKind type();
    std::string stringify();
};

// test.hello(), obj.prop, etc
class DotExpression : public AstNode {
    Token m_token;
    AstNodePtr m_owner; // the node that comes before the .
    AstNodePtr m_referenced; // the node that comes after the .

  public:
    DotExpression(Token tok, AstNodePtr owner, AstNodePtr referenced);

    AstNodePtr owner();
    AstNodePtr referenced();

    Token token();
    AstKind type();
    std::string stringify();
};

class IfStatement : public AstNode {
    Token m_token;
    AstNodePtr m_condition;
    AstNodePtr m_ifBody;

    // first item in the pair is the condition, and the second is the block to
    // be executed
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elifs;

    AstNodePtr m_elseBody;

  public:
    IfStatement(Token tok, AstNodePtr condition, AstNodePtr ifBody, AstNodePtr elseBody,
                std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs);

    AstNodePtr condition();
    AstNodePtr ifBody();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs();
    AstNodePtr elseBody();

    Token token();
    AstKind type();
    std::string stringify();
};

class WhileStatement : public AstNode {
    Token m_token;
    AstNodePtr m_condition;
    AstNodePtr m_body;

  public:
    WhileStatement(Token tok, AstNodePtr condition, AstNodePtr body);

    AstNodePtr condition();
    AstNodePtr body();

    Token token();
    AstKind type();
    std::string stringify();
};

class ForStatement : public AstNode {
    Token m_token;
    AstNodePtr m_variable;

    // the object that we will iterate on the loop, like a list
    AstNodePtr m_sequence; 

    AstNodePtr m_body;

  public:
    ForStatement(Token tok, AstNodePtr variable, AstNodePtr sequence, AstNodePtr body);

    AstNodePtr variable();
    AstNodePtr sequence();
    AstNodePtr body();

    Token token();
    AstKind type();
    std::string stringify();
};

class BreakStatement : public AstNode {
    Token m_token;
  public:
    BreakStatement(Token tok);

    Token token();
    AstKind type();
    std::string stringify();
};

class PassStatement : public AstNode {
    Token m_token;
public:
    PassStatement(Token tok);

    Token token();
    AstKind type();
    std::string stringify();
};

class ContinueStatement : public AstNode {
    Token m_token;
  public:
    ContinueStatement(Token tok);

    Token token();
    AstKind type();
    std::string stringify();
};

class CppStatement : public AstNode {
    Token m_token;
    std::string m_cppCode;

  public:
    CppStatement(Token tok, std::string cpp);
    std::string value();

    Token token();
    AstKind type();
    std::string stringify();
};

class ScopeStatement : public AstNode {
    Token m_token;
    AstNodePtr m_scopeBody;

  public:
    ScopeStatement(Token tok, AstNodePtr body);
    AstNodePtr body();

    Token token();
    AstKind type();
    std::string stringify();
};

class TypeDefinition : public AstNode {
    Token m_token;
    AstNodePtr m_name;
    AstNodePtr m_type;

  public:
    TypeDefinition(Token tok, AstNodePtr name, AstNodePtr type);

    AstNodePtr name();
    AstNodePtr baseType();
  
    Token token();
    AstKind type();
    std::string stringify();
};

class MatchStatement : public AstNode {
    Token m_token;
    std::vector<AstNodePtr> m_toMatch;
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> m_cases;
    AstNodePtr m_default;

  public:
    MatchStatement(Token tok, std::vector<AstNodePtr> to_match,
                std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases,
                AstNodePtr defaultbody);

    std::vector<AstNodePtr> matchItem();
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> caseBody();
    AstNodePtr defaultBody();

    Token token();
    AstKind type();
    std::string stringify();
};

}

#endif