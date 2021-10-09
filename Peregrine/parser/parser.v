module parser

import ast { Ast, Body, Var }
import lexer { Lex }
// Original author: Saptak Bhoumik

__global (
	code Lex
)

fn advance(index int, tokens []string) string {
	mut next_item := ''
	if tokens.len >= index + 1 {
		next_item = tokens[index + 1]
	}
	return next_item
}

pub fn parser(code []Lex) (Ast, string) {
	peregrine_type := ['char', 'int', 'bool', 'str', 'list', 'dictionary', 'float', 'void', 'int32',
		'int16', 'int8', 'float32', 'uint32', 'uint16', 'uint8', 'uint']

	decorator := ['@method', '@free'] // more will be added soon
	operater := ['=', '==', '+', '-', '*', '/', '^', '//', '%', '>', '<', '>=', '<=', '!=', '++',
		'--', '&', '|', '~', '<<', '>>', '+=', '-=', '*=', '/=', '%=', '<<=', '>>=', '&=', '|=',
		'^=']
	loop := ['if', 'while', 'elif', 'else', 'for', 'match', 'case', 'default']
	logic := ['and', 'or', 'not', 'in', 'is']
	variable := '_variable'
	underscore := '_'
	mut var := Var{}
	mut is_pointer := false
	mut error := ''
	mut line := 0
	mut return_type := false
	mut is_asm := false
	mut first_bracket_count := 0
	mut is_function_call := false
	mut is_return := false
	mut is_operator := false
	mut is_argument := false
	mut next_item := ''
	mut right := false
	mut is_loop := false
	mut previus_item := ''
	mut is_tab := true
	mut tab := 0.0
	mut is_func_def := false
	mut is_ccode := false
	mut code_block := Body{}
	mut previous_code_block := Body{}
	mut last_left_code_block := Body{}
	mut json := Ast{}
	return json, error
}
