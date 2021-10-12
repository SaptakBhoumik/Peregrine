"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from parser.parser import *
from lexer.tokens import *

tokens = [
        Token("(", TokenType.tk_l_paren, 0, 0),
        Token("-", TokenType.tk_minus, 0, 0),
        Token("34", TokenType.tk_int, 0, 0),
        Token("+", TokenType.tk_plus, 0, 0),
        Token("76", TokenType.tk_int, 0, 0),
        Token(")", TokenType.tk_r_paren, 0, 0),
        Token("*", TokenType.tk_asterisk, 0, 0),
        Token("34", TokenType.tk_int, 0, 0),
    ]

parser = Parser(tokens)

program = parser.parse()
print(program.__str__())