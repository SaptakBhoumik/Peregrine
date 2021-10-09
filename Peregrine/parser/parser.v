/*
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
*/

module parser
import ast { AstNode }
import tokenizer

enum Precedence {
	pr_lowest    // lowest possible precedence
	pr_sum       // + and -
	pr_mul       // * and /
	pr_prefix    // -x and !y
}

pub struct Parser {
pub mut:
	tk_index int // index of the current token
	tokens []tokenizer.Token
	current_token tokenizer.Token

	precedence_map map[tokenizer.TokenType]Precedence
}

pub fn (mut p Parser) parse(tokens []tokenizer.Token) AstNode {
	p.tokens = tokens
	mut program_node := ast.Program{}

	p.precedence_map = {
		tokenizer.TokenType.tk_plus : Precedence.pr_sum
		tokenizer.TokenType.tk_minus : Precedence.pr_sum
		tokenizer.TokenType.tk_asterisk : Precedence.pr_mul
		tokenizer.TokenType.tk_divide : Precedence.pr_mul
	}

	p.advance()

	for p.tk_index < p.tokens.len {
		// program_node.nodes << match p.current_token.tk_type {
		// 	else { p.parse_expression(int(Precedence.pr_lowest)) }
		// }
		program_node.nodes << p.parse_expression(int(Precedence.pr_lowest))
	}

	return program_node
}

fn (mut p Parser) advance() {
	if p.tk_index >= p.tokens.len {
		return
	}

	p.current_token = p.tokens[p.tk_index]
	p.tk_index++
}

fn (p Parser) next_token() ?tokenizer.Token {
	if p.tk_index < p.tokens.len {
		return p.tokens[p.tk_index]
	}

	return none
}

fn (p Parser) next_precedence() int {
	nt := p.next_token() or {
		return int(Precedence.pr_lowest)
	}

	if nt.tk_type in p.precedence_map {
		return int(p.precedence_map[nt.tk_type])
	}

	return int(Precedence.pr_lowest) 
}

fn (mut p Parser) parse_infix_expression(left AstNode) AstNode {
	operator := p.current_token.value
	current_precedence := p.precedence_map[p.current_token.tk_type]

	p.advance()

	right := p.parse_expression(int(current_precedence))

	infix_expr := ast.InfixExpression {
		left: left
		operator: operator
		right: right
	}

	return infix_expr
}

fn (mut p Parser) parse_integer() AstNode {
	int_node := ast.IntegerLiteral{ value: p.current_token.value }

	return int_node
}

fn (mut p Parser) parse_expression(precedence int) AstNode {
	mut left := match p.current_token.tk_type {
		.tk_integer { p.parse_integer() }
		else { 
			println(int(p.current_token.tk_type))
			exit(1) 
			} 
	}

	for p.next_precedence() > precedence {
		p.advance()
		left = p.parse_infix_expression(left) //not only infix
	}

	return left
}
