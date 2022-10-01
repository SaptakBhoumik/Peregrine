//TODO: handle user defined types like `type name=int` properly
//int==user_defined_type
//should be checked properly
#include "types.hpp"
#include "lexer/tokens.hpp"
#include "ast.hpp"

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

TypePtr IntType::postfixOperatorResult(Token op) const{
    return TypeProducer::integer();
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

    if (TokenUtils::isBitwiseToken(op)){
        if(type->category()==TypeCategory::Integer)
            return type;
        else
            return nullptr;
    }
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

ast::AstNodePtr IntType::getTypeAst() const {
    std::string res;
    if(m_modifier==Modifier::Unsigned)
        res = "u";
    else
        res = "i";
    switch (m_intSize) {
        case IntType::Int8:
            res += "8";
            break;
        case IntType::Int16:
            res += "16";
            break;
        case IntType::Int32:
            res += "32";
            break;
        case IntType::Int64:
            if(m_modifier==Modifier::Unsigned)
                res = "uint";
            else
                res = "int";
            break;
    }
    return std::make_shared<ast::TypeExpression>((Token){}, res);
}

ast::AstNodePtr IntType::defaultValue() const {
    return std::make_shared<ast::IntegerLiteral>((Token){.keyword="0",.tkType=tk_integer}, "0");
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
    if(m_decimalSize>typeDecimal.size())
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
    if(m_decimalSize==DecimalSize::Float64)
        return "float";
    else if(m_decimalSize==DecimalSize::Float32)
        return "f32";
    else
        return "f128";
}

ast::AstNodePtr DecimalType::getTypeAst() const {
    std::string res;
    switch (m_decimalSize) {
        case DecimalSize::Float64:
            res += "float";
            break;
        case DecimalSize::Float32:
            res += "f32";
            break;
        case DecimalSize::Float128:
            res += "f128";
            break;
    }
    return std::make_shared<ast::TypeExpression>((Token){}, res);
}

bool DecimalType::isFloat() const {
    return (m_decimalSize == DecimalSize::Float64);
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

TypePtr DecimalType::postfixOperatorResult(Token op) const{
    return TypeProducer::decimal();
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
ast::AstNodePtr DecimalType::defaultValue() const {
    return std::make_shared<ast::DecimalLiteral>((Token){.keyword="0",.tkType=tk_decimal}, "0");
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

TypePtr StringType::postfixOperatorResult(Token op) const{
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
ast::AstNodePtr StringType::getTypeAst() const {
    return std::make_shared<ast::TypeExpression>((Token){}, "str");
}
ast::AstNodePtr StringType::defaultValue() const {
    return std::make_shared<ast::StringLiteral>((Token){.keyword="\"\"",.tkType=tk_string}, "\"\"",false);
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

ast::AstNodePtr BoolType::getTypeAst() const {
    return std::make_shared<ast::TypeExpression>((Token){}, "bool");
}

ast::AstNodePtr BoolType::defaultValue() const {
    return std::make_shared<ast::BoolLiteral>((Token){.keyword="False",.tkType=tk_false}, "False");
}

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
        case TypeCategory::Pointer://TODO:Check if it inherits from the same basetype
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
TypePtr PointerType::postfixOperatorResult(Token op) const{
    return TypeProducer::pointer(m_baseType);
}
// TODO: allow pointer arithmetic as normal binary operations?
TypePtr PointerType::infixOperatorResult(Token op, const TypePtr type) const {
    switch (type->category()) {
        case TypeCategory::Integer:
            break;

        default:
            return nullptr;
    }

    if (op.tkType==tk_plus || op.tkType==tk_minus)
        return TypeProducer::pointer(m_baseType);

    if (TokenUtils::isComparisonToken(op))
        return TypeProducer::boolean();

    return nullptr;
}

ast::AstNodePtr PointerType::getTypeAst() const {
    return std::make_shared<ast::PointerTypeExpr>((Token){}, m_baseType->getTypeAst());
}

ast::AstNodePtr PointerType::defaultValue() const {
    return std::make_shared<ast::NoneLiteral>((Token){.keyword="None",.tkType=tk_none});
}

ReferenceType::ReferenceType(TypePtr baseType) { m_baseType = baseType; }

TypeCategory ReferenceType::category() const { return TypeCategory::Reference; }

TypePtr ReferenceType::baseType() const { return m_baseType; }

bool ReferenceType::isConvertibleTo(const Type& type) const {
    return m_baseType->isConvertibleTo(type);
}

bool ReferenceType::isCastableTo(const Type& type) const {
    return m_baseType->isCastableTo(type);
}

std::string ReferenceType::stringify() const {
    return "&" + m_baseType->stringify();
}

TypePtr ReferenceType::prefixOperatorResult(Token op) const {
    return m_baseType->prefixOperatorResult(op);
}
TypePtr ReferenceType::postfixOperatorResult(Token op) const{
    return m_baseType->postfixOperatorResult(op);
}
TypePtr ReferenceType::infixOperatorResult(Token op, const TypePtr type) const {
    return m_baseType->infixOperatorResult(op, type);
}

ast::AstNodePtr ReferenceType::getTypeAst() const {
    return std::make_shared<ast::RefTypeExpr>((Token){}, m_baseType->getTypeAst());
}

ast::AstNodePtr ReferenceType::defaultValue() const {
    return NULL;
}

TypeCategory VoidType::category() const { return TypeCategory::Void; }

bool VoidType::isConvertibleTo(const Type& type) const { return false; }

bool VoidType::isCastableTo(const Type& type) const { return false; }

std::string VoidType::stringify() const { return "void"; }

ast::AstNodePtr VoidType::getTypeAst() const {
    return std::make_shared<ast::TypeExpression>((Token){}, "void");
}
ast::AstNodePtr VoidType::defaultValue() const {
    return std::make_shared<ast::NoLiteral>();
}
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
        (m_size == listType.size()||listType.size() == "-1"))
        return true;

    return false;
}

ast::AstNodePtr ListType::getTypeAst() const {
    ast::AstNodePtr size;
    if(m_size=="-1"){
        size=std::make_shared<ast::NoLiteral>();
    }
    else{
        size=std::make_shared<ast::IntegerLiteral>((Token){}, m_size);
    }
    return std::make_shared<ast::ListTypeExpr>((Token){}, m_elemType->getTypeAst(),size);
}
ast::AstNodePtr ListType::defaultValue() const {
    return std::make_shared<ast::ListLiteral>((Token){});
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
ast::AstNodePtr UserDefinedType::defaultValue() const {
    return m_baseType->defaultValue();
}
// TODO
std::string UserDefinedType::stringify() const { return m_baseType->stringify(); }

bool UserDefinedType::operator==(const Type& type) const {
    if (m_baseType->operator==(type))
        return true;
    return false;
}

ast::AstNodePtr UserDefinedType::getTypeAst() const {
    return m_baseType->getTypeAst();
}

FunctionType::FunctionType(std::vector<TypePtr> parameterTypes,
                           TypePtr returnType,bool is_user_defined_method) {
    m_parameterTypes = parameterTypes;
    m_returnType = returnType;
    m_is_user_defined_method=is_user_defined_method;
}

TypeCategory FunctionType::category() const {
    return TypeCategory::Function; 
}

bool FunctionType::isMethod() const{
    return m_is_user_defined_method;
}

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
// TODO
ast::AstNodePtr FunctionType::defaultValue() const {return std::make_shared<ast::NoLiteral>();}
std::string FunctionType::stringify() const { return "function"; }

ast::AstNodePtr FunctionType::getTypeAst() const {
    std::vector<ast::AstNodePtr> params;
    for (auto& paramType : m_parameterTypes)
        params.push_back(paramType->getTypeAst());
    return std::make_shared<ast::FunctionTypeExpr>((Token){},params, m_returnType->getTypeAst());
}

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

MultipleReturnType::MultipleReturnType(std::vector<TypePtr> returnTypes) {
    m_returnTypes = returnTypes;
}

ast::AstNodePtr MultipleReturnType::getTypeAst() const{
    std::vector<ast::AstNodePtr> params;
    for (auto& paramType : m_returnTypes){
        if(paramType!=nullptr){
            params.push_back(paramType->getTypeAst());
        }
    }
    return std::make_shared<ast::TypeTuple>(params);
}

TypeCategory MultipleReturnType::category() const{
    return TypeCategory::MultipleReturn;
}

bool MultipleReturnType::isConvertibleTo(const Type& type) const{
    if(type.category()==TypeCategory::MultipleReturn){
        auto& multipleType=dynamic_cast<const MultipleReturnType&>(type);
        if(m_returnTypes.size()!=multipleType.returnTypes().size())
            return false;
        for(size_t i=0;i<m_returnTypes.size();i++){
            if(!m_returnTypes[i]->isConvertibleTo(*multipleType.returnTypes()[i]))
                return false;
        }
        return true;
    }
    return false;
}

bool MultipleReturnType::isCastableTo(const Type& type) const{
    return false;
}

ast::AstNodePtr MultipleReturnType::defaultValue() const{
    //Should never be used
    return NULL;
}
std::string MultipleReturnType::stringify() const{
    std::string ret="(";
    for(size_t i=0;i<m_returnTypes.size();i++){
        ret+=m_returnTypes[i]->stringify();
        if(i<m_returnTypes.size()-1)
            ret+=",";
    }
    ret+=")";
    return ret;
}

std::vector<TypePtr> MultipleReturnType::returnTypes() const{
    return m_returnTypes;
}

bool MultipleReturnType::operator==(const Type& type) const{
    if(type.category()==TypeCategory::MultipleReturn){
        auto& multipleType=dynamic_cast<const MultipleReturnType&>(type);
        if(m_returnTypes.size()!=multipleType.returnTypes().size())
            return false;
        for(size_t i=0;i<m_returnTypes.size();i++){
            if(*m_returnTypes[i]!=*multipleType.returnTypes()[i])
                return false;
        }
        return true;
    }
    return false;
}

EnumType::EnumType(std::string name,std::vector<std::string> items,std::string curr_value){
    m_name=name;
    m_items=items;
    m_curr_value=curr_value;
}

ast::AstNodePtr EnumType::getTypeAst() const{
    return std::make_shared<ast::TypeExpression>((Token){},m_name.c_str());
}

TypeCategory EnumType::category() const{
    return TypeCategory::Enum;
}

bool EnumType::isConvertibleTo(const Type& type) const{
    return false;
}

bool EnumType::isCastableTo(const Type& type) const{
    if (type.category() == TypeCategory::UserDefined) {
        auto& userDefinedType = dynamic_cast<const UserDefinedType&>(type);
        return isCastableTo(*userDefinedType.baseType());
    }
    switch(type.category()){
        case TypeCategory::Integer:
            return true;
        case TypeCategory::Decimal:
            return true;
        case TypeCategory::Bool:
            return true;
        case TypeCategory::Enum:
            return true;
        case TypeCategory::Pointer:
            return true;
        default:
            return false;
    }
    return false;
}

ast::AstNodePtr EnumType::defaultValue() const{
    auto owner=std::make_shared<ast::IdentifierExpression>((Token){},m_name.c_str());
    auto reference=std::make_shared<ast::IdentifierExpression>((Token){},m_items[0].c_str());
    return std::make_shared<ast::DotExpression>((Token){},owner,reference);
}

std::string EnumType::stringify() const{
    std::string res=m_name;
    if(m_curr_value!="")
        res+="."+m_curr_value;
    return res;
}

std::vector<std::string> EnumType::getItem() const{
    return m_items;
}
std::string EnumType::getName() const{
    return m_name;
}
std::string EnumType::getCurrValue() const{
    return m_curr_value;
}

bool EnumType::operator==(const Type& type) const{
    if(type.category()==TypeCategory::Enum){
        auto& enumType=dynamic_cast<const EnumType&>(type);
        auto item=enumType.getItem();
        if(m_name==enumType.getName()&&item.size()==m_items.size()){
            for(size_t i=0; i<m_items.size();++i){
                if(m_items[i]!=item[i]){
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

UnionTypeDef::UnionTypeDef(std::string name,std::map<std::string,TypePtr> items){
    m_name=name;
    m_items=items;
}

ast::AstNodePtr UnionTypeDef::getTypeAst() const{
    return std::make_shared<ast::TypeExpression>((Token){},m_name.c_str());
}

TypeCategory UnionTypeDef::category() const{
    return TypeCategory::Union;
}

bool UnionTypeDef::isConvertibleTo(const Type& type) const{
    return false;
}

bool UnionTypeDef::isCastableTo(const Type& type) const{
    switch(type.category()){
        case TypeCategory::Pointer:
            return true;
        default:
            return false;
    }
}

std::string UnionTypeDef::stringify() const{
    return m_name;
}

std::map<std::string,TypePtr> UnionTypeDef::getItem() const{
    return m_items;
}

std::string UnionTypeDef::getName() const{
    return m_name;
}

bool UnionTypeDef::operator==(const Type& type) const{
    if(type.category() == TypeCategory::Union){
        auto& unionType=dynamic_cast<const UnionTypeDef&>(type);
        auto items=unionType.getItem();
        if(m_name==unionType.getName()&&items.size()==m_items.size()){
            for(auto& item : items){
                if(m_items.contains(item.first)){
                    auto pos = m_items.find(item.first);
                    if(pos->second!=item.second){
                        return false;
                    }
                }
                else{
                    return false;
                }
            }
        }
    }
    return false;
}

ast::AstNodePtr UnionTypeDef::defaultValue() const{
    return std::make_shared<ast::NoLiteral>();
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

std::array<TypePtr, 3> TypeProducer::m_decimal = {
    std::make_shared<DecimalType>(DecimalType::DecimalSize::Float64),
    std::make_shared<DecimalType>(DecimalType::DecimalSize::Float32),
    std::make_shared<DecimalType>(DecimalType::DecimalSize::Float128)};

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

TypePtr TypeProducer::function(std::vector<TypePtr> parameterTypes, TypePtr returnType){
    return std::make_shared<FunctionType>(parameterTypes, returnType);
}

TypePtr TypeProducer::pointer(TypePtr baseType) {
    return std::make_shared<PointerType>(baseType);
}
TypePtr TypeProducer::multipleReturn(std::vector<TypePtr> returnTypes){
    return std::make_shared<MultipleReturnType>(returnTypes);
}
TypePtr TypeProducer::enumT(std::string name,std::vector<std::string> items,std::string curr_value){
    return std::make_shared<EnumType>(name,items,curr_value);
}
TypePtr TypeProducer::unionT(std::string name,std::map<std::string,TypePtr> items){
    return std::make_shared<UnionTypeDef>(name,items);
}
std::map<std::string, TypePtr> identifierToTypeMap = {
    {"i8", TypeProducer::integer(IntType::IntSizes::Int8)},
    {"i16", TypeProducer::integer(IntType::IntSizes::Int16)},
    {"i32", TypeProducer::integer()},
    {"int", TypeProducer::integer()},
    {"u8", TypeProducer::integer(IntType::IntSizes::Int8,
                                 IntType::Modifier::Unsigned)},
    {"u16", TypeProducer::integer(IntType::IntSizes::Int16,
                                  IntType::Modifier::Unsigned)},
    {"u32", TypeProducer::integer(IntType::IntSizes::Int32,
                                  IntType::Modifier::Unsigned)},
    {"uint", TypeProducer::integer(IntType::IntSizes::Int32,
                                   IntType::Modifier::Unsigned)},
    {"float", TypeProducer::decimal()},
    {"f32", TypeProducer::decimal(DecimalType::DecimalSize::Float32)},
    {"f128", TypeProducer::decimal(DecimalType::DecimalSize::Float128)},
    {"str", TypeProducer::string()},
    {"bool", TypeProducer::boolean()},
    {"void", TypeProducer::voidT()}};

} // namespace types