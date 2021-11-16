#ifndef PEREGRINE_TYPE_CHECKER_HPP
#define PEREGRINE_TYPE_CHECKER_HPP

#include "ast/ast.hpp"
#include "ast/types.hpp"
#include "utils/symbolTable.hpp"

#include <memory>
#include <vector>

class TypeChecker {
    std::unique_ptr<SymbolTable<TypePtr>> m_env;

public:
    TypeChecker();

    void error(std::string_view msg);
    void expectType(TypePtr type1, TypePtr type2);
    TypePtr check(AstNodePtr astNode);
};

#endif