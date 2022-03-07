#ifndef PEREGRINE_PARSER_HPP
#define PEREGRINE_PARSER_HPP

#include "ast/ast.hpp"
#include "errors/error.hpp"
#include "lexer/lexer.hpp"
#include "lexer/tokens.hpp"
#include <map>
#include <string>
#include <vector>

using namespace ast;

enum PrecedenceType {
    pr_lowest,      // lowest possible precedence
    pr_conditional, // if...else
    pr_and_or,      // and,or
    pr_not,         // not
    pr_compare,     // ==, !=, <, >, <=, >=
    pr_bit_or,      // |
    pr_bit_xor,     // ^
    pr_bit_and,     // &
    pr_bit_shift,   // >> , <<
    pr_sum_minus,   // +, -
    pr_mul_div,     // *, /, %, //
    pr_expo,        // **
    pr_prefix,      // -x
    pr_dot_arrow_ref,     // x.test(), x.prop ,x->y
    pr_list_access, // x[0], x["test"]
    pr_call,         // x()
    pr_postfix      // ++x
};

std::map<TokenType, PrecedenceType> createMap();

class Parser {
  private:
    size_t m_tokIndex{0};
    Token m_currentToken;
    std::vector<Token> m_tokens;
    std::string m_filename;
    const std::vector<TokenType> aug_operators{
                                            tk_slash_equal,
                                            tk_floor_equal,
                                            tk_plus_equal,
                                            tk_minus_equal,
                                            tk_times_equal,
                                            tk_mod_equal,
                                            tk_shift_left_equal,
                                            tk_shift_right_equal,
                                            tk_bit_and_equal,
                                            tk_bit_or_equal,
                                            tk_bit_xor_equal,
                                            tk_exponent_equal
                                        };
    std::map<TokenType, PrecedenceType> precedenceMap = createMap();
    void advance();
    void advanceOnNewLine();
    void expect(TokenType expectedType, std::string msg="",std::string submsg="",std::string hint="",std::string ecode="");
    Token next();
    PrecedenceType nextPrecedence();

    void error(Token tok, std::string msg,std::string submsg="",std::string hint="",std::string ecode="");

    parameter parseParameter();

    AstNodePtr parseInteger();
    AstNodePtr parseDecimal();
    AstNodePtr parseString(bool isFormatted, bool isRaw);
    AstNodePtr parseBool();
    AstNodePtr parseNone();
    AstNodePtr parseIdentifier();
    AstNodePtr parseName();
    AstNodePtr parseList();
    AstNodePtr parseDict();

    AstNodePtr parseType(bool var_dec=false, 
                         bool* has_value=nullptr,
                         bool can_be_sumtype=true
                          );
    AstNodePtr parseImportedType(bool var_dec,bool* has_value);
    AstNodePtr parsePointerType(bool var_dec,bool* has_value);
    AstNodePtr parseRefType(bool var_dec,bool* has_value);
    AstNodePtr parseListType(bool var_dec,bool* has_value);
    AstNodePtr parseFuncType();

    AstNodePtr parseExpression(PrecedenceType type = pr_lowest);
    AstNodePtr parsePrefixExpression();
    AstNodePtr parseGroupedExpr();

    AstNodePtr parseBinaryOperation(AstNodePtr left);
    AstNodePtr parseTernaryIf(AstNodePtr left);
    AstNodePtr parseFunctionCall(AstNodePtr left);
    AstNodePtr parseListOrDictAccess(AstNodePtr left);
    AstNodePtr parseDotExpression(AstNodePtr left);
    AstNodePtr parseArrowExpression(AstNodePtr left);
    AstNodePtr parsePostfixExpression(AstNodePtr left);

    AstNodePtr parseReturnExprTurple(AstNodePtr item);
    AstNodePtr parseReturnTypeTurple(AstNodePtr item);
    AstNodePtr parseExtern();
    AstNodePtr parseMultipleAssign(AstNodePtr);
    AstNodePtr parseVirtual();
    AstNodePtr parseCast();
    AstNodePtr parseStatement();
    AstNodePtr parseWith();
    AstNodePtr parseDecoratorCall();
    AstNodePtr parseBlockStatement();
    AstNodePtr parseImport();
    AstNodePtr parseRaise();
    AstNodePtr parseStatic();
    AstNodePtr parseInline();
    AstNodePtr parseVariableStatement();
    AstNodePtr parseConstDeclaration();
    AstNodePtr parseIf();
    AstNodePtr parseAssert();
    AstNodePtr parseMatch();
    AstNodePtr parseScope();
    AstNodePtr parseWhile();
    AstNodePtr parseFor();
    AstNodePtr parseFunctionDef();
    AstNodePtr parseExternFuncDef(Token);
    AstNodePtr parseExternUnion(Token tok);
    AstNodePtr parseExternStruct(Token tok);
    AstNodePtr parseMethodDef();
    AstNodePtr parseClassDefinition();
    AstNodePtr parseReturn();
    AstNodePtr parseUnion();
    AstNodePtr parseEnum();
    AstNodePtr parseExport();
    AstNodePtr parseTypeDef();
    AstNodePtr parseDefaultArg();
    AstNodePtr parseTryExcept();

  public:
    Parser(const std::vector<Token>& tokens,std::string filename);
    ~Parser();

    AstNodePtr parse();
};

#endif
