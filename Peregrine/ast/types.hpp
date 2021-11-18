#ifndef PEREGRINE_TYPES_HPP
#define PEREGRINE_TYPES_HPP

#include "ast.hpp"
#include "lexer/tokens.hpp"

#include <map>
#include <string>
#include <vector>
#include <memory>

enum class TypeCategory {
    Integer, Decimal, String,
    Bool, List, Dict, 
    UserDefined, Function, Class,
    None
};

class Type;

typedef std::shared_ptr<Type> TypePtr;

class Type {
public:
    virtual TypeCategory category() const = 0;

    // returns true if the type can be converted to the other IMPLICITLY
    virtual bool isConvertibleTo(const TypePtr type) const = 0;

    virtual std::string stringify() const { return ""; }

    // returns the type obtained after applying the given operator to this type
    // e.g. -34 -> Integer
    virtual TypePtr prefixOperatorResult(Token op) const { return nullptr; }

    // returns the type obtained after applying the given operator to both types
    // e.g. false == false -> Bool
    virtual TypePtr infixOperatorResult(Token op, const TypePtr type) const { return nullptr; }

    bool operator==(const TypePtr type) {
        return category() == type->category();
    }

    bool operator!=(const TypePtr type) {
        return category() != type->category();
    }
};

class IntType : public Type {
public:
    enum IntSizes {
        Int8,
        Int16,
        Int32,
        Int64
    };

    enum class Modifier {
        Signed,
        Unsigned
    };

    IntType(IntSizes intSize = IntSizes::Int32, Modifier modifier = Modifier::Signed);

    TypeCategory category() const;
    IntSizes size() const;
    Modifier modifier() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
private:
    IntSizes m_intSize;
    Modifier m_modifier;
};

class DecimalType : public Type {
public:
    enum class DecimalSize {
        Float,
        Double
    };

    DecimalType(DecimalSize decimalSize = DecimalSize::Float);

    TypeCategory category() const;
    DecimalSize size() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;

    bool isFloat() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
private:
    DecimalSize m_decimalSize;
};

class StringType : public Type {
public:
    StringType();

    TypeCategory category() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
};

class BoolType : public Type {
public:
    BoolType();

    TypeCategory category() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
};

class NoneType : public Type {
public:
    NoneType();

    TypeCategory category() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;
};

class ListType : public Type {
    TypePtr m_baseType;
public:
    ListType(TypePtr baseType);

    TypeCategory category() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;
};

class UserDefinedType : public Type {
    TypePtr m_baseType;
public:
    UserDefinedType(const TypePtr baseType);

    TypeCategory category() const;
    TypePtr baseType() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;
};

class FunctionType : public Type {
    std::vector<TypePtr> m_parameterTypes;
    TypePtr m_returnType;
public:
    FunctionType(std::vector<TypePtr> parameterTypes, TypePtr returnType);

    TypeCategory category() const;
    std::vector<TypePtr> parameterTypes() const;
    TypePtr returnType() const;
    bool isConvertibleTo(const TypePtr type) const;
    std::string stringify() const;
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

    TypePtr none();
}

extern std::map<std::string, TypePtr> identifierToTypeMap;

#endif