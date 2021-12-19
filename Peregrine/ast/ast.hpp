#ifndef PEREGRINE_AST_HPP
#define PEREGRINE_AST_HPP

#include "lexer/tokens.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

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
    KAstList,
    KAstDict,
    KAstCpp,
    KAstBinaryOp,
    KAstPrefixExpr,
    KAstImportStmt,
    KAstVariableStmt,
    KAstConstDecl,
    KAstBlockStmt,
    KAstFunctionDef,
    KAstClassDef,
    KAstReturnStatement,
    KAstFunctionCall,
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
    virtual AstKind type() { return KAstNone; };
    virtual std::string stringify() { return ""; };
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class Program : public AstNode {
    std::vector<AstNodePtr> m_statements;

  public:
    Program(std::vector<AstNodePtr> statements);

    std::vector<AstNodePtr> statements();

    AstKind type();
    std::string stringify();
};

class IntegerLiteral : public AstNode {
    std::string m_value;

  public:
    IntegerLiteral(std::string_view value);

    std::string value();

    AstKind type();
    std::string stringify();
};

class DecimalLiteral : public AstNode {
    std::string m_value;

  public:
    DecimalLiteral(std::string_view value);

    std::string value();

    AstKind type();
    std::string stringify();
};

class StringLiteral : public AstNode {
    std::string m_value;
    bool m_formatted;
    bool m_raw;

  public:
    StringLiteral(std::string_view value, bool formatted, bool raw);

    std::string value();
    bool formatted();
    bool raw();

    AstKind type();
    std::string stringify();
};

class BoolLiteral : public AstNode {
    std::string m_value;

  public:
    BoolLiteral(std::string_view value);

    std::string value();

    AstKind type();
    std::string stringify();
};

class NoneLiteral : public AstNode {
  public:
    NoneLiteral();

    AstKind type();
    std::string stringify();
};

class NoLiteral : public AstNode {
  public:
    NoLiteral();

    AstKind type();
    std::string stringify();
};

class IdentifierExpression : public AstNode {
    std::string m_value;

  public:
    IdentifierExpression(std::string_view value);

    std::string value();

    AstKind type();
    std::string stringify();
};

class TypeExpression : public AstNode {
    std::string m_value;

  public:
    TypeExpression(std::string_view value);

    std::string value();

    AstKind type();
    std::string stringify();
};

class ListLiteral : public AstNode {
    AstNodePtr m_type;
    std::vector<AstNodePtr> m_elements;

  public:
    ListLiteral(AstNodePtr type, std::vector<AstNodePtr> elements);

    AstNodePtr listType();
    std::vector<AstNodePtr> elements();

    AstKind type();
    std::string stringify();
};

class DictLiteral : public AstNode {
    // first is the key, second is the value
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elements;

  public:
    DictLiteral(std::vector<std::pair<AstNodePtr, AstNodePtr>> elements);

    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements();

    AstKind type();
    std::string stringify();
};

class BinaryOperation : public AstNode {
    AstNodePtr m_left;
    Token m_operator;
    AstNodePtr m_right;

  public:
    BinaryOperation(AstNodePtr left, Token op, AstNodePtr right);

    AstNodePtr left();
    Token op();
    AstNodePtr right();

    AstKind type();
    std::string stringify();
};

class PrefixExpression : public AstNode {
    Token m_prefix;
    AstNodePtr m_right;

  public:
    PrefixExpression(Token prefix, AstNodePtr right);

    Token prefix();
    AstNodePtr right();

    AstKind type();
    std::string stringify();
};

class ImportStatement : public AstNode {
    // first is the module name and the second is the alias
    std::pair<AstNodePtr, AstNodePtr> m_moduleName;

    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_importedSymbols;

  public:
    ImportStatement(
        std::pair<AstNodePtr, AstNodePtr> moduleName,
        std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols);

    std::pair<AstNodePtr, AstNodePtr> moduleName();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols();

    AstKind type();
    std::string stringify();
};

// variable declaration, assignment and reassignment
class VariableStatement : public AstNode {
    AstNodePtr m_type;
    AstNodePtr m_name;
    AstNodePtr m_value;

  public:
    VariableStatement(AstNodePtr type, AstNodePtr name, AstNodePtr value);

    AstNodePtr varType();
    AstNodePtr name();
    AstNodePtr value();

    AstKind type();
    std::string stringify();
};

class ConstDeclaration : public AstNode {
    AstNodePtr m_type;
    AstNodePtr m_name;
    AstNodePtr m_value;

  public:
    ConstDeclaration(AstNodePtr type, AstNodePtr name, AstNodePtr value);

    AstNodePtr constType();
    AstNodePtr name();
    AstNodePtr value();

    AstKind type();
    std::string stringify();
};

class BlockStatement : public AstNode {
    std::vector<AstNodePtr> m_statements;

  public:
    BlockStatement(std::vector<AstNodePtr> statements);

    std::vector<AstNodePtr> statements();

    AstKind type();
    std::string stringify();
};

struct parameter {
    AstNodePtr p_type;
    AstNodePtr p_name;
};

class ClassDefinition: public AstNode {

  AstNodePtr c_name;
  AstNodePtr c_type;
  std::vector<AstNodePtr> c_attributes;
  std::vector<AstNodePtr> c_methods;
  
  public:
    ClassDefinition(AstNodePtr c_name,std::vector<AstNodePtr> c_attributes,std::vector<AstNodePtr> c_methods);
    AstNodePtr name();
    std::vector<AstNodePtr> attributes();
    std::vector<AstNodePtr> methods();
    AstKind type();
    std::string stringify();
};


class FunctionDefinition : public AstNode {
    AstNodePtr m_returnType;
    AstNodePtr m_name;

    std::vector<parameter> m_parameters;

    AstNodePtr m_body;

  public:
    FunctionDefinition(AstNodePtr returnType, AstNodePtr name,
                       std::vector<parameter> parameters, AstNodePtr body);

    AstNodePtr returnType();
    AstNodePtr name();
    std::vector<parameter> parameters();
    AstNodePtr body();

    AstKind type();
    std::string stringify();
};

class ReturnStatement : public AstNode {
    AstNodePtr m_returnValue;

  public:
    ReturnStatement(AstNodePtr returnValue);

    AstNodePtr returnValue();

    AstKind type();
    std::string stringify();
};

class FunctionCall : public AstNode {
    AstNodePtr m_name;
    std::vector<AstNodePtr> m_arguments;

  public:
    FunctionCall(AstNodePtr name, std::vector<AstNodePtr> arguments);

    AstNodePtr name();
    std::vector<AstNodePtr> arguments();

    AstKind type();
    std::string stringify();
};

class IfStatement : public AstNode {
    AstNodePtr m_condition;
    AstNodePtr m_ifBody;

    // first item in the pair is the condition, and the second is the block to
    // be executed
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elifs;

    AstNodePtr m_elseBody;

  public:
    IfStatement(AstNodePtr condition, AstNodePtr ifBody, AstNodePtr elseBody,
                std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs);

    AstNodePtr condition();
    AstNodePtr ifBody();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs();
    AstNodePtr elseBody();

    AstKind type();
    std::string stringify();
};

class WhileStatement : public AstNode {
    AstNodePtr m_condition;
    AstNodePtr m_body;

  public:
    WhileStatement(AstNodePtr condition, AstNodePtr body);

    AstNodePtr condition();
    AstNodePtr body();

    AstKind type();
    std::string stringify();
};

class ForStatement : public AstNode {
    AstNodePtr m_variable;
    AstNodePtr
        m_sequence; // the object that we will iterate on the loop, like a list
    AstNodePtr m_body;

  public:
    ForStatement(AstNodePtr variable, AstNodePtr sequence, AstNodePtr body);

    AstNodePtr variable();
    AstNodePtr sequence();
    AstNodePtr body();

    AstKind type();
    std::string stringify();
};

class BreakStatement : public AstNode {
  public:
    AstKind type();
    std::string stringify();
};

class PassStatement : public AstNode {
  public:
    AstKind type();
    std::string stringify();
};

class ContinueStatement : public AstNode {
  public:
    AstKind type();
    std::string stringify();
};

class CppStatement : public AstNode {
    std::string m_cppCode;

  public:
    CppStatement(std::string cpp);
    std::string value();
    AstKind type();
    std::string stringify();
};

class ScopeStatement : public AstNode {
    AstNodePtr m_scopeBody;

  public:
    ScopeStatement(AstNodePtr body);
    AstNodePtr body();
    AstKind type();
    std::string stringify();
};

class TypeDefinition : public AstNode {
    AstNodePtr m_name;
    AstNodePtr m_type;

  public:
    TypeDefinition(AstNodePtr name, AstNodePtr type);

    AstNodePtr name();
    AstNodePtr baseType();
  
    AstKind type();
    std::string stringify();
};

class MatchStatement : public AstNode {
    std::vector<AstNodePtr> m_to_match;
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> m_cases;
    AstNodePtr m_default;

  public:
    MatchStatement(std::vector<AstNodePtr> to_match,
                std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases,
                AstNodePtr defaultbody);

    std::vector<AstNodePtr> matchItem();
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> caseBody();
    AstNodePtr defaultBody();

    AstKind type();
    std::string stringify();
};

#endif
