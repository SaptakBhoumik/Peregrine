"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from typing import List

from lexer.tokens import Token

class Node:
    def __str__(self) -> str:
        pass

class Program(Node):
    nodes: List[Node] = []

    def __str__(self) -> str:
        result = ""

        for node in self.nodes:
            result += node.__str__()

        return result

class IntegerLiteral(Node):
    value: str

    def __init__(self, value: str) -> None:
        self.value = value

    def __str__(self) -> str:
        return self.value

class StringLiteral(Node):
    value: str

    def __init__(self, value: str) -> None:
        self.value = value

    def __str__(self) -> str:
        return self.value

class BoolLiteral(Node):
    value: bool

    def __init__(self, value: bool) -> None:
        self.value = value

    def __str__(self) -> str:
        return "true" if self.value else "false"

class Identifier(Node):
    value: str

    def __init__(self, value: str) -> None:
        self.value = value

    def __str__(self) -> str:
        return self.value

class BinaryOperation(Node):
    left: Node
    operator: str
    right: Node

    def __init__(self, left: Node, operator: str, right: Node) -> None:
        self.left = left
        self.operator = operator
        self.right = right

    def __str__(self) -> str:
        return f"({self.left.__str__()} {self.operator} {self.right.__str__()})"

class PrefixExpression(Node):
    prefix: str
    value: Node

    def __init__(self, prefix: str, value: Node) -> None:
        self.prefix = prefix
        self.value = value

    def __str__(self) -> str:
        return f"({self.prefix}{self.value.__str__()})"

class VariableDeclaration(Node):
    varType: Token
    varName: str
    value: Node

    def __init__(self, varType: Token, varName: str, value: Node = None) -> None:
        self.varType = varType
        self.varName = varName
        self.value = value

    def __str__(self) -> str:
        return f"{self.varType.keyword} {self.varName} = {str(self.value)}"

class VariableReassignment(Node):
    varName: str
    newValue: Node

    def __init__(self, varName: str, newValue: Node) -> None:
        self.varName = varName
        self.newValue = newValue

    def __str__(self) -> str:
        return f"{self.varName} = {str(self.newValue)}"

    