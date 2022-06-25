#ifndef PEREGRINE_TYPES_HPP
#define PEREGRINE_TYPES_HPP

#include "lexer/tokens.hpp"

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ast{
    class AstNode;
    using AstNodePtr = std::shared_ptr<AstNode>;
}

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
    MultipleReturn,
    Enum,
    Void,
    Union
};

class Type;

using TypePtr = std::shared_ptr<Type>;

class Type {
  public:
    virtual ~Type() = default;

    virtual ast::AstNodePtr getTypeAst() const = 0;

    virtual TypeCategory category() const = 0;

    // returns true if the type can be converted to the other IMPLICITLY
    virtual bool isConvertibleTo(const Type& type) const = 0;

    // returns true if the type can be converted to the other EXPLICITLY (by
    // using cast)
    virtual bool isCastableTo(const Type& type) const = 0;

    //some default value for the type
    virtual ast::AstNodePtr defaultValue() const = 0;

    virtual std::string stringify() const { return ""; }

    // returns the type obtained after applying the given operator to this type
    // e.g. -34 -> Integer
    virtual TypePtr prefixOperatorResult(Token op) const { return nullptr; }

    // returns the type obtained after applying the given operator to this type
    // e.g. 34++ -> Integer
    virtual TypePtr postfixOperatorResult(Token op) const { return nullptr; }

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

    IntType(IntSizes intSize = IntSizes::Int64,
            Modifier modifier = Modifier::Signed);

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    IntSizes size() const;
    Modifier modifier() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    std::string stringify() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr postfixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;

    bool operator==(const Type& type) const;

  private:
    IntSizes m_intSize;
    Modifier m_modifier;
};

class DecimalType : public Type {
  public:
    enum DecimalSize { Float32, Float64,Float128 };

    DecimalType(DecimalSize decimalSize = DecimalSize::Float64);

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    DecimalSize size() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    std::string stringify() const;

    bool isFloat() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr postfixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;

    bool operator==(const Type& type) const;

  private:
    DecimalSize m_decimalSize;
};

class StringType : public Type {
  public:
    StringType() = default;

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;
    ast::AstNodePtr defaultValue() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr postfixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
};

class BoolType : public Type {
  public:
    BoolType() = default;

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    std::string stringify() const;
};

class PointerType : public Type {
    TypePtr m_baseType;

  public:
    PointerType(TypePtr baseType);

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    TypePtr baseType() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;
    ast::AstNodePtr defaultValue() const;

    TypePtr prefixOperatorResult(Token op) const;
    TypePtr postfixOperatorResult(Token op) const;
    TypePtr infixOperatorResult(Token op, const TypePtr type) const;
};

class VoidType : public Type {
  public:
    VoidType() = default;

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;
};

class ListType : public Type {
    TypePtr m_elemType;
    std::string m_size;

  public:
    ListType(TypePtr elemType, std::string size);

    ast::AstNodePtr getTypeAst() const;
    ast::AstNodePtr defaultValue() const;
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

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    TypePtr baseType() const;
    bool isConvertibleTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;

    bool operator==(const Type& type) const;
};

class FunctionType : public Type {
    std::vector<TypePtr> m_parameterTypes;
    TypePtr m_returnType;

  public:
    FunctionType(std::vector<TypePtr> parameterTypes, TypePtr returnType);

    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    const std::vector<TypePtr>& parameterTypes() const;
    TypePtr returnType() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    std::string stringify() const;

    bool operator==(const Type& type) const;
};

class MultipleReturnType : public Type {
  public:
    MultipleReturnType(std::vector<TypePtr> returnTypes);
        
    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    std::string stringify() const;
    std::vector<TypePtr> returnTypes() const;

    bool operator==(const Type& type) const;
    private:
    std::vector<TypePtr> m_returnTypes;
};

class EnumType : public Type {
  public:
    EnumType(std::string name,std::vector<std::string> items,std::string curr_value="");
        
    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    ast::AstNodePtr defaultValue() const;
    std::string stringify() const;
    std::vector<std::string> getItem() const;
    std::string getName() const;
    std::string getCurrValue() const;

    bool operator==(const Type& type) const;
    private:
    std::vector<std::string> m_items;
    std::string m_curr_value;
    std::string m_name;
};

class UnionTypeDef : public Type {
  public:
    UnionTypeDef(std::string name,std::map<std::string,TypePtr> items);
        
    ast::AstNodePtr getTypeAst() const;
    TypeCategory category() const;
    bool isConvertibleTo(const Type& type) const;
    bool isCastableTo(const Type& type) const;
    std::string stringify() const;
    std::map<std::string,TypePtr> getItem() const;
    std::string getName() const;
    ast::AstNodePtr defaultValue() const;

    bool operator==(const Type& type) const;
    private:
    std::map<std::string,TypePtr> m_items;
    std::string m_name;
};

class TypeProducer {
    static std::array<TypePtr, 8> m_integer;
    static std::array<TypePtr, 3> m_decimal;

    static TypePtr m_bool;
    static TypePtr m_string;
    static TypePtr m_void;

  public:
    static TypePtr
    integer(IntType::IntSizes intSize = IntType::IntSizes::Int64,
            IntType::Modifier modifier = IntType::Modifier::Signed);
    static TypePtr decimal(
        DecimalType::DecimalSize decimalSize = DecimalType::DecimalSize::Float64);
    static TypePtr string();
    static TypePtr boolean();
    static TypePtr function(std::vector<TypePtr> parameterTypes, TypePtr returnType);
    static TypePtr voidT();
    static TypePtr multipleReturn(std::vector<TypePtr> returnTypes);
    static TypePtr list(TypePtr elemType, std::string size);
    static TypePtr pointer(TypePtr baseType);
    static TypePtr enumT(std::string name,std::vector<std::string> items,std::string curr_value="");
    static TypePtr unionT(std::string name,std::map<std::string,TypePtr> items);
};

extern std::map<std::string, TypePtr> identifierToTypeMap;

} // namespace types

#endif