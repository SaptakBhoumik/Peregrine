#ifndef PEREGRINE_TYPES_HPP
#define PEREGRINE_TYPES_HPP

#include "lexer/tokens.hpp"

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace types {

enum TypeCategory {
    Integer,
    Decimal,
    String,
    Bool,
    Pointer,
    List,
    Dict,
    UserDefined,
    Function,
    Class,
    Void
};

class Type;

using TypePtr = std::shared_ptr<Type>;

class Type {
  public:
    virtual ~Type() = default;

    virtual TypeCategory category() const = 0;

    // returns true if the type can be converted to the other IMPLICITLY
    virtual bool isConvertibleTo(const Type& type) const = 0;

    // returns true if the type can be converted to the other EXPLICITLY (by
    // using cast)
    virtual bool isCastableTo(const Type& type) const = 0;

    virtual std::string stringify() const { return ""; }

    // returns the type obtained after applying the given operator to this type
    // e.g. -34 -> Integer
    virtual TypePtr prefixOperatorResult(Token op) const { return nullptr; }

    // returns the type obtained after applying the given operator to both types
    // e.g. false == false -> Bool
    virtual TypePtr infixOperatorResult(Token op, const TypePtr type) const {
        return nullptr;
    }

    virtual bool operator==(const Type& type) const {
        return category() == type.category();
    }

    bool operator!=(const Type& type) const { return !operator==(type); }
};

class IntType : public Type {
  public:
    enum IntSizes { Int8, Int16, Int32, Int64 };

    enum class Modifier { Signed, Unsigned };

    IntType(IntSizes intSize = IntSizes::Int32,
            Modifier modifier = Modifier::Signed);

    TypeCategory category() const;
    IntSizes size() const;
    Modifier modifier() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;

    bool operator==(const Type& type) const;

  private:
    IntSizes m_intSize;
    Modifier m_modifier;
};

class DecimalType : public Type {
  public:
    enum DecimalSize { Float, Double };

    DecimalType(DecimalSize decimalSize = DecimalSize::Float);

    TypeCategory category() const;
    DecimalSize size() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    bool isFloat() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;

    bool operator==(const Type& type) const;

  private:
    DecimalSize m_decimalSize;
};

class StringType : public Type {
  public:
    StringType() = default;

    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
};

class BoolType : public Type {
  public:
    BoolType() = default;

    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;
};

class PointerType : public Type {
    TypePtr m_baseType;

  public:
    PointerType(TypePtr baseType);

    TypeCategory category() const;
    TypePtr baseType() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
};

class VoidType : public Type {
  public:
    VoidType() = default;

    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;
};

class ListType : public Type {
    TypePtr m_elemType;
    std::string m_size;

  public:
    ListType(TypePtr elemType, std::string size);

    TypeCategory category() const;
    TypePtr elemType() const;
    std::string size() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    bool operator==(const Type& type) const;
};

class UserDefinedType : public Type {
    TypePtr m_baseType;

  public:
    UserDefinedType(TypePtr baseType);

    TypeCategory category() const;
    TypePtr baseType() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    bool operator==(const Type& type) const;
};

class FunctionType : public Type {
    std::vector<TypePtr> m_parameterTypes;
    TypePtr m_returnType;

  public:
    FunctionType(std::vector<TypePtr> parameterTypes, TypePtr returnType);

    TypeCategory category() const;
    const std::vector<TypePtr>& parameterTypes() const;
    TypePtr returnType() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    bool operator==(const Type& type) const;
};

class TypeProducer {
    static std::array<TypePtr, 8> m_integer;
    static std::array<TypePtr, 2> m_decimal;

    static TypePtr m_bool;
    static TypePtr m_string;
    static TypePtr m_void;

  public:
    static TypePtr
    integer(IntType::IntSizes intSize = IntType::IntSizes::Int32,
            IntType::Modifier modifier = IntType::Modifier::Signed);
    static TypePtr decimal(
        DecimalType::DecimalSize decimalSize = DecimalType::DecimalSize::Float);
    static TypePtr string();
    static TypePtr boolean();
    static TypePtr voidT();

    static TypePtr list(TypePtr elemType, std::string size);
    static TypePtr pointer(TypePtr baseType);
};

extern std::map<std::string, TypePtr> identifierToTypeMap;

} // namespace types

#endif