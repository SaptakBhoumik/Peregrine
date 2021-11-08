#ifndef PEREGRINE_AST_HPP
#define PEREGRINE_AST_HPP

#include "lexer/tokens.hpp"
#include <memory>
#include <string>
#include <vector>

enum AstKind {
    KAstProgram,
    KAstInteger,
    KAstDecimal,
    KAstString,
    KAstBool,
    KAstNone,
    KAstIdentifier,
    KAstList,
    KAstDict,
    KAstCpp,
    KAstBinaryOp,
    KAstPrefixExpr,
    KAstVariableStmt,
    KAstConstDecl,
    KAstBlockStmt,
    KAstFunctionDef,
    KAstFunctionCall,
    KAstIfStmt,
    KAstWhileStmt
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

  public:
    StringLiteral(std::string_view value, bool formatted);

    std::string value();
    bool formatted();

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

class IdentifierExpression : public AstNode {
    std::string m_value;

  public:
    IdentifierExpression(std::string_view value);

    std::string value();

    AstKind type();
    std::string stringify();
};

class ListLiteral : public AstNode {
    AstNodePtr m_type;
    std::vector<AstNodePtr> m_elements;

  public:
    ListLiteral(AstNodePtr type, std::vector<AstNodePtr> elements);

    AstNodePtr list_type();
    std::vector<AstNodePtr> elements();

    AstKind type();
    std::string stringify();
};

class BinaryOperation : public AstNode {
    AstNodePtr m_left;
    std::string m_operator;
    AstNodePtr m_right;

  public:
    BinaryOperation(AstNodePtr left, std::string_view op, AstNodePtr right);

    AstNodePtr left();
    std::string op();
    AstNodePtr right();

    AstKind type();
    std::string stringify();
};

class PrefixExpression : public AstNode {
    std::string m_prefix;
    AstNodePtr m_right;

  public:
    PrefixExpression(std::string_view prefix, AstNodePtr right);

    std::string prefix();
    AstNodePtr right();

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

    AstNodePtr var_type();
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

    AstNodePtr const_type();
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

class FunctionDefinition : public AstNode {
    AstNodePtr m_return_type;
    AstNodePtr m_name;

    std::vector<parameter> m_parameters;

    AstNodePtr m_body;

  public:
    // TODO: return void by default?
    FunctionDefinition(AstNodePtr return_type, AstNodePtr name,
                       std::vector<parameter> parameters, AstNodePtr body);

    AstNodePtr return_type();
    AstNodePtr name();
    std::vector<parameter> parameters();
    AstNodePtr body();

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
    AstNodePtr m_if_body;

    // first item in the pair is the condition, and the second is the block to
    // be executed
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elifs;

    AstNodePtr m_else_body;

  public:
    IfStatement(AstNodePtr condition, AstNodePtr if_body, AstNodePtr else_body,
                std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs);

    AstNodePtr condition();
    AstNodePtr if_body();
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs();
    AstNodePtr else_body();

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

#endif