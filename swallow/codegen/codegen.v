// Original Author: Ethan Olchik

module codegen

import parser
import os

fn codegen_variable(ast parser.Ast, id int, idx int) string {
	println(ast.body)
	
	return "${ast.body[idx+2].ast_type} ${ast.body[idx].keyword} ${ast.body[idx+1].keyword} ${ast.body[idx+2].keyword};"
}

fn codegen_assign(ast parser.Ast, id int, idx int) {

}

fn codegen_binop(ast parser.Ast, id int, _idx int) (string, int) {
	mut out := ""
	mut op_list := ["=","==",'+','-','*','/','^','//','%','>','<','>=','<=','!=']
	mut idx := _idx
	println(ast.body)
	if ast.body[idx-2].keyword in op_list {
		println("bruh")
		mut b := 0
		for ast.body[idx-2].keyword in op_list && !(idx - 2 >= ast.body.len) && ast.body[idx-2].ast_type != 'new_line'{
			println(b)
			b++
			println(ast.body[idx-2].ast_type)
			out += " ${ast.body[idx].keyword} ${ast.body[idx+1].keyword}"
			idx += 1
		}
		out += ";\n"
	} else if ast.body[idx+2].ast_type == 'binary_operator' {
		out += "${ast.body[idx-1].keyword} ${ast.body[idx].keyword} ${ast.body[idx+1].keyword}"
	} else {
		out += "${ast.body[idx-1].keyword} ${ast.body[idx].keyword} ${ast.body[idx+1].keyword};\n"
	}
	// match (ast.body[idx].keyword) {
	// 	'+' {
	// 		println(ast.body)
	// 		
			
	// 	}
	// 	'-' {
	// 		out += "${ast.body[idx-1].keyword} - ${ast.body[idx+1].keyword};\n"
	// 	}
	// 	'*' {
	// 		out += "${ast.body[idx-1].keyword} * ${ast.body[idx+1].keyword};\n"
	// 	}
	// 	'/' {
	// 		out += "${ast.body[idx-1].keyword} / ${ast.body[idx+1].keyword};\n"
	// 	}
	// 	// '+=' {
	// 	// 	out += "${ast.body[idx-1].keyword} += ${ast.body[idx+1].keyword};\n"
	// 	// }
	// 	// '-=' {
	// 	// 	out += "${ast.body[idx-1].keyword} -= ${ast.body[idx+1].keyword};\n"
	// 	// }
	// 	// '*=' {
	// 	// 	out += "${ast.body[idx-1].keyword} *= ${ast.body[idx+1].keyword};\n"
	// 	// }
	// 	// '/=' {
	// 	// 	out += "${ast.body[idx-1].keyword} /= ${ast.body[idx+1].keyword};\n"
	// 	// }
	// 	'//' {
	// 		out += "${ast.body[idx-1].keyword} // ${ast.body[idx+1].keyword};\n"
	// 	}
	// 	'%' {
	// 		out += "${ast.body[idx-1].keyword} % ${ast.body[idx+1].keyword};\n"
	// 	}
	// 	'^' {
	// 		out += "${ast.body[idx-1].keyword} ^ ${ast.body[idx+1].keyword};\n"
	// 	}
	// 	else { out += "" }
	// }

	return out, idx
}

fn codegen_comparison(ast parser.Ast, id int, idx int) string {
	mut out := ""

	match (ast.body[idx].keyword) {
		'==' {
			out += "${ast.body[idx-1].keyword} == ${ast.body[idx+1].keyword};\n"
		}
		'!=' {
			out += "${ast.body[idx-1].keyword} != ${ast.body[idx+1].keyword};\n"
		}
		'>=' {
			out += "${ast.body[idx-1].keyword} >= ${ast.body[idx+1].keyword};\n"
		}
		'<=' {
			out += "${ast.body[idx-1].keyword} <= ${ast.body[idx+1].keyword};\n"
		}
		'<' {
			out += "${ast.body[idx-1].keyword} < ${ast.body[idx+1].keyword};\n"
		}
		'>' {
			out += "${ast.body[idx-1].keyword} > ${ast.body[idx+1].keyword};\n"
		}
		else { out += "" }
	}

	return out
}

pub fn codegen(ast parser.Ast) ? {
	mut idx := 0
	mut output := ""
	output += "#include \"builtin.h\"\n"
	for x in ast.body {
		match (x.ast_type) {
			'variable' {
				output += codegen_variable(ast, x.id, idx)
			}
			// "assign" {
			// 	output += codegen_assign(ast, x.id, idx)
			// }
			'binary_operator' {
				o, i := codegen_binop(ast, x.id, idx)
				output += o
				println(i)
				idx = i
			}
			'compare' {
				output += codegen_comparison(ast, x.id, idx)
			}
			'new_line' {
				output+="\n"
			}
			else {}
		}
		println(x.ast_type)
		idx++
	}

	path := "${ast.path}.c"

	os.write_file(path, output)?
}