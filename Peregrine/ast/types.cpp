#include "types.hpp"
#include "lexer/tokens.hpp"

#include <memory>

IntType::IntType(IntSizes intSize, Modifier modifier) {
    m_intSize = intSize;
    m_modifier = modifier;
}

TypeCategory IntType::category() const {
    return TypeCategory::Integer;
}

IntType::IntSizes IntType::size() const {
    return m_intSize;
}

IntType::Modifier IntType::modifier() const {
    return m_modifier;
}

bool IntType::isConvertibleTo(const TypePtr type) const {
    switch (type->category()) {
        case TypeCategory::Integer:
        case TypeCategory::Decimal:
            break;

        default:
            return false;
    }

    if (type->category() == TypeCategory::Integer) {
        auto typeInt = std::dynamic_pointer_cast<IntType>(type);

        // an Int32 can't be converted to a Int8
        if (m_intSize > typeInt->size()) 
            return false;
    } else {
        auto typeDecimal = std::dynamic_pointer_cast<DecimalType>(type);

        // if the integer has a value of 64 bits, it can only fit in doubles
        if (m_intSize == IntType::Int64 && typeDecimal->isFloat())
            return false;
    }

    return true;
} 

std::string IntType::stringify() const {
    return "integer";
}

//TODO: unsigned ints
TypePtr IntType::prefixOperatorResult(Token op) const {
    switch (op.tkType) {
        case tk_not:
            return TypeList::boolean();

        case tk_minus:
        case tk_bit_not:
            return TypeList::integer32(); //no

        default:
            return nullptr;
    }
}

TypePtr IntType::infixOperatorResult(Token op, const TypePtr type) const {
    switch (type->category()) {
        case TypeCategory::Integer:
        case TypeCategory::Decimal:
            break;

        default:
            return nullptr;
    }

    if (TokenUtils::isArithmeticToken(op)) 
        return type;

    if (TokenUtils::isComparisonToken(op)) 
        return TypeList::boolean();

    //TODO: handle bitwise operations
    return nullptr;
}

DecimalType::DecimalType(DecimalSize decimalSize) {
    m_decimalSize = decimalSize;
}

TypeCategory DecimalType::category() const {
    return TypeCategory::Decimal;
}

DecimalType::DecimalSize DecimalType::size() const {
    return m_decimalSize;
}

bool DecimalType::isConvertibleTo(const TypePtr type) const {
    if (type->category() != TypeCategory::Decimal)
        return false;

    auto typeDecimal = std::dynamic_pointer_cast<DecimalType>(type);
    
    if (!isFloat() && typeDecimal->isFloat()) 
        return false;

    return true;
}

std::string DecimalType::stringify() const {
    return (isFloat()) ? "float" : "double";
}

bool DecimalType::isFloat() const {
    return (m_decimalSize == DecimalSize::Float) ? true : false;
}

TypePtr DecimalType::prefixOperatorResult(Token op) const {
    switch (op.tkType) {
        case tk_not:
            return TypeList::boolean();

        case tk_minus:
            return TypeList::decimal(); //no

        default:
            return nullptr;
    }
}

TypePtr DecimalType::infixOperatorResult(Token op, const TypePtr type) const {
    switch (type->category()) {
        case TypeCategory::Integer:
        case TypeCategory::Decimal:
            break;
    
        default:
            return nullptr;
    }

    if (TokenUtils::isArithmeticToken(op)) 
        return TypeList::decimal(); //TODO: improve

    if (TokenUtils::isComparisonToken(op))
        return TypeList::boolean();

    return nullptr;
}

StringType::StringType() {
}

TypeCategory StringType::category() const {
    return TypeCategory::String;
}

bool StringType::isConvertibleTo(const TypePtr type) const {
    return (type->category() != TypeCategory::String) ? false : true;
}

std::string StringType::stringify() const {
    return "string";
}

TypePtr StringType::prefixOperatorResult(Token op) const {
    if (op.tkType == tk_not)
        return TypeList::boolean();

    return nullptr;
}

TypePtr StringType::infixOperatorResult(Token op, const TypePtr type) const {
    if (type->category() != TypeCategory::String)
        return nullptr;

    switch (op.tkType) {
        case tk_plus: 
            return type;

        case tk_equal:
        case tk_not_equal:
            return TypeList::boolean();
        
        default:
            return nullptr;
    }
}

BoolType::BoolType() {
}

TypeCategory BoolType::category() const {
    return TypeCategory::Bool;
}

bool BoolType::isConvertibleTo(const TypePtr type) const {
    return (type->category() == TypeCategory::Bool) ? true : false;
}

std::string BoolType::stringify() const {
    return "bool";
}

TypePtr BoolType::prefixOperatorResult(Token op) const {
    return nullptr;
}   

TypePtr BoolType::infixOperatorResult(Token op, const TypePtr type) const {
    return nullptr;
}

ListType::ListType(TypePtr baseType) {
    m_baseType = baseType;
}

TypeCategory ListType::category() const {
    return TypeCategory::List;
}

bool ListType::isConvertibleTo(const TypePtr type) const {
    return false;
}

std::string ListType::stringify() const {
    return "";
}

UserDefinedType::UserDefinedType(TypePtr baseType) {
    m_baseType = baseType;
}

TypeCategory UserDefinedType::category() const {
    return TypeCategory::UserDefined;
}

bool UserDefinedType::isConvertibleTo(const TypePtr type) const {
    return m_baseType->isConvertibleTo(type);
}

std::string UserDefinedType::stringify() const {
    return "";
}

FunctionType::FunctionType(std::vector<TypePtr> parameterTypes, TypePtr returnType) {
    m_parameterTypes = parameterTypes;
    m_returnType = returnType;
}

TypeCategory FunctionType::category() const {
    return TypeCategory::Function;
}

std::vector<TypePtr> FunctionType::parameterTypes() const {
    return m_parameterTypes;
}

TypePtr FunctionType::returnType() const {
    return m_returnType;
}

bool FunctionType::isConvertibleTo(const TypePtr type) const {
    return false;
}

std::string FunctionType::stringify() const {
    return "function";
}

std::map<std::string, TypePtr> identifierToTypeMap = {
    { "i8", std::make_shared<IntType>(IntType::IntSizes::Int8) }, 
    { "i16", std::make_shared<IntType>(IntType::IntSizes::Int16) }, 
    { "i32", std::make_shared<IntType>() }, 
    { "int", std::make_shared<IntType>() }, 
    { "i64", std::make_shared<IntType>(IntType::IntSizes::Int64) }, 
    { "str", std::make_shared<StringType>() },
    { "bool", std::make_shared<BoolType>() },
};

//TODO: not ideal
namespace TypeList {
    TypePtr integer8() {
        return std::make_shared<IntType>(IntType::IntSizes::Int8);
    }

    TypePtr integer16() {
        return std::make_shared<IntType>(IntType::IntSizes::Int16);
    }

    TypePtr integer32() {
        return std::make_shared<IntType>(IntType::IntSizes::Int32);
    }

    TypePtr integer64() {
        return std::make_shared<IntType>(IntType::IntSizes::Int64);
    }

    TypePtr boolean() {
        return std::make_shared<BoolType>();
    }

    TypePtr string() {
        return std::make_shared<StringType>();
    }

    TypePtr decimal() {
        return std::make_shared<DecimalType>(DecimalType::DecimalSize::Float);
    }
}
