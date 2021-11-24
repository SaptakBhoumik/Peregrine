#ifndef PEREGRINE_TYPE_CHECKER_HPP
#define PEREGRINE_TYPE_CHECKER_HPP

#include "ast/ast.hpp"
#include "ast/types.hpp"
#include "utils/symbolTable.hpp"

#include <memory>
#include <vector>

using namespace types;

class TypeChecker {
    std::shared_ptr<SymbolTable<TypePtr>> m_env;

    // the function whose body is being currently checked
    std::shared_ptr<FunctionType> m_currentFunction;

  public:
    TypeChecker();

    void error(std::string_view msg);
    void expectType(TypePtr type1, TypePtr type2);
    TypePtr check(ast::AstNodePtr astNode);

    void enterLocalEnv();
    void exitLocalEnv();

    std::string identifierName(ast::AstNodePtr identifier);
};

#endif