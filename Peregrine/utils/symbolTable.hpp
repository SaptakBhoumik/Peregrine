#ifndef PEREGRINE_SYM_TABLE_HPP
#define PEREGRINE_SYM_TABLE_HPP

#include "ast/ast.hpp"

#include <map>
#include <memory>
#include <string>
#include <optional>

template<typename T>
class SymbolTable {
    std::map<std::string, T> m_symbols;
    std::shared_ptr<SymbolTable<T>> m_parent;

  public:
    SymbolTable(std::shared_ptr<SymbolTable> parent) {
        m_parent = parent;
    }

    std::optional<T> get(std::string name) {
        if (m_symbols.find(name) == m_symbols.end()) {
            if (!m_parent) {
                return std::nullopt; // the symbol was not found
            }

            return m_parent->get(name);
        }
        
        return m_symbols[name];
    }

    bool set(std::string name, T value) {
        if (get(name)) {
            return false; // the symbol has been defined already
        }

        m_symbols[name] = value;
        get(name);
        return true;
    }

    bool reassign(std::string name, T value) {
        if (m_symbols.find(name) == m_symbols.end()) {
            if (!m_parent) {
                return false; // the symbol was not found
            }

            return m_parent->reassign(name, value);
        }

        m_symbols[name] = value;
        return true;
    }

    std::shared_ptr<SymbolTable<T>> parent() {
        return m_parent;
    }
};

#endif