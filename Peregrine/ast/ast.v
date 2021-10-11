/*
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
*/

module ast

pub interface AstNode {
	to_string() string
}

pub struct Program {
pub mut:	
	nodes []AstNode
}

fn (program Program) to_string() string {
	mut result := ''

	for node in program.nodes {
		result += '${node.to_string()}\n'
	}

	return result
}

pub struct IntegerLiteral {
	value string
}

fn (il IntegerLiteral) to_string() string {
	return il.value
}

pub struct StringLiteral {
	value string
}

fn (sl StringLiteral) to_string() string {
	return sl.value
}

pub struct InfixExpression {
	left AstNode
	operator string
	right AstNode
}

fn (ie InfixExpression) to_string() string {
	return '(${ie.left.to_string()} ${ie.operator} ${ie.right.to_string()})'
}

pub struct Body {
pub mut:
	ast_type string
	keyword  string
	length   i64
	right    []Body
	left     []Body
}

pub struct Var {
pub mut:
	variable string
	var_type string
}

pub struct Function {
pub mut:
	name          string
	variable      []string
	variable_type []Var
	return_type   []string
	free          []string
}

pub struct Ast {
pub mut:
	import_file          []string
	header_file          []string
	c_file               []string
	folder               string
	path                 string
	function_return_type []Function
	function_define      []string = ['printf', 'exit']
	function_call        []string = ['main']
	method_define        []string
	method_call          []string
	free                 []string
	body                 []Body
}
