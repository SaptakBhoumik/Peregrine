#include "symbolTable.hpp"

#include <memory>
#include <string_view>

SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent) {
    m_parent = parent;
}

AstNodePtr SymbolTable::get(std::string_view name) {
    if (m_symbols.count(name) == 0) {
        if (!m_parent) {
            return nullptr; // the symbol was not found
        }

        return m_parent->get(name);
    }

    return m_symbols[name];
}

// returns true if the symbol could be created
bool SymbolTable::set(std::string_view name, AstNodePtr value) {
    if (get(name)) {
        return false; // the symbol has been defined already
    }

    m_symbols[name];
    return true;
}

bool SymbolTable::reassign(std::string_view name, AstNodePtr value) {
    if (m_symbols.count(name) == 0) {
        if (!m_parent) {
            return false; // the symbol was not found
        }

        return m_parent->reassign(name, value);
    }

    m_symbols[name] = value;
    return true;
}