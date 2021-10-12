"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from parser.parser import *
from lexer.tokens import *

def createToken(keyword: str, type: TokenType) -> Token:
    tok = Token(keyword, 0, 0, type, 0)
    return tok

tokens = [
    createToken("int", TokenType.tk_int),
    createToken("test", TokenType.tk_identifier),
    createToken("=", TokenType.tk_assign),
    createToken("(", TokenType.tk_l_paren),
    createToken("-", TokenType.tk_minus),
    createToken("34", TokenType.tk_int),
    createToken("+", TokenType.tk_plus),
    createToken("76", TokenType.tk_int),
    createToken(")", TokenType.tk_r_paren),
    createToken("*", TokenType.tk_asterisk),
    createToken("34", TokenType.tk_int),
]

parser = Parser("", tokens)

program = parser.parse()
print(program.__str__())