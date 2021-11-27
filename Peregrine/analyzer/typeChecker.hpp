#ifndef PEREGRINE_TYPE_CHECKER_HPP
#define PEREGRINE_TYPE_CHECKER_HPP

#include "ast/ast.hpp"
#include "ast/types.hpp"
#include "utils/symbolTable.hpp"

#include <memory>
#include <vector>

using namespace types;

typedef std::shared_ptr<SymbolTable<TypePtr>> EnvPtr;

class TypeChecker {
    // the function whose body is being currently checked
    std::shared_ptr<FunctionType> m_currentFunction;

  public:
    TypeChecker();

    void error(std::string_view msg);

    void check(EnvPtr env, ast::AstNodePtr expr, TypePtr expectedType);
    TypePtr infer(EnvPtr env, ast::AstNodePtr astNode);

    std::string identifierName(ast::AstNodePtr identifier);
};

#endif