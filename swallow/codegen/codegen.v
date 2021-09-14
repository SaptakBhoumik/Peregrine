// Original author: Saptak Bhoumik

module codegen

import parser

pub fn codegen(ast parser.Ast) []string{
	required_arg:=["str_variable_required_argument","int_variable_required_argument","bool_variable_required_argument","list_variable_required_argument","dictionary_variable_required_argument","float_variable_required_argument","void_variable_required_argument"]
	variable_ast:=["str_variable","int_variable","bool_variable","list_variable","dictionary_variable","float_variable","void_variable"]
	ast_type:=["function_call","function_define"]
	mut is_function_call:=false
	mut is_operator:=false
	mut keyword:=""
	mut first_bracket_count:=0
	mut code:=[]string{}
	mut previous_code_block:=parser.Body{}
	mut next_item:=parser.Body{}
	for index,item in ast.body{
		keyword=item.keyword
		// println(is_function_call)
		if index<ast.body.len-1 && index!=0{
			next_item=ast.body[index+1]
		}
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
		//codegen starts here
		if item.ast_type=="Ccode"{
			code<<item.keyword
		}

		else if next_item.keyword=="//"{
			code<<"round_number($item.keyword"
			is_operator==true
		}
		else if item.keyword=="//"{
			code<<"/"
		}
		else if previous_code_block.keyword=="//"{
			code<<"$item.keyword)"
		}


		else if next_item.keyword=="^"{
			code<<"mypow($item.keyword"
			is_operator==true
		}
		else if item.keyword=="^"{
			code<<","
		}
		else if previous_code_block.keyword=="^"{
			code<<"$item.keyword)"
		}
		else if item.ast_type in variable_ast{
			code<<variable(item.ast_type,keyword)
		}
		else if is_operator==true && item.keyword!=r"\n"{
			code[code.len-1]+=keyword
		}
		else if is_operator==true && item.keyword==r"\n"{
			code<<";\n"
			is_operator=false
		}
		else if (next_item.ast_type=="binary_operator" || next_item.ast_type=="assign") && is_operator==false{
			is_operator=true
			code<<keyword
		}
		else if (item.ast_type=="binary_operator" || item.ast_type=="assign") && is_operator==false{
			is_operator=true
			code[code.len-1]+=keyword
		}
		else if item.ast_type=="function_define"{
			code<<func_return(item.keyword,ast)
		}
		else if item.ast_type in required_arg{
			if item.ast_type=="str_variable_required_argument"{
				code<<"char * ${item.keyword}"
			}
			else if item.ast_type=="void_variable_required_argument"{
				code<<"void * ${item.keyword}"
			}
			else if item.ast_type=="int_variable_required_argument"{
				code<<"int64_t ${item.keyword}"
			}
			else if item.ast_type=="bool_variable_required_argument"{
				code<<"bool ${item.keyword}"
			}
		}
		else if item.ast_type =="function_call" && is_function_call==false{
			code<<item.keyword
		}
		else if previous_code_block.ast_type =="function_call" && is_function_call==false{
			code[code.len-1]+=keyword
		}
		else if is_function_call==true{
			code[code.len-1]+=keyword
		}
		if item.keyword=="(" && is_function_call==true{
			first_bracket_count++
		}
		else if item.keyword=="(" && is_function_call==false && previous_code_block.ast_type=="function_call"{
			first_bracket_count++
		}
		else if item.keyword==")" && is_function_call==true{
			first_bracket_count--
		}
		if first_bracket_count==0{
			if is_function_call==true{
				if next_item.line!=item.line || next_item==item || next_item.keyword==r"\n"{
					code<<";\n"
				}
			}
			is_function_call=false
		}
		else{
			is_function_call=true
		} 
		if previous_code_block.tab==item.tab+1 && keyword!=r"\n" && is_function_call==false && previous_code_block.ast_type!="function_define" && (item.ast_type in ast_type) == false{
			code<<"\n}\n"
		}
		else if next_item.tab==item.tab-1 && next_item.keyword!=r"\n" && keyword!=r"\n" && is_function_call==false && previous_code_block.ast_type!="function_define" && (item.ast_type in ast_type) == false{
			code<<"\n}\n"
		}
		else if item==next_item && item!=parser.Body{}{
			code<<"\n}\n"
		}
		
		if keyword!=r"\n"{
			previous_code_block=item
		}
	} 
	return code
}