#ifndef PEREGRINE_SYM_TABLE_HPP
#define PEREGRINE_SYM_TABLE_HPP

#include "ast/ast.hpp"

#include <map>
#include <memory>
#include <string_view>

class SymbolTable {
    std::map<std::string_view, AstNodePtr> m_symbols;
    std::shared_ptr<SymbolTable> m_parent;

  public:
    SymbolTable(std::shared_ptr<SymbolTable> parent);

    AstNodePtr get(std::string_view name);
    bool set(std::string_view name, AstNodePtr value);
    bool reassign(std::string_view name, AstNodePtr value);
};

#endif