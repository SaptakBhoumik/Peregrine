// Original author: Saptak Bhoumik
module codegen

import parser

pub fn codegen(mut ast parser.Ast) []string {
	loop := ['if', 'while', 'elif', 'else', 'for', 'match', 'case']
	if_else_loop := ['if', 'while', 'elif', 'else', 'for']
	mut is_function_call := false
	mut is_match_case := false
	mut is_default := false
	mut last_match := []parser.Body{}
	mut match_item := []string{}
	mut sign := ''
	mut is_string_compare := false
	mut function := parser.Function{}
	mut tab_dif := 0
	mut is_case := false
	mut is_match := false
	mut is_loop := false
	mut is_return := false
	mut is_operator := false
	mut is_func_def := false
	mut keyword := ''
	mut c_function := 'string'
	mut free := []string{}
	mut first_bracket_count := 0
	mut code := []string{}
	mut previous_code_block := parser.Body{}
	mut next_item := parser.Body{}
	for index, mut item in ast.body {
		keyword = item.keyword
		if item.keyword == 'case' || item.keyword == 'default' || item.keyword == 'match' {
			is_match_case = true
		} else if item.keyword == 'else' && match_item.len >= 1 {
			is_match_case = true
		}
		if index < ast.body.len - 1 && index != 0 {
			next_item = ast.body[index + 1]
		}
		if previous_code_block.ast_type == 'function_define' && item.direction != 'right' {
			code << '){\n'
		} else if 'required' in previous_code_block.ast_type.split('_') && item.direction != 'right' {
			code << '){\n'
		}
		if is_loop == true && (item.direction == 'left' || item.keyword == 'pass') {
			if previous_code_block.keyword != 'else' {
				if is_string_compare == true {
					code << ')${sign}0'
					sign = ''
					is_string_compare = false
				}
				code << '){\n'
			} else {
				code << '{\n'
			}
			is_loop = false
			is_case = false
		} else if 'required' in previous_code_block.ast_type.split('_') && item.direction == 'right'
			&& 'required' in item.ast_type.split('_') {
			code << ','
		}

		if item.ast_type == 'formatted_string' {
			keyword = fstring(item)
			item.keyword = keyword
		} else if item.ast_type == 'raw_string' {
			if item.keyword.split('')[0] == '"' {
				keyword = item.keyword.split('"')[1]
				keyword = 'R"($keyword)"'
			} else {
				keyword = item.keyword.split("'")[1]
				keyword = 'R"($keyword)"'
			}
			item.keyword = keyword
		}
		if (previous_code_block.keyword in if_else_loop || previous_code_block.keyword == 'or'
			|| previous_code_block.keyword == 'and') && is_case == false {
			if item.ast_type == 'string' || item.ast_type == 'raw_string'
				|| item.ast_type == 'formatted_string' {
				code << ' compare('
				is_string_compare = true
			} else if item.ast_type == 'variable' && item.keyword in function.variable
				&& var_type(function.variable_type, keyword) == 'str' {
				code << ' compare('
				is_string_compare = true
			} else if item.ast_type == 'function_call'
				&& func_return_if_else(keyword, ast) == ['str'] {
				code << ' compare('
				is_string_compare = true
			}
		}
		if item.keyword == 'or' || item.keyword == 'and' {
			if is_string_compare == true {
				code << ')==0 '
				is_string_compare = false
			}
		}

		// codegen starts here
		if item.keyword == 'pass' {
			// do nothing
		} else if item.keyword == 'default' {
			is_default = true
		} else if previous_code_block.keyword == 'match' {
			match_item << item.keyword
			code << 'True'
			is_match = true
		} else if is_case == true {
			if item.keyword == 'or' {
				code << ' || '
			} else if item.keyword == 'and' {
				code << ' && '
			} else {
				if item.ast_type == 'string' || item.ast_type == 'raw_string'
					|| item.ast_type == 'formatted_string' {
					code << ' compare($match_item.last(),$item.keyword)==0 '
				} else if item.ast_type == 'variable' && item.keyword in function.variable
					&& var_type(function.variable_type, keyword) == 'str' {
					code << ' compare($match_item.last(),$item.keyword)==0'
				} else if item.ast_type == 'function_call'
					&& func_return_if_else(keyword, ast) == ['str'] {
					code << ' compare($match_item.last(),$item.keyword)==0'
				} else {
					code << ' $match_item.last()==$item.keyword'
				}
			}
		} else if (item.keyword == '==' || item.keyword == '!=') && is_string_compare == true {
			code << ','
			sign = item.keyword
		} else if item.ast_type == 'compare' {
			code << '$item.keyword'
		} else if item.keyword == 'and' {
			code << '&& '
		} else if item.keyword == 'or' {
			code << '|| '
		} else if item.keyword == 'break' || item.keyword == 'continue' {
			code << '$item.keyword ;\n'
		} else if item.keyword in loop {
			if item.keyword == 'else' {
				code << '$keyword '
			} else {
				if keyword == 'elif' {
					keyword = 'else if'
				} else if keyword == 'match' {
					keyword = 'while '
					last_match << item
				} else if item.keyword == 'case' && is_match == true {
					is_match = false
					is_case = true
					keyword = 'if '
				} else if item.keyword == 'case' && is_match == false {
					keyword = 'else if '
					is_case = true
				}
				code << '$keyword ('
			}
			is_loop = true
		} else if (item.line != next_item.line || next_item.ast_type == 'new_line')
			&& is_return == true {
			if next_item.ast_type != 'new_line' {
				code << '$keyword ;\n'
				is_return = false
			} else {
				code << '$keyword '
			}
		} else if is_return == true && item == next_item {
			code << '$keyword ;\n'
			is_return = false
		} else if is_return == true && item.ast_type == 'new_line' {
			code << ';\n'
			is_return = false
		} else if item.tab != next_item.tab && is_return == true {
			code << '$keyword ;\n'
			is_return = false
		} else if item.ast_type == 'function_define' {
			c_function, free, function = func_return(item.keyword, ast)
			code << c_function
			is_func_def = true
		} else if ((is_function_call == true && (next_item.line != item.line
			|| next_item == item || next_item.ast_type == 'new_line') && item.keyword == ')')
			|| (is_operator == true && item.ast_type == 'new_line')
			|| (is_operator == true && item == next_item)) && is_loop == false {
			if keyword == ')' {
				code << ');\n'
			} else if is_operator == true && item == next_item {
				code << '$keyword;\n'
			} else if previous_code_block.keyword == ')' && item.ast_type == 'new_line' {
				// do nothing
			} else {
				code << ';\n'
			}
			is_operator = false
		} else if item.ast_type == 'Ccode' {
			code << '$item.keyword \n'
		} else if item.keyword == 'return' {
			code << '$item.keyword '
			is_return = true
		} else if next_item.keyword == '//' {
			code << 'myFloor($item.keyword'
			is_operator == true
		} else if item.keyword == '//' {
			code << '/'
		} else if previous_code_block.keyword == '//' {
			code << '$item.keyword)'
		} else if next_item.keyword == '**' {
			code << 'mypow($item.keyword'
			is_operator == true
		} else if item.keyword == '**' {
			code << ','
		} else if previous_code_block.keyword == '**' {
			code << '$item.keyword)'
		} else if ('variable' in item.ast_type.split('_')
			|| 'constant' in item.ast_type.split('_')) && item.ast_type.split('_').len == 2 {
			code << variable(item.ast_type, keyword)
			if next_item.ast_type == 'new_line' || item.line < next_item.line {
				code << ';\n'
			} else {
				is_operator == true
			}
		} else if is_operator == true && item.ast_type != 'new_line' {
			code[code.len - 1] += keyword
		} else if (next_item.ast_type == 'binary_operator' || next_item.ast_type == 'assign')
			&& is_operator == false {
			is_operator = true
			code << keyword
		} else if (item.ast_type == 'binary_operator' || item.ast_type == 'assign')
			&& is_operator == false {
			is_operator = true
			code[code.len - 1] += keyword
		} else if 'required' in item.ast_type.split('_') {
			code << required_arg(item)
		} else if item.ast_type == 'function_call' && is_function_call == false {
			code << item.keyword
			is_function_call == true
		} else if previous_code_block.ast_type == 'function_call' && is_function_call == false {
			code[code.len - 1] += keyword
		} else if is_function_call == true {
			code[code.len - 1] += keyword
		} else if is_return == true && item.ast_type != 'new_line' {
			code[code.len - 1] += '$keyword '
		} else if is_loop == true && item.direction == 'right' {
			code << '$item.keyword '
		}
		if item.keyword == '(' && is_function_call == true {
			first_bracket_count++
		} else if item.keyword == '(' && is_function_call == false
			&& previous_code_block.ast_type == 'function_call' {
			first_bracket_count++
		} else if item.keyword == ')' && is_function_call == true {
			first_bracket_count--
		}
		if first_bracket_count == 0 {
			is_function_call = false
		} else {
			is_function_call = true
		}
		if is_func_def == true && item.tab != 0 {
			is_func_def = false
		}

		// if (next_item.ast_type=="function_define" || item==next_item) && free.len!=0{
		// 	for free_var in free{
		// 		code<<"\nfree($free_var);\n$free_var=NULL;\n"
		// 	}
		// 	free=[]string{}
		// }
		if last_match.len > 0 && is_match_case == true && is_loop == false {
			if last_match.last().tab == next_item.tab && item != next_item
				&& next_item.ast_type != 'new_line' {
				is_match_case = false
				if is_function_call == true || is_operator == true || is_return == true {
					is_operator = false
					is_function_call = false
					is_return = false
					code << ';\n'
				}
				if is_default == true {
					code << '\nbreak;\n'
					is_default = false
				} else {
					code << '\n}'
					code << '\nbreak;\n'
				}
				tab_dif = int(item.tab - next_item.tab)
				for _ in 1 .. tab_dif {
					code << '\n}\n'
				}
				last_match.pop()
			} else if item.ast_type == 'new_line' && ast.body.len > index + 2 && item != next_item
				&& last_match.last().tab == next_item.tab {
				tab_dif = int(item.tab - ast.body[index + 2].tab)
				if is_function_call == true || is_operator == true || is_return == true {
					is_operator = false
					is_function_call = false
					is_return = false
					code << ';\n'
				}
				if is_default == true {
					code << '\nbreak;\n'
					is_default = false
				} else {
					code << '\n}'
					code << '\nbreak;\n'
				}
				for _ in 1 .. tab_dif {
					code << '\n}\n'
				}
				last_match.pop()
			} else if item == next_item {
				tab_dif = int(item.tab) - 1
				for _ in last_match {
					if is_default == true {
						code << '\nbreak;\n'
						is_default = false
					} else {
						code << '\n}'
						code << '\nbreak;\n'
					}
					tab_dif--
				}
				for _ in 0 .. tab_dif {
					code << '\n}\n'
				}
				code << '\nreturn 0;\n}'
			} else if next_item.ast_type == 'new_line' && ast.body.len > index + 2
				&& item != next_item && is_loop == false {
				tab_dif = int(item.tab - ast.body[index + 2].tab)
				if is_function_call == true || is_operator == true || is_return == true {
					is_operator = false
					is_function_call = false
					is_return = false
					code << ';\n'
				}
				for _ in 0 .. tab_dif {
					code << '\n}\n'
				}
			} else if next_item.direction == 'left' && item != next_item {
				tab_dif = int(item.tab - next_item.tab)
				for _ in 0 .. tab_dif {
					code << '\n}\n'
				}
			}
		} else if next_item.tab < item.tab || item == next_item {
			if next_item == item {
				tab_dif = int(item.tab) - 1
				for _ in 0 .. tab_dif {
					code << '\n}\n'
				}
				code << '\nreturn 0;\n}'
			} else if next_item.ast_type == 'new_line' && ast.body.len > index + 2
				&& item != next_item && is_loop == false {
				tab_dif = int(item.tab - ast.body[index + 2].tab)
				if is_function_call == true || is_operator == true || is_return == true {
					is_operator = false
					is_function_call = false
					is_return = false
					code << ';\n'
				}
				for _ in 0 .. tab_dif {
					code << '\n}\n'
				}
			} else if next_item.direction == 'left' && item != next_item {
				tab_dif = int(item.tab - next_item.tab)
				for _ in 0 .. tab_dif {
					code << '\n}\n'
				}
			}
		}

		if item.ast_type != 'new_line' {
			previous_code_block = ast.body[index]
		}
	}
	return code
}
