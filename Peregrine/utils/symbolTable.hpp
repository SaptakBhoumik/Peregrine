#ifndef PEREGRINE_SYM_TABLE_HPP
#define PEREGRINE_SYM_TABLE_HPP

#include "ast/ast.hpp"

#include <iostream>
#include <assert.h>
#include <map>
#include <memory>
#include <algorithm>
#include <string>
#include <optional>
namespace Utils{
template<typename T>
class SymbolTable {
    std::map<std::string, T> m_symbols;
    std::shared_ptr<SymbolTable<T>> m_parent;
    std::map<std::string, T> m_enumMap;

  public:
    SymbolTable(std::shared_ptr<SymbolTable> parent) {
        m_parent = parent;
        if(m_parent!=nullptr){
            m_enumMap = parent->getEnumMap();
        }
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

    std::map<std::string, T> getEnumMap(){
        return m_enumMap;
    }

    void add_enum(std::string key,T value){
        m_enumMap[key]=value;
    }

    bool set(std::string name, T value) {
        if (get(name)) {
            return false; // the symbol has been defined already
        }

        m_symbols[name] = value;
        get(name);
        return true;
    }

    bool set(ast::AstNodePtr name, T value) {
        assert(name->type() == ast::KAstIdentifier);
        auto identifier=std::dynamic_pointer_cast<ast::IdentifierExpression>(name);
        return set(identifier->value(), value);
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

class MangleName{
    std::map<std::string, std::string> m_global_names={
                                                        {"error","error"},
                                                        {"printf","printf"},
                                                        {"i8","int8_t"},
                                                        {"i16","int16_t"},
                                                        {"i32","int32_t"},
                                                        {"int","int64_t"},
                                                        {"u8","uint8_t"},
                                                        {"u16","uint16_t"},
                                                        {"u32","uint32_t"},
                                                        {"uint","uint64_t"},
                                                        {"f32","float"},
                                                        {"float","double"},
                                                        {"f128","long double"},
                                                        };
    std::map<std::string, std::string> m_local_names;
    public:
    MangleName()=default;
    MangleName(MangleName const &other);
    void set_local(std::string original);
    void set_local(std::string original,std::string mangled);
    void set_global(std::string original,std::string mangled);
    
    bool contains(std::string name);
    std::string operator[](std::string name);
    void print();
};
}

#endif