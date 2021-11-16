#ifndef PEREGRINE_TYPES_HPP
#define PEREGRINE_TYPES_HPP

#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <map>
#include <string>
#include <memory>

enum TypeCategory {
    Integer,
    Uint8,
    Uint16,
    Uint32,
    Uint64,
    Float,
    Double,
    String,
    Bool,
    List,
    UserDefined
};

enum IntSizes {
    Int8,
    Int16,
    Int32,
    Int64
};

class Type;

typedef std::shared_ptr<Type> TypePtr;

class Type {
public:
    virtual TypeCategory category() = 0;
    virtual bool isConvertibleTo(const TypePtr type) = 0;
    virtual std::string stringify() { return ""; }

    // returns the type obtained after applying the given operator to this type
    // e.g. -34 -> Integer
    virtual TypePtr prefixOperatorResult(Token op) { return nullptr; }

    // returns the type obtained after applying the given operator to both types
    // e.g. false == false -> Bool
    virtual TypePtr infixOperatorResult(Token op, const TypePtr type) { return nullptr; }

    bool operator==(const TypePtr type) {
        return category() == type->category();
    }

    bool operator!=(const TypePtr type) {
        return category() != type->category();
    }
};

class IntType : public Type {
    IntSizes m_intSize;
public:
    IntType(IntSizes intSize = IntSizes::Int32);

    TypeCategory category();
    IntSizes size();
    bool isConvertibleTo(const TypePtr type);

    TypePtr prefixOperatorResult(Token op);
    TypePtr infixOperatorResult(Token op, const TypePtr type);
};

class DecimalType : public Type {
    TypeCategory m_category;
public:
    DecimalType(TypeCategory decimalSize = TypeCategory::Float);

    TypeCategory category();
    bool isConvertibleTo(const TypePtr type);

    TypePtr prefixOperatorResult(Token op);
    TypePtr infixOperatorResult(Token op, const TypePtr type);
};

class StringType : public Type {
public:
    StringType();

    TypeCategory category();
    bool isConvertibleTo(const TypePtr type);

    TypePtr prefixOperatorResult(Token op);
    TypePtr infixOperatorResult(Token op, const TypePtr type);
};

class BoolType : public Type {
public:
    BoolType();

    TypeCategory category();
    bool isConvertibleTo(const TypePtr type);

    TypePtr prefixOperatorResult(Token op);
    TypePtr infixOperatorResult(Token op, const TypePtr type);
};

class ListType : public Type {
    TypePtr m_baseType;
public:
    ListType(TypePtr baseType);

    TypeCategory category();
    bool isConvertibleTo(const TypePtr type);
};

class UserDefinedType : public Type {
    TypePtr m_baseType;
public:
    UserDefinedType(const TypePtr baseType);

    TypeCategory category();
    TypePtr baseType();
    bool isConvertibleTo(const TypePtr type);
};

//TODO: not ideal
namespace TypeList {
    TypePtr integer8();

    TypePtr integer16();

    TypePtr integer32();

    TypePtr integer64();

    TypePtr boolean();

    TypePtr string();

    TypePtr decimal();
}

extern std::map<std::string, TypePtr> identifierToTypeMap;

#endif