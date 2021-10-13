"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from pe_parser.pe_parser import *
from lexer.tokens import *
from lexer import lexer
tokens=lexer.lexer("int test=(-34+76)*34.5","file")
parser = Parser("", tokens)
program = parser.parse()
print(program.__str__())