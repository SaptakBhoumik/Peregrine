#include "types.hpp"
#include "lexer/tokens.hpp"

#include <memory>

IntType::IntType(IntSizes intSize) {
    m_intSize = intSize;
}

TypeCategory IntType::category() {
    return TypeCategory::Integer;
}

IntSizes IntType::size() {
    return m_intSize;
}

bool IntType::isConvertibleTo(const TypePtr type) {
    switch (type->category()) {
        case TypeCategory::Integer:
        case TypeCategory::Float:
        case TypeCategory::Double:
            break;

        default:
            return false;
    }

    if (type->category() == TypeCategory::Integer) {
        auto typeInt = std::dynamic_pointer_cast<IntType>(type);

        //TODO: check to see if value is actually smaller than the max size
        if (typeInt->size() < size()) 
            return false;
    }

    //TODO: check if it fits in a float or double

    return true;
} 

//TODO: unsigned ints
TypePtr IntType::prefixOperatorResult(Token op) {
    switch (op.tkType) {
        case tk_not:
            return TypeList::boolean();

        case tk_minus:
        case tk_bit_not:
            return TypeList::integer32();

        default:
            return nullptr;
    }
}

TypePtr IntType::infixOperatorResult(Token op, const TypePtr type) {
    switch (type->category()) {
        case TypeCategory::Integer:
        case TypeCategory::Float:
        case TypeCategory::Double:
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

DecimalType::DecimalType(TypeCategory decimalSize) {
    m_category = decimalSize;
}

TypeCategory DecimalType::category() {
    return m_category;
}

bool DecimalType::isConvertibleTo(const TypePtr type) {

}

TypePtr DecimalType::prefixOperatorResult(Token op) {
    
}

TypePtr DecimalType::infixOperatorResult(Token op, const TypePtr type) {

}

StringType::StringType() {
}

TypeCategory StringType::category() {
    return TypeCategory::String;
}

bool StringType::isConvertibleTo(const TypePtr type) {
    switch (type->category()) {
        //TODO: check to see if its user defined
        default:
            return false;
    }
}

TypePtr StringType::prefixOperatorResult(Token op) {

}

TypePtr StringType::infixOperatorResult(Token op, const TypePtr type) {

}

BoolType::BoolType() {
}

TypeCategory BoolType::category() {
    return TypeCategory::Bool;
}

bool BoolType::isConvertibleTo(const TypePtr type) {
    return (type->category() == TypeCategory::Bool) ? true : false;
}

TypePtr BoolType::prefixOperatorResult(Token op) {

}

TypePtr BoolType::infixOperatorResult(Token op, const TypePtr type) {

}

ListType::ListType(TypePtr baseType) {
    m_baseType = baseType;
}

TypeCategory ListType::category() {
    return TypeCategory::List;
}

bool ListType::isConvertibleTo(const TypePtr type) {

}

UserDefinedType::UserDefinedType(TypePtr baseType) {
    m_baseType = baseType;
}

TypeCategory UserDefinedType::category() {
    return TypeCategory::UserDefined;
}

bool UserDefinedType::isConvertibleTo(const TypePtr type) {
    return m_baseType->isConvertibleTo(type);
}

std::map<std::string, TypePtr> identifierToTypeMap = {
    { "i8", std::make_shared<IntType>(IntSizes::Int8) }, 
    { "i16", std::make_shared<IntType>(IntSizes::Int16) }, 
    { "i32", std::make_shared<IntType>() }, 
    { "int", std::make_shared<IntType>() }, 
    { "i64", std::make_shared<IntType>(IntSizes::Int64) }, 
    { "str", std::make_shared<StringType>() },
    { "bool", std::make_shared<BoolType>() },
};


//TODO: not ideal
namespace TypeList {
    TypePtr integer8() {
        return std::make_shared<IntType>(IntSizes::Int8);
    }

    TypePtr integer16() {
        return std::make_shared<IntType>(IntSizes::Int16);
    }

    TypePtr integer32() {
        return std::make_shared<IntType>(IntSizes::Int32);
    }

    TypePtr integer64() {
        return std::make_shared<IntType>(IntSizes::Int64);
    }

    TypePtr boolean() {
        return std::make_shared<BoolType>();
    }

    TypePtr string() {
        return std::make_shared<StringType>();
    }

    TypePtr decimal() {
        return std::make_shared<DecimalType>(TypeCategory::Float);
    }
}
