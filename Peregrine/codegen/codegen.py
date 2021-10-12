"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from parser import ast

class Codegen:

    def __init__(self, output: str) -> None:
        self.file = open(output)

    def write(self, code: str) -> None:
        try:
            self.file.write(code)
        except:
            pass #panic

    def generate(self, node: ast.Node) -> None:
        
        if isinstance(node, ast.Program):
            for stmt in node.nodes:
                self.generate(stmt)
        
        elif isinstance(node, ast.IntegerLiteral):
            self.write(node.value)

        else:
            pass # should not happen

        self.file.close()
