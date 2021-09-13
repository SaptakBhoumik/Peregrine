// Original author: Saptak Bhoumik

module codegen

import parser

pub fn codegen(ast parser.Ast) []string{
	required_arg:=["str_variable_required_argument","int_variable_required_argument","bool_variable_required_argument","list_variable_required_argument","dictionary_variable_required_argument","float_variable_required_argument","void_variable_required_argument"]
	mut code:=[]string{}
	mut previous_code_block:=parser.Body{}
	for item in ast.body{
		if previous_code_block.ast_type=="function_define" && item.direction!="right"{
			code<<"(){\n"
		}
		else if previous_code_block.ast_type=="function_define" && item.direction=="right" && item.ast_type in required_arg{
			code<<"("
		}
		else if previous_code_block.ast_type in required_arg && item.direction=="right" && item.ast_type in required_arg{
			code<<","
		}
		else if previous_code_block.ast_type in required_arg && item.direction!="right"{
			code<<"){\n"
		}
		if item.ast_type=="Ccode"{
			code<<item.keyword
		}
		else if item.ast_type=="function_define"{
			code<<func_return(item.keyword,ast)
		}
		else if item.ast_type in required_arg{
			if item.ast_type=="str_variable_required_argument"{
				code<<"char * ${item.keyword}"
			}
		}
		else if item.keyword==r"\n" && previous_code_block.keyword!=r"\n" && previous_code_block.keyword!=r""{
			code<<";\n"
		}
		previous_code_block=item
	} 
	println(ast)
	return code
}