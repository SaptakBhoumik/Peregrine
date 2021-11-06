#ifndef PEREGRINE_AST_HPP
#define PEREGRINE_AST_HPP

#include "lexer/tokens.hpp"
#include <string>
#include <vector>

enum AstKind {
    KAstProgram,
    KAstInteger,
    KAstDecimal,
    KAstString,
    KAstBool,
    KAstNone,
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
    virtual AstKind type() = 0;
    virtual std::string stringify() = 0;
};

class Program : public AstNode {
    std::vector<AstNode> m_statements;

  public:
    Program(std::vector<AstNode> statements);

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

class ListLiteral : public AstNode {
    AstNode m_type;
    std::vector<AstNode> m_elements;

  public:
    ListLiteral(AstNode type, std::vector<AstNode> elements);

    AstNode list_type();
    std::vector<AstNode> elements();

    AstKind type();
    std::string stringify();
};

class BinaryOperation : public AstNode {
    AstNode m_left;
    std::string m_operator;
    AstNode m_right;

  public:
    BinaryOperation(AstNode left, std::string_view op, AstNode right);

    AstNode left();
    std::string op();
    AstNode right();

    AstKind type();
    std::string stringify();
};

class PrefixExpression : public AstNode {
    std::string m_prefix;
    AstNode m_right;

  public:
    PrefixExpression(std::string_view prefix, AstNode right);

    std::string prefix();
    AstNode right();

    AstKind type();
    std::string stringify();
};

// variable declaration, assignment and reassignment
class VariableStatement : public AstNode {
    AstNode m_type;
    AstNode m_name;
    AstNode m_value;

  public:
    /*
        We need to handle all the possibilities, hence the NoneLiteral()

        "int a" has no value
        "a = 32" has no type (it's inferred)
    */
    VariableStatement(AstNode type = NoneLiteral(), AstNode name,
                      AstNode value = NoneLiteral());

    AstNode var_type();
    AstNode name();
    AstNode value();

    AstKind type();
    std::string stringify();
};

class ConstDeclaration : public AstNode {
    AstNode m_type;
    AstNode m_name;
    AstNode m_value;

  public:
    ConstDeclaration(AstNode type = NoneLiteral(), AstNode name, AstNode value);

    AstNode const_type();
    AstNode name();
    AstNode value();

    AstKind type();
    std::string stringify();
};

class BlockStatement : public AstNode {
    std::vector<AstNode> m_statements;

  public:
    BlockStatement(std::vector<AstNode> statements);

    std::vector<AstNode> statements();

    AstKind type();
    std::string stringify();
};

struct parameter {
    AstNode p_type;
    AstNode p_name;
};

class FunctionDefinition : public AstNode {
    AstNode m_return_type;
    AstNode m_name;

    std::vector<parameter> m_parameters;

    AstNode m_body;

  public:
    // TODO: return void by default?
    FunctionDefinition(
        AstNode return_type, AstNode name,
        std::vector<parameter> parameters = std::vector<parameter>(),
        AstNode body);

    AstNode return_type();
    AstNode name();
    std::vector<parameter> parameters();
    AstNode body();

    AstKind type();
    std::string stringify();
};

class FunctionCall : public AstNode {
    AstNode m_name;
    std::vector<AstNode> m_arguments;

  public:
    FunctionCall(AstNode name,
                 std::vector<AstNode> arguments = std::vector<AstNode>());

    AstNode name();
    std::vector<AstNode> arguments();

    AstKind type();
    std::string stringify();
};

class IfStatement : public AstNode {
    AstNode m_condition;
    AstNode m_if_body;

    // first item in the pair is the condition, and the second is the block to
    // be executed
    std::vector<std::pair<AstNode, AstNode>> m_elifs;

    AstNode m_else_body;

  public:
    IfStatement(AstNode condition, AstNode if_body,
                AstNode else_body = NoneLiteral(),
                std::vector<std::pair<AstNode, AstNode>> elifs =
                    std::vector<std::pair<AstNode, AstNode>>());

    AstNode condition();
    AstNode if_body();
    std::vector<std::pair<AstNode, AstNode>> elifs();
    AstNode else_body();

    AstKind type();
    std::string stringify();
};

class WhileStatement : public AstNode {
    AstNode m_condition;
    AstNode m_body;

  public:
    WhileStatement(AstNode condition, AstNode body);

    AstNode condition();
    AstNode body();

    AstKind type();
    std::string stringify();
};

#endif