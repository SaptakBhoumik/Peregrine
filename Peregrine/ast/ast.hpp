#ifndef PEREGRINE_AST
#define PEREGRINE_AST
#include "../lexer/tokens.hpp"
#include <memory>
#include <string>
#include <vector>

/// AST - Base class for all expression nodes.
class Ast {
public:
  virtual ~Ast() {}
};
/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public Ast {
  Token Val;

public:
  NumberExprAST(Token Val) : Val(Val) {}
};
/// StringExprAST - Expression class for string literals like "a".
class StringExprAST : public Ast {
  Token Val;

public:
  StringExprAST(Token Val) : Val(Val) {}
};
/// true_false_AST - Expression class for bool values like True and False.
class true_false_AST : public Ast {
  Token Val;

public:
  true_false_AST(Token Val) : Val(Val) {}
};
/// ListExprAST - Expression class for list literals like "[a,b]".
class ListExprAST : public Ast {
  Token Val;

public:
  ListExprAST(Token Val) : Val(Val) {}
};
/// DictExprAST - Expression class for struct,dictionary literals like "{a:B}".
class DictExprAST : public Ast {
  Token Val;

public:
  DictExprAST(Token Val) : Val(Val) {}
};
/// CppExprAST - Expression class for anything inside Cpp().
class CppExprAST : public Ast {
  Token Val;

public:
  CppExprAST(Token Val) : Val(Val) {}
};
/// VariableExprAST - Expression class for referencing a variable, like a.
class VariableExprAST : public Ast {
  Token Name;
  Token Type;            // infer type and also includes the `type` keyword
  bool is_const = false; // checks if constant
public:
  VariableExprAST(const Token &Name, Token Type, bool is_const)
      : Name(Name), Type(Type), is_const(is_const) {}
};

/// OperaterExprAST - Expression class for a binary operator+assign+compare+or_and etc.
class OperaterExprAST : public Ast {
  Token Op;
  std::unique_ptr<Ast> LHS, RHS;

public:
  OperaterExprAST(Token &op, std::unique_ptr<Ast> LHS, std::unique_ptr<Ast> RHS)
      : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};
// formatted_str - formated string like f"Hello {name}"
class formatted_str : public Ast {
  Token token;
  StringExprAST string;

public:
  formatted_str(Token token, StringExprAST string)
      : token(token), string(string) {}
};
// raw_str - raw string like r"Hello \n"
class raw_str : public Ast {
  Token token;
  StringExprAST string;

public:
  raw_str(Token token, StringExprAST string) : token(token), string(string) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public Ast {
  Token Callee;
  std::vector<std::unique_ptr<Ast>> Args;

public:
  CallExprAST(const Token &Callee, std::vector<std::unique_ptr<Ast>> Args)
      : Callee(Callee), Args(std::move(Args)) {}
};

// DecoratorExprAST - Expression class for decorator calls.
class DecoratorExprAST : public Ast {
  Token Callee;
  std::vector<std::unique_ptr<Ast>> Args;

public:
  DecoratorExprAST(const Token &Callee, std::vector<std::unique_ptr<Ast>> Args)
      : Callee(Callee), Args(std::move(Args)) {}
};

/// PrototypeAST - This class represents the "prototype" for a function+class,
/// which captures its name, and its argument names .
class PrototypeAST {
  Token Name;
  std::vector<std::unique_ptr<Ast>> Args;
  std::vector<Token> return_type;
  std::vector<DecoratorExprAST> decorator;

public:
  PrototypeAST(const Token &name, std::vector<std::unique_ptr<Ast>> Args,
               std::vector<Token> return_type,
               std::vector<DecoratorExprAST> decorator)
      : Name(name), Args(std::move(Args)), return_type(return_type),
        decorator(decorator) {}

  const Token &getName() const { return Name; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<Ast> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<Ast> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}
};
/// ClassAST - This class represents a class/object definition itself.
class ClassAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<Ast> Body;

public:
  ClassAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<Ast> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)){}
};
/// ScopeAST - This class represents a scope definition .
class ScopeAST {
  Token token;
  std::vector<std::unique_ptr<Ast>> Body;

public:
  ScopeAST(Token token, std::vector<std::unique_ptr<Ast>> Body)
      : token(token), Body(std::move(Body)){}
};
/// EnumAST - This class represents a enum definition .
class EnumAST {
  Token token;
  std::vector<std::unique_ptr<Ast>> Body;

public:
  EnumAST(Token token, std::vector<std::unique_ptr<Ast>> Body)
      : token(token), Body(std::move(Body)){}
};
/// ReturnAST - This class represents a return definition .
class ReturnAST {
  Token token;
  std::vector<std::unique_ptr<Ast>> Body;

public:
  ReturnAST(Token token, std::vector<std::unique_ptr<Ast>> Body)
      : token(token), Body(std::move(Body)){}
};

#endif
