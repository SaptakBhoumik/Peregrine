"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from typing import List

class Node:
    def toString(self) -> str:
        pass

class Program(Node):
    nodes: List[Node] = []

    def toString(self) -> str:
        result = ""

        for node in self.nodes:
            result += node.toString()

        return result

class IntegerLiteral(Node):
    value: str

    def __init__(self, value: str) -> None:
        self.value = value

    def toString(self) -> str:
        return self.value

class StringLiteral(Node):
    value: str

    def __init__(self, value: str) -> None:
        self.value = value

    def toString(self) -> str:
        return self.value

class BoolLiteral(Node):
    value: bool

    def __init__(self, value: bool) -> None:
        self.value = value

    def toString(self) -> str:
        return "true" if self.value else "false"

class Identifier(Node):
    value: str

    def __init__(self, value: str) -> None:
        self.value = value

    def toString(self) -> str:
        return self.value

class BinaryOperation(Node):
    left: Node
    operator: str
    right: Node

    def __init__(self, left: Node, operator: str, right: Node) -> None:
        self.left = left
        self.operator = operator
        self.right = right

    def toString(self) -> str:
        return f"({self.left.toString()} {self.operator} {self.right.toString()})"

class PrefixExpression(Node):
    prefix: str
    value: Node

    def __init__(self, prefix: str, value: Node) -> None:
        self.prefix = prefix
        self.value = value

    def toString(self) -> str:
        return f"({self.prefix}{self.value.toString()})"