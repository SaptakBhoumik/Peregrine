module lexer

pub enum Token_type {
	space
	lparan
	rparan
	_if
	_else
	_and
	_or
	_not
	_in
	_is
	_elif
	_while
	_for
	_match
	_case
	_default
	_break
	_continue
	newline
	bracket
	peregrine_type
	comma
	dot
	decorator
	operator
	colon
	loop
	logic
	def
	add
	minus
	struct_name
	class
	constant
	assign
	ccode
	equals
	shl
	n_equals
	gte
	gt
	bitand
	xor_equal
	minus_minus
	bitnot
	rem_equal
	shr_equal
	bitand_equal
	shl_equal
	bitor_equal
	minus_equal
	div_equal
	multiply_equal
	plus_equal
	floor
	bitor
	add_add
	lt
	lte
	xor
	shr
	rem
	ret
	multi
	div
	pass
	str
	integer
	decimal
	pointer
	null
	list
	dictionary
	_char
	_int
	_bool
	_str
	_list
	_dictionary
	_float
	_void
	_int32
	_int16
	_int8
	_float32
	_uint32
	_uint16
	_uint8
	_uint
	no
	yes
	unknown
}

pub struct Lex {
pub mut:
	keyword      string
	keyword_type Token_type
}

pub fn lexer(token []string) []Lex {
	mut lex_item := Lex{}
	mut collection := []Lex{}
	for item in token {
		match item {
			' ' {
				lex_item.keyword_type = Token_type.space
			}
			r'\n' {
				lex_item.keyword_type = Token_type.newline
			}
			'(' {
				lex_item.keyword_type = Token_type.lparan
			}
			')' {
				lex_item.keyword_type = Token_type.rparan
			}
			'return' {
				lex_item.keyword_type = Token_type.ret
			}
			'break' {
				lex_item.keyword_type = Token_type._break
			}
			'continue' {
				lex_item.keyword_type = Token_type._continue
			}
			',' {
				lex_item.keyword_type = Token_type.comma
			}
			'.' {
				lex_item.keyword_type = Token_type.dot
			}
			':' {
				lex_item.keyword_type = Token_type.colon
			}
			'def' {
				lex_item.keyword_type = Token_type.def
			}
			'struct' {
				lex_item.keyword_type = Token_type.struct_name
			}
			'class' {
				lex_item.keyword_type = Token_type.class
			}
			'const' {
				lex_item.keyword_type = Token_type.constant
			}
			'Ccode' {
				lex_item.keyword_type = Token_type.ccode
			}
			'pass' {
				lex_item.keyword_type = Token_type.pass
			}
			'if' {
				lex_item.keyword_type = Token_type._if
			}
			'else' {
				lex_item.keyword_type = Token_type._else
			}
			'elif' {
				lex_item.keyword_type = Token_type._elif
			}
			'for' {
				lex_item.keyword_type = Token_type._for
			}
			'while' {
				lex_item.keyword_type = Token_type._while
			}
			'match' {
				lex_item.keyword_type = Token_type._match
			}
			'case' {
				lex_item.keyword_type = Token_type._case
			}
			'default' {
				lex_item.keyword_type = Token_type._default
			}
			'or' {
				lex_item.keyword_type = Token_type._or
			}
			'and' {
				lex_item.keyword_type = Token_type._and
			}
			'not' {
				lex_item.keyword_type = Token_type._not
			}
			'in' {
				lex_item.keyword_type = Token_type._in
			}
			'is' {
				lex_item.keyword_type = Token_type._is
			}
			'+' {
				lex_item.keyword_type = Token_type.add
			}
			'-' {
				lex_item.keyword_type = Token_type.minus
			}
			'=' {
				lex_item.keyword_type = Token_type.assign
			}
			'*' {
				lex_item.keyword_type = Token_type.multi
			}
			'/' {
				lex_item.keyword_type = Token_type.div
			}
			'%' {
				lex_item.keyword_type = Token_type.rem
			}
			'==' {
				lex_item.keyword_type = Token_type.equals
			}
			'!=' {
				lex_item.keyword_type = Token_type.n_equals
			}
			'<<' {
				lex_item.keyword_type = Token_type.shl
			}
			'>>' {
				lex_item.keyword_type = Token_type.shr
			}
			'>=' {
				lex_item.keyword_type = Token_type.gte
			}
			'>' {
				lex_item.keyword_type = Token_type.gt
			}
			'<=' {
				lex_item.keyword_type = Token_type.lte
			}
			'<' {
				lex_item.keyword_type = Token_type.lt
			}
			'&' {
				lex_item.keyword_type = Token_type.bitand
			}
			'|' {
				lex_item.keyword_type = Token_type.bitor
			}
			'^' {
				lex_item.keyword_type = Token_type.xor
			}
			'++' {
				lex_item.keyword_type = Token_type.add_add
			}
			'--' {
				lex_item.keyword_type = Token_type.minus_minus
			}
			'//' {
				lex_item.keyword_type = Token_type.floor
			}
			'&=' {
				lex_item.keyword_type = Token_type.bitand_equal
			}
			'|=' {
				lex_item.keyword_type = Token_type.bitor_equal
			}
			'+=' {
				lex_item.keyword_type = Token_type.plus_equal
			}
			'-=' {
				lex_item.keyword_type = Token_type.minus_equal
			}
			'*=' {
				lex_item.keyword_type = Token_type.multiply_equal
			}
			'/=' {
				lex_item.keyword_type = Token_type.div_equal
			}
			'%=' {
				lex_item.keyword_type = Token_type.rem_equal
			}
			'~' {
				lex_item.keyword_type = Token_type.bitnot
			}
			'<<=' {
				lex_item.keyword_type = Token_type.shl_equal
			}
			'>>=' {
				lex_item.keyword_type = Token_type.shr_equal
			}
			'^=' {
				lex_item.keyword_type = Token_type.xor_equal
			}
			'char' {
				lex_item.keyword_type = Token_type._char
			}
			'int' {
				lex_item.keyword_type = Token_type._int
			}
			'bool' {
				lex_item.keyword_type = Token_type._bool
			}
			'str' {
				lex_item.keyword_type = Token_type._str
			}
			'list' {
				lex_item.keyword_type = Token_type._list
			}
			'dictionary' {
				lex_item.keyword_type = Token_type._dictionary
			}
			'float' {
				lex_item.keyword_type = Token_type._float
			}
			'void' {
				lex_item.keyword_type = Token_type._void
			}
			'int32' {
				lex_item.keyword_type = Token_type._int32
			}
			'int16' {
				lex_item.keyword_type = Token_type._int16
			}
			'int8' {
				lex_item.keyword_type = Token_type._int8
			}
			'float32' {
				lex_item.keyword_type = Token_type._float32
			}
			'uint32' {
				lex_item.keyword_type = Token_type._uint32
			}
			'uint16' {
				lex_item.keyword_type = Token_type._uint16
			}
			'uint8' {
				lex_item.keyword_type = Token_type._uint8
			}
			'uint' {
				lex_item.keyword_type = Token_type._uint
			}
			'True' {
				lex_item.keyword_type = Token_type.yes
			}
			'False' {
				lex_item.keyword_type = Token_type.no
			}
			else {
				if item.split('')[0] == '@' {
					lex_item.keyword_type = Token_type.decorator
				} else {
					lex_item.keyword_type = know_type(item)
				}
			}
		}
		lex_item.keyword = item
		collection << lex_item
		lex_item = Lex{}
	}
	return collection
}
