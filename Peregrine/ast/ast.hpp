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
    KAstPointerTypeExpr,
    KAstReferencedTypeExpr,
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
    KAstClassDef,
    KAstReturnStatement,
    KAstFunctionCall,
    KAstDotExpression,
    KAstIfStmt,
    KAstAssertStmt,
    KAstMatchStmt,
    KAstScopeStmt,
    KAstWhileStmt,
    KAstForStatement,
    KAstBreakStatement,
    KAstContinueStatement,
    KAstTypeDefinition,
    KAstRaiseStmt,
    KAstDecorator,
    KAstPassStatement,
    KAstUnion,
    KAstStatic,
    KAstInline,
    KAstEnum,
    KAstWith,
    KAstVirtual,
    KAstCast
};

class AstVisitor;

class AstNode {
  public:
    virtual ~AstNode() = default;

    virtual Token token() const = 0;
    virtual AstKind type() const = 0;
    virtual std::string stringify() const = 0;
    virtual void accept(AstVisitor& visitor) const = 0;
};

using AstNodePtr = std::shared_ptr<AstNode>;

class Program : public AstNode {
    std::vector<AstNodePtr> m_statements;

  public:
    Program(std::vector<AstNodePtr> statements);

    std::vector<AstNodePtr> statements() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class IntegerLiteral : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    IntegerLiteral(Token tok, std::string_view value);

    std::string value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class DecimalLiteral : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    DecimalLiteral(Token tok, std::string_view value);

    std::string value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class StringLiteral : public AstNode {
    Token m_token;
    std::string m_value;
    bool m_formatted;
    bool m_raw;

  public:
    StringLiteral(Token tok, std::string_view value, bool formatted, bool raw);

    std::string value() const;
    bool formatted() const;
    bool raw() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class BoolLiteral : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    BoolLiteral(Token tok, std::string_view value);

    std::string value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class NoneLiteral : public AstNode {
    Token m_token;

  public:
    NoneLiteral(Token tok);

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class NoLiteral : public AstNode {
  public:
    NoLiteral() = default;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class IdentifierExpression : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    IdentifierExpression(Token tok, std::string_view value);

    std::string value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

// ------------ TYPES ------------

class TypeExpression : public AstNode {
    Token m_token;
    std::string m_value;

  public:
    TypeExpression(Token tok, std::string_view value);

    std::string value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ListTypeExpr : public AstNode {
    Token m_token;
    AstNodePtr m_elemType;
    AstNodePtr m_fixed_size;

  public:
    ListTypeExpr(Token tok, AstNodePtr elemType,AstNodePtr fixed_size);

    AstNodePtr elemType() const;

    AstNodePtr fixed_size() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class PointerTypeExpr : public AstNode {
    Token m_token;
    AstNodePtr m_baseType;

  public:
    PointerTypeExpr(Token tok, AstNodePtr baseType);

    AstNodePtr baseType() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ReferenceTypeExpr : public AstNode {
    Token m_token;
    AstNodePtr m_baseType;

  public:
    ReferenceTypeExpr(Token tok, AstNodePtr baseType);

    AstNodePtr baseType() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class DictTypeExpr : public AstNode {
    Token m_token;
    AstNodePtr m_keyType;
    AstNodePtr m_valueType;

  public:
    DictTypeExpr(Token tok, AstNodePtr keyType, AstNodePtr valueType);

    AstNodePtr keyType() const;
    AstNodePtr valueType() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class FunctionTypeExpr : public AstNode {
    Token m_token;
    std::vector<AstNodePtr> m_argTypes;
    std::vector<AstNodePtr> m_returnTypes;

  public:
    FunctionTypeExpr(Token tok, std::vector<AstNodePtr> argTypes,
                     std::vector<AstNodePtr> returnTypes);

    std::vector<AstNodePtr> argTypes() const;
    std::vector<AstNodePtr> returnTypes() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

// ------------------------------

class ListLiteral : public AstNode {
    Token m_token;
    AstNodePtr m_type;
    std::vector<AstNodePtr> m_elements;

  public:
    ListLiteral(Token tok, AstNodePtr type, std::vector<AstNodePtr> elements);

    AstNodePtr listType() const;
    std::vector<AstNodePtr> elements() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class DictLiteral : public AstNode {
    Token m_token;
    // first is the key, second is the value
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elements;

  public:
    DictLiteral(Token tok,
                std::vector<std::pair<AstNodePtr, AstNodePtr>> elements);

    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class UnionLiteral : public AstNode {
    Token m_token;
    // first is the key, second is the value
    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_elements;
    AstNodePtr m_name;

  public:
    UnionLiteral(Token tok,
                 std::vector<std::pair<AstNodePtr, AstNodePtr>> elements,
                 AstNodePtr name);

    std::vector<std::pair<AstNodePtr, AstNodePtr>> elements() const;

    AstNodePtr name() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class EnumLiteral : public AstNode {
    Token m_token;

    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_fields;
    AstNodePtr m_name;

  public:
    EnumLiteral(Token tok,
                std::vector<std::pair<AstNodePtr, AstNodePtr>> fields,
                AstNodePtr name);

    std::vector<std::pair<AstNodePtr, AstNodePtr>> fields() const;

    AstNodePtr name() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class BinaryOperation : public AstNode {
    Token m_token;
    AstNodePtr m_left;
    Token m_operator;
    AstNodePtr m_right;

  public:
    BinaryOperation(Token tok, AstNodePtr left, Token op, AstNodePtr right);

    AstNodePtr left() const;
    Token op() const;
    AstNodePtr right() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class PrefixExpression : public AstNode {
    Token m_token;
    Token m_prefix;
    AstNodePtr m_right;

  public:
    PrefixExpression(Token tok, Token prefix, AstNodePtr right);

    Token prefix() const;
    AstNodePtr right() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ListOrDictAccess : public AstNode {
    Token m_token;
    AstNodePtr m_container;
    AstNodePtr m_keyOrIndex;

  public:
    ListOrDictAccess(Token tok, AstNodePtr container, AstNodePtr keyOrIndex);

    AstNodePtr container() const;
    AstNodePtr keyOrIndex() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ImportStatement : public AstNode {
    Token m_token;
    // first is the module name and the second is the alias
    std::pair<AstNodePtr, AstNodePtr> m_moduleName;

    std::vector<std::pair<AstNodePtr, AstNodePtr>> m_importedSymbols;

  public:
    ImportStatement(
        Token tok, std::pair<AstNodePtr, AstNodePtr> moduleName,
        std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols);

    std::pair<AstNodePtr, AstNodePtr> moduleName() const;
    std::vector<std::pair<AstNodePtr, AstNodePtr>> importedSymbols() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

// variable declaration, assignment and reassignment
class VariableStatement : public AstNode {
    Token m_token;
    AstNodePtr m_type;
    AstNodePtr m_name;
    AstNodePtr m_value;

  public:
    VariableStatement(Token tok, AstNodePtr type, AstNodePtr name,
                      AstNodePtr value);

    AstNodePtr varType() const;
    AstNodePtr name() const;
    AstNodePtr value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ConstDeclaration : public AstNode {
    Token m_token;
    AstNodePtr m_type;
    AstNodePtr m_name;
    AstNodePtr m_value;

  public:
    ConstDeclaration(Token tok, AstNodePtr type, AstNodePtr name,
                     AstNodePtr value);

    AstNodePtr constType() const;
    AstNodePtr name() const;
    AstNodePtr value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class BlockStatement : public AstNode {
    std::vector<AstNodePtr> m_statements;

  public:
    BlockStatement(std::vector<AstNodePtr> statements);

    std::vector<AstNodePtr> statements() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

struct parameter {
    AstNodePtr p_type;
    AstNodePtr p_name;
};

class ClassDefinition : public AstNode {
    Token m_token;
    AstNodePtr m_name;
    std::vector<AstNodePtr> m_parent;//NOTE:class test(parent1,parent2)
    std::vector<AstNodePtr> m_attributes;
    std::vector<AstNodePtr> m_methods;
    std::vector<AstNodePtr> m_other;

  public:
    ClassDefinition(Token tok, AstNodePtr name, std::vector<AstNodePtr>  parent,
                    std::vector<AstNodePtr> attributes,
                    std::vector<AstNodePtr> methods,
                    std::vector<AstNodePtr> other);

    AstNodePtr name() const;
    std::vector<AstNodePtr>  parent() const;
    std::vector<AstNodePtr> attributes() const;
    std::vector<AstNodePtr> methods() const;
    std::vector<AstNodePtr> other() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
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

    AstNodePtr returnType() const;
    AstNodePtr name() const;
    std::vector<parameter> parameters() const;
    AstNodePtr body() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ReturnStatement : public AstNode {
    Token m_token;
    AstNodePtr m_returnValue;

  public:
    ReturnStatement(Token tok, AstNodePtr returnValue);

    AstNodePtr returnValue() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class FunctionCall : public AstNode {
    Token m_token;
    AstNodePtr m_name;
    std::vector<AstNodePtr> m_arguments;

  public:
    FunctionCall(Token tok, AstNodePtr name, std::vector<AstNodePtr> arguments);

    AstNodePtr name() const;
    std::vector<AstNodePtr> arguments() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

// test.hello(), obj.prop, etc
class DotExpression : public AstNode {
    Token m_token;
    AstNodePtr m_owner;      // the node that comes before the .
    AstNodePtr m_referenced; // the node that comes after the .

  public:
    DotExpression(Token tok, AstNodePtr owner, AstNodePtr referenced);

    AstNodePtr owner() const;
    AstNodePtr referenced() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
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
    IfStatement(Token tok, AstNodePtr condition, AstNodePtr ifBody,
                AstNodePtr elseBody,
                std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs);

    AstNodePtr condition() const;
    AstNodePtr ifBody() const;
    std::vector<std::pair<AstNodePtr, AstNodePtr>> elifs() const;
    AstNodePtr elseBody() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class AssertStatement : public AstNode {
    Token m_token;
    AstNodePtr m_condition;

  public:
    AssertStatement(Token tok, AstNodePtr condition);
    AstNodePtr condition() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class StaticStatement : public AstNode {
    Token m_token;
    AstNodePtr m_body;

  public:
    StaticStatement(Token tok, AstNodePtr body);
    AstNodePtr body() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class InlineStatement : public AstNode {
    Token m_token;
    AstNodePtr m_body;

  public:
    InlineStatement(Token tok, AstNodePtr body);
    AstNodePtr body() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class VirtualStatement : public AstNode {
    Token m_token;
    AstNodePtr m_body;

  public:
    VirtualStatement(Token tok, AstNodePtr body);
    AstNodePtr body() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class RaiseStatement : public AstNode {
    Token m_token;
    AstNodePtr m_value;

  public:
    RaiseStatement(Token tok, AstNodePtr value);
    AstNodePtr value() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class WhileStatement : public AstNode {
    Token m_token;
    AstNodePtr m_condition;
    AstNodePtr m_body;

  public:
    WhileStatement(Token tok, AstNodePtr condition, AstNodePtr body);

    AstNodePtr condition() const;
    AstNodePtr body() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ForStatement : public AstNode {
    Token m_token;
    AstNodePtr m_variable;

    // the object that we will iterate on the loop, like a list
    AstNodePtr m_sequence;

    AstNodePtr m_body;

  public:
    ForStatement(Token tok, AstNodePtr variable, AstNodePtr sequence,
                 AstNodePtr body);

    AstNodePtr variable() const;
    AstNodePtr sequence() const;
    AstNodePtr body() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class BreakStatement : public AstNode {
    Token m_token;

  public:
    BreakStatement(Token tok);

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class PassStatement : public AstNode {
    Token m_token;

  public:
    PassStatement(Token tok);

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ContinueStatement : public AstNode {
    Token m_token;

  public:
    ContinueStatement(Token tok);

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class CppStatement : public AstNode {
    Token m_token;
    std::string m_cppCode;

  public:
    CppStatement(Token tok, std::string cpp);

    std::string value() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class ScopeStatement : public AstNode {
    Token m_token;
    AstNodePtr m_scopeBody;

  public:
    ScopeStatement(Token tok, AstNodePtr body);

    AstNodePtr body() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class TypeDefinition : public AstNode {
    Token m_token;
    AstNodePtr m_name;
    AstNodePtr m_type;

  public:
    TypeDefinition(Token tok, AstNodePtr name, AstNodePtr type);

    AstNodePtr name() const;
    AstNodePtr baseType() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class MatchStatement : public AstNode {
    Token m_token;
    std::vector<AstNodePtr> m_toMatch;
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> m_cases;
    AstNodePtr m_default;

  public:
    MatchStatement(
        Token tok, std::vector<AstNodePtr> toMatch,
        std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>> cases,
        AstNodePtr defaultbody);

    std::vector<AstNodePtr> matchItem() const;
    std::vector<std::pair<std::vector<AstNodePtr>, AstNodePtr>>
    caseBody() const;
    AstNodePtr defaultBody() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class DecoratorStatement : public AstNode {
    Token m_token;
    std::vector<AstNodePtr> m_decorators;
    AstNodePtr m_body;

  public:
    DecoratorStatement(Token tok, std::vector<AstNodePtr> decorators,
                       AstNodePtr body);

    std::vector<AstNodePtr> decoratorItem() const;
    AstNodePtr body() const;

    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};
class WithStatement : public AstNode {
    Token m_token;
    std::vector<AstNodePtr> m_variables;
    std::vector<AstNodePtr> m_values;
    AstNodePtr m_body;

  public:
    WithStatement(Token tok, std::vector<AstNodePtr> variables,
                  std::vector<AstNodePtr> values, AstNodePtr body);

    std::vector<AstNodePtr> variables() const;
    std::vector<AstNodePtr> values() const;
    AstNodePtr body() const;
    Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};
class CastStatement:public AstNode{
  Token m_token;
  AstNodePtr m_type;
  AstNodePtr m_value;
  public:
  CastStatement(Token token,
                AstNodePtr type,
                AstNodePtr value);
  AstNodePtr cast_type() const;
  AstNodePtr value() const;
  Token token() const;
  AstKind type() const;
  std::string stringify() const;
  void accept(AstVisitor& visitor) const;
};
} // namespace ast

#endif
