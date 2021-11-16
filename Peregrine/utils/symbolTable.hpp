#ifndef PEREGRINE_SYM_TABLE_HPP
#define PEREGRINE_SYM_TABLE_HPP

#include "ast/ast.hpp"

#include <map>
#include <memory>
#include <string_view>
#include <optional>

template<typename T>
class SymbolTable {
    std::map<std::string_view, T> m_symbols;
    std::shared_ptr<SymbolTable> m_parent;

  public:
    SymbolTable(std::shared_ptr<SymbolTable> parent) {
        m_parent = parent;
    }

    std::optional<T> get(std::string_view name) {
        if (m_symbols.count(name) == 0) {
            if (!m_parent) {
                return std::nullopt; // the symbol was not found
            }

            return m_parent->get(name);
        }

        return m_symbols[name];
    }

    bool set(std::string_view name, T value) {
        if (get(name)) {
            return false; // the symbol has been defined already
        }

        m_symbols[name];
        return true;
    }

    bool reassign(std::string_view name, T value) {
        if (m_symbols.count(name) == 0) {
            if (!m_parent) {
                return false; // the symbol was not found
            }

            return m_parent->reassign(name, value);
        }

        m_symbols[name] = value;
        return true;
    }
};

#endif