module ast

// Original author: Saptak Bhoumik
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
