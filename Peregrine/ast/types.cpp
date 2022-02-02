#include "types.hpp"
#include "lexer/tokens.hpp"

#include <memory>

namespace types {

IntType::IntType(IntSizes intSize, Modifier modifier) {
    m_intSize = intSize;
    m_modifier = modifier;
}

TypeCategory IntType::category() const { return TypeCategory::Integer; }

IntType::IntSizes IntType::size() const { return m_intSize; }

IntType::Modifier IntType::modifier() const { return m_modifier; }

bool IntType::isConvertibleTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::Integer:
        case TypeCategory::Decimal:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::Integer) {
        auto& typeInt = dynamic_cast<const IntType&>(type);

        // an Int32 can't be converted to a Int8
        if (m_intSize > typeInt.size())
            return false;
    } else {
        auto& typeDecimal = dynamic_cast<const DecimalType&>(type);

        // if the integer has a value of 64 bits, it can only fit in doubles
        if (m_intSize == IntType::Int64 && typeDecimal.isFloat())
            return false;
    }

    return true;
}

bool IntType::isCastableTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::Integer:
        case TypeCategory::Decimal:
        case TypeCategory::Pointer:
        case TypeCategory::Bool:
        case TypeCategory::UserDefined:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);

        return isCastableTo(*userDefinedType.baseType());
    }

    return true;
}

std::string IntType::stringify() const { return "integer"; }

// TODO: unsigned ints
TypePtr IntType::prefixOperatorResult(Token op) const {
    switch (op.tkType) {
        case tk_not:
            return TypeProducer::boolean();

        case tk_minus:
        case tk_bit_not:
            return TypeProducer::integer(); // no

        case tk_ampersand:
            return TypeProducer::pointer(std::make_shared<IntType>(*this));

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
        return TypeProducer::boolean();

    // TODO: handle bitwise operations
    return nullptr;
}

bool IntType::operator==(const Type& type) const {
    if (type.category() == TypeCategory::Integer) {
        auto& intType = dynamic_cast<const IntType&>(type);
        if (intType.size() == m_intSize && intType.modifier() == m_modifier)
            return true;
    }

    return false;
}

DecimalType::DecimalType(DecimalSize decimalSize) {
    m_decimalSize = decimalSize;
}

TypeCategory DecimalType::category() const { return TypeCategory::Decimal; }

DecimalType::DecimalSize DecimalType::size() const { return m_decimalSize; }

bool DecimalType::isConvertibleTo(const Type& type) const {
    if (type.category() != TypeCategory::Decimal)
        return false;

    auto& typeDecimal = dynamic_cast<const DecimalType&>(type);

    if (!isFloat() && typeDecimal.isFloat())
        return false;

    return true;
}

bool DecimalType::isCastableTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::Integer:
        case TypeCategory::Decimal:
        case TypeCategory::Bool:
        case TypeCategory::UserDefined:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);

        return isCastableTo(*userDefinedType.baseType());
    }

    return true;
}

std::string DecimalType::stringify() const {
    return (isFloat()) ? "float" : "double";
}

bool DecimalType::isFloat() const {
    return (m_decimalSize == DecimalSize::Float);
}

TypePtr DecimalType::prefixOperatorResult(Token op) const {
    switch (op.tkType) {
        case tk_not:
            return TypeProducer::boolean();

        case tk_minus:
            return TypeProducer::decimal(); // no

        case tk_ampersand:
            return TypeProducer::pointer(std::make_shared<DecimalType>(*this));

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
        return TypeProducer::decimal();

    if (TokenUtils::isComparisonToken(op))
        return TypeProducer::boolean();

    return nullptr;
}

bool DecimalType::operator==(const Type& type) const {
    if (type.category() == TypeCategory::Decimal) {
        auto& decimalType = dynamic_cast<const DecimalType&>(type);
        if (decimalType.size() == size())
            return true;
    }

    return false;
}

TypeCategory StringType::category() const { return TypeCategory::String; }

bool StringType::isConvertibleTo(const Type& type) const {
    return (type.category() == TypeCategory::String);
}

bool StringType::isCastableTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::UserDefined:
        case TypeCategory::String:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);

        return isCastableTo(*userDefinedType.baseType());
    }

    return true;
}

std::string StringType::stringify() const { return "string"; }

TypePtr StringType::prefixOperatorResult(Token op) const {
    if (op.tkType == tk_not)
        return TypeProducer::boolean();

    // TODO: should we implement it like that?
    //  if (op.tkType == tk_ampersand) {
    //      return TypeProducer::pointer(std::make_shared<IntType>(*this));
    //  }

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
            return TypeProducer::boolean();

        default:
            return nullptr;
    }
}

TypeCategory BoolType::category() const { return TypeCategory::Bool; }

bool BoolType::isConvertibleTo(const Type& type) const {
    return (type.category() == TypeCategory::Bool);
}

bool BoolType::isCastableTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::UserDefined:
        case TypeCategory::Integer:
        case TypeCategory::Bool:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);

        return isCastableTo(*userDefinedType.baseType());
    }

    return true;
}

std::string BoolType::stringify() const { return "bool"; }

PointerType::PointerType(TypePtr baseType) { m_baseType = baseType; }

TypeCategory PointerType::category() const { return TypeCategory::Pointer; }

TypePtr PointerType::baseType() const { return m_baseType; }

bool PointerType::isConvertibleTo(const Type& type) const {
    if (type.category() != TypeCategory::Pointer)
        return false;

    auto pointerType = dynamic_cast<const PointerType&>(type);
    if (m_baseType->isConvertibleTo(*pointerType.baseType()))
        return true;

    return false;
}

bool PointerType::isCastableTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::UserDefined:
        case TypeCategory::Integer:
        case TypeCategory::Pointer:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);

        return isCastableTo(*userDefinedType.baseType());
    }

    return true;
}

std::string PointerType::stringify() const {
    return "*" + m_baseType->stringify();
}

TypePtr PointerType::prefixOperatorResult(Token op) const {
    switch (op.tkType) {
        case tk_multiply:
            return m_baseType;

        case tk_ampersand:
            return TypeProducer::pointer(std::make_shared<PointerType>(*this));

        default:
            return nullptr;
    }
}

// TODO: allow pointer arithmetic as normal binary operations?
TypePtr PointerType::infixOperatorResult(Token op, const TypePtr type) const {
    return nullptr;
}

TypeCategory VoidType::category() const { return TypeCategory::Void; }

bool VoidType::isConvertibleTo(const Type& type) const { return false; }

bool VoidType::isCastableTo(const Type& type) const { return false; }

std::string VoidType::stringify() const { return "void"; }

ListType::ListType(TypePtr elemType, std::string size) {
    m_elemType = elemType;
    m_size = size;
}

TypeCategory ListType::category() const { return TypeCategory::List; }

TypePtr ListType::elemType() const { return m_elemType; }

std::string ListType::size() const { return m_size; }

bool ListType::isConvertibleTo(const Type& type) const {
    if (type.category() != TypeCategory::List)
        return false;

    auto listType = dynamic_cast<const ListType&>(type);
    if (m_elemType->isConvertibleTo(*listType.elemType()) &&
        m_size == listType.size())
        return true;

    return false;
}

bool ListType::isCastableTo(const Type& type) const {
    switch (type.category()) {
        case TypeCategory::UserDefined:
        case TypeCategory::List:
            break;

        default:
            return false;
    }

    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);

        return isCastableTo(*userDefinedType.baseType());
    }

    return true;
}

std::string ListType::stringify() const {
    return "[" + m_size + "]" + m_elemType->stringify();
}

bool ListType::operator==(const Type& type) const {
    if (type.category() != TypeCategory::List)
        return false;

    auto listType = dynamic_cast<const ListType&>(type);
    if (m_elemType->operator==(*listType.elemType()) &&
        m_size == listType.size())
        return true;

    return false;
}

UserDefinedType::UserDefinedType(TypePtr baseType) { m_baseType = baseType; }

TypeCategory UserDefinedType::category() const {
    return TypeCategory::UserDefined;
}

TypePtr UserDefinedType::baseType() const { return m_baseType; }

bool UserDefinedType::isConvertibleTo(const Type& type) const {
    return m_baseType->isConvertibleTo(type);
}

bool UserDefinedType::isCastableTo(const Type& type) const {
    return m_baseType->isCastableTo(type);
}

// TODO
std::string UserDefinedType::stringify() const { return ""; }

bool UserDefinedType::operator==(const Type& type) const {
    if (m_baseType->operator==(type))
        return true;
    return false;
}

FunctionType::FunctionType(std::vector<TypePtr> parameterTypes,
                           TypePtr returnType) {
    m_parameterTypes = parameterTypes;
    m_returnType = returnType;
}

TypeCategory FunctionType::category() const { return TypeCategory::Function; }

const std::vector<TypePtr>& FunctionType::parameterTypes() const {
    return m_parameterTypes;
}

TypePtr FunctionType::returnType() const { return m_returnType; }

bool FunctionType::isConvertibleTo(const Type& type) const {
    if (type.category() != TypeCategory::Function)
        return false;

    auto& funcType = dynamic_cast<const FunctionType&>(type);

    if (!m_returnType->isConvertibleTo(*funcType.returnType()) &&
        !funcType.returnType()->isConvertibleTo(*m_returnType))
        return false;

    for (size_t i = 0; i < m_parameterTypes.size(); i++) {
        auto& thisParamType = *m_parameterTypes[i];
        auto& thatParamType = *funcType.parameterTypes()[i];

        if (!thisParamType.isConvertibleTo(thatParamType) &&
            !thatParamType.isConvertibleTo(thisParamType))
            return false;
    }

    return true;
}

// TODO
bool FunctionType::isCastableTo(const Type& type) const { return false; }

std::string FunctionType::stringify() const { return "function"; }

bool FunctionType::operator==(const Type& type) const {
    if (type.category() != TypeCategory::Function)
        return false;

    auto& funcType = dynamic_cast<const FunctionType&>(type);
    if (funcType.returnType()->operator!=(*m_returnType) ||
        funcType.parameterTypes().size() != m_parameterTypes.size())
        return false;

    for (size_t i = 0; i < m_parameterTypes.size(); i++) {
        if (*m_parameterTypes[i] != *funcType.parameterTypes()[i])
            return false;
    }

    return true;
}

std::array<TypePtr, 8> TypeProducer::m_integer = {
    std::make_shared<IntType>(IntType::IntSizes::Int8),
    std::make_shared<IntType>(IntType::IntSizes::Int16),
    std::make_shared<IntType>(IntType::IntSizes::Int32),
    std::make_shared<IntType>(IntType::IntSizes::Int64),
    std::make_shared<IntType>(IntType::IntSizes::Int8,
                              IntType::Modifier::Unsigned),
    std::make_shared<IntType>(IntType::IntSizes::Int16,
                              IntType::Modifier::Unsigned),
    std::make_shared<IntType>(IntType::IntSizes::Int32,
                              IntType::Modifier::Unsigned),
    std::make_shared<IntType>(IntType::IntSizes::Int64,
                              IntType::Modifier::Unsigned)};

std::array<TypePtr, 2> TypeProducer::m_decimal = {
    std::make_shared<DecimalType>(DecimalType::DecimalSize::Float),
    std::make_shared<DecimalType>(DecimalType::DecimalSize::Double)};

TypePtr TypeProducer::m_bool = std::make_shared<BoolType>();
TypePtr TypeProducer::m_string = std::make_shared<StringType>();
TypePtr TypeProducer::m_void = std::make_shared<VoidType>();

TypePtr TypeProducer::integer(IntType::IntSizes intSize,
                              IntType::Modifier modifier) {
    if (modifier == IntType::Modifier::Signed) {
        return m_integer[intSize];
    }

    return m_integer[intSize + 4];
}

TypePtr TypeProducer::decimal(DecimalType::DecimalSize decimalSize) {
    return m_decimal[decimalSize];
}

TypePtr TypeProducer::boolean() { return m_bool; }

TypePtr TypeProducer::string() { return m_string; }

TypePtr TypeProducer::voidT() { return m_void; }

TypePtr TypeProducer::list(TypePtr elemType, std::string size) {
    return std::make_shared<ListType>(elemType, size);
}

TypePtr TypeProducer::pointer(TypePtr baseType) {
    return std::make_shared<PointerType>(baseType);
}

std::map<std::string, TypePtr> identifierToTypeMap = {
    {"i8", TypeProducer::integer(IntType::IntSizes::Int8)},
    {"i16", TypeProducer::integer(IntType::IntSizes::Int16)},
    {"i32", TypeProducer::integer()},
    {"int", TypeProducer::integer()},
    {"i64", TypeProducer::integer(IntType::IntSizes::Int64)},
    {"u8", TypeProducer::integer(IntType::IntSizes::Int8,
                                 IntType::Modifier::Unsigned)},
    {"u16", TypeProducer::integer(IntType::IntSizes::Int16,
                                  IntType::Modifier::Unsigned)},
    {"u32", TypeProducer::integer(IntType::IntSizes::Int32,
                                  IntType::Modifier::Unsigned)},
    {"uint", TypeProducer::integer(IntType::IntSizes::Int32,
                                   IntType::Modifier::Unsigned)},
    {"u64", TypeProducer::integer(IntType::IntSizes::Int64,
                                  IntType::Modifier::Unsigned)},
    {"float", TypeProducer::decimal()},
    {"double", TypeProducer::decimal(DecimalType::DecimalSize::Double)},
    {"str", TypeProducer::string()},
    {"bool", TypeProducer::boolean()},
    {"void", TypeProducer::voidT()}};

} // namespace types