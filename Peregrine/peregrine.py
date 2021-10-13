"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

import pe_parser
from lexer import lexer
print(lexer.lexer('int test = (-34 + 76) * 34.5',"file"))