// Original author: Saptak Bhoumik

module codegen

import parser

pub fn codegen(ast parser.Ast) []string{
	required_arg:=["str_variable_required_argument","int_variable_required_argument","bool_variable_required_argument","list_variable_required_argument","dictionary_variable_required_argument","float_variable_required_argument","void_variable_required_argument"]
	variable_ast:=["str_variable","int_variable","bool_variable","list_variable","dictionary_variable","float_variable","void_variable"]
	loop:=["if","while","elif","else","for","match","case","default"]
	if_else_loop:=["if","while","elif","else","for"]
	mut ast_type:=["function_call","function_define","binary_operator","assign"]
	mut is_function_call:=false
	mut is_string_compare:=false
	mut function:=parser.Function{}
	mut tab_dif:=0
	mut is_case:=false
	mut is_match:=false
	mut is_loop:=false
	mut is_return:=false
	mut is_operator:=false
	mut is_func_def:=false
	mut keyword:=""
	mut c_function:="string"
	mut free:=[]string{}
	mut first_bracket_count:=0
	mut code:=[]string{}
	mut previous_code_block:=parser.Body{}
	mut next_item:=parser.Body{}
	ast_type<<required_arg
	ast_type<<variable_ast
	for index,mut item in ast.body{
		keyword=item.keyword
		// if item.ast_type in required_arg {
		// 	//do nothing
		// }
		if item.ast_type=="new_line" {
			item.keyword=""
		}
		if index<ast.body.len-1 && index!=0{
			next_item=ast.body[index+1]
		}
		if previous_code_block.ast_type=="function_define" && item.direction!="right"{
			code<<"){\n"
		}
		else if previous_code_block.ast_type in required_arg && item.direction!="right"{
			code<<"){\n"
		}
		if item.keyword=="case" && is_case==false{
			is_case=true
			if is_match==true{
				code<<"){\n"
				is_match=false
			}
		}
		else if item.keyword=="match"  && is_match==false{
			is_match=true
			if is_case==true{
				code<<"):{\n"
				is_case=false
			}
		}
		else if is_case==true && item.direction=="left"{
			is_loop=false
			code<<"):{\n"
			is_case=false
		}
		else if item.keyword=="default"{
			//do nothing
			}
		else if previous_code_block.keyword=="default"{
			is_loop=false
			code<<":{\n"
			}
		
		else if is_loop==true && (item.direction=="left" || item.keyword=="pass"){
			if previous_code_block.keyword!="else" {
				if is_string_compare==true{
					code<<")==0"
					is_string_compare=false
				}
				code<<"){\n"
			}
			else{
				code<<"{\n"
			}
			is_loop=false
		}
		else if previous_code_block.ast_type in required_arg && item.direction=="right" && item.ast_type in required_arg{
			code<<","
		}
		

		if item.ast_type=="formatted_string"{
			keyword=fstring(item)
			item.keyword=keyword
		}
		else if item.ast_type=="raw_string"	{
			if item.keyword.split('')[0]=='"'{
				keyword=item.keyword.split('"')[1]
				keyword='R"($keyword)"'
			}
			else{
				keyword=item.keyword.split("'")[1]
				keyword='R"($keyword)"'
			}
			item.keyword=keyword
		}
		if previous_code_block.keyword in if_else_loop || previous_code_block.keyword=="or" || 	previous_code_block.keyword=="and"{
			if item.ast_type=="string"{
				code<<" strcmp(" 
				is_string_compare=true
			}
			else if item.ast_type=="variable" && item.keyword in function.variable && var_type(function.variable_type,keyword)=="str"{
				code<<" strcmp(" 
				is_string_compare=true
			}
			else if item.ast_type=="function_call" && func_return_if_else(keyword,ast)==["str"]{
				code<<" strcmp(" 
				is_string_compare=true
			}
		}
		if item.keyword=="or" || item.keyword=="and"{
			if is_string_compare==true{
				code<<")==0 "
				is_string_compare=false
			}
		}
		//codegen starts here
		if item.keyword=="pass"{
			//do nothing
		}
		else if item.keyword=="==" && is_string_compare==true{
			code<<","
		}
		else if item.ast_type=="compare"{
			code<<"$item.keyword"
		}
		else if item.keyword=="and"{
			code<<"&& "
		}
		else if item.keyword=="or"{
			code<<"|| "
		}
		else  if item.keyword=="break" || item.keyword=="continue"{
			code<<"$item.keyword ;\n"
		}
		else if item.keyword in loop{
			if item.keyword=="else" || item.keyword=="default"{
				code<<"$keyword "
			}
			else{
				if keyword=="elif"{
					keyword="else if"
				}
				else if keyword=="match"{
					keyword="switch"
				}
				code<<"$keyword ("
			}
			is_loop=true
		}
		else if (item.line!=next_item.line||next_item.ast_type=="new_line") && is_return==true{
			if next_item.ast_type!="new_line"{
				code<<"$keyword ;\n"
				is_return=false
			}
			else{
				code<<"$keyword "
			}
		}
		else if is_return==true && item==next_item{
			code<<"$keyword ;\n"
			is_return=false
		}
		else if is_return==true && item.ast_type=="new_line"{
			code<<";\n"
			is_return=false
		}
		else if item.tab!=next_item.tab && is_return==true{
			code<<"$keyword ;\n"
			is_return=false
		}
		else if item.ast_type=="function_define"{
			c_function,free,function=func_return(item.keyword,ast)
			code<<c_function
			is_func_def=true
		}
		else if ((is_function_call==true && (next_item.line!=item.line || next_item==item || next_item.ast_type=="new_line") && item.keyword==")")||( is_operator==true && item.ast_type=="new_line")||( is_operator==true && item==next_item)) && is_loop==false{
			if keyword==")"{
				code<<");\n"
			}
			else if is_operator==true && item==next_item{
				code<<"$keyword;\n"
			}
			else if previous_code_block.keyword==")" && item.ast_type=="new_line"{
				//do nothing
			}
			else{
				code<<";\n"
			}
			is_operator=false

		}
		else if item.ast_type=="Ccode"{
			code<<"$item.keyword \n"
		}
		else if item.keyword=="return"{
			code<<"$item.keyword "
			is_return=true
		}
		else if next_item.keyword=="//"{
			code<<"myFloor($item.keyword"
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
			is_operator==true
		}
		else if is_operator==true && item.ast_type!="new_line"{
			code[code.len-1]+=keyword
		}
		else if (next_item.ast_type=="binary_operator" || next_item.ast_type=="assign") && is_operator==false{
			is_operator=true
			code<<keyword
		}
		else if (item.ast_type=="binary_operator" || item.ast_type=="assign") && is_operator==false{
			is_operator=true
			code[code.len-1]+=keyword
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
			is_function_call==true
		}
		else if previous_code_block.ast_type =="function_call" && is_function_call==false{
			code[code.len-1]+=keyword
		}
		else if is_function_call==true{
			code[code.len-1]+=keyword
		}
		else if is_return==true && item.ast_type!="new_line"{
			code[code.len-1]+="$keyword "
		}
		else if is_loop==true && item.direction=="right"{
			code<<"$item.keyword "
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
			is_function_call=false
		}
		else{
			is_function_call=true
		}
		if is_func_def==true && item.tab!=0{
			is_func_def=false
		}
		// if (next_item.ast_type=="function_define" || item==next_item) && free.len!=0{
		// 	for free_var in free{
		// 		code<<"\nfree($free_var);\n$free_var=NULL;\n"
		// 	} 
		// 	free=[]string{}
		// }
		
		if next_item.tab<item.tab || item==next_item{
			if next_item==item{
				tab_dif=int(item.tab)
				for _ in 0..tab_dif{
					code<<"\n}\n"
				}
			}

			else if next_item.ast_type=="new_line" && ast.body.len>index+2 && item!=next_item && is_loop==false{
				tab_dif=int(item.tab-ast.body[index+2].tab)
				if is_function_call==true || is_operator==true || is_return==true {
					is_operator=false
					is_function_call=false
					is_return=false
					code<<";\n"
				}
				for _ in 0..tab_dif{
					code<<"\n}\n"
				}
			}
			else if next_item.direction=="left" && item!=next_item{
				tab_dif=int(item.tab-next_item.tab)
				for _ in 0..tab_dif{
					code<<"\n}\n"
				}
			}
			
		}
		
		if item.ast_type!="new_line"{
			previous_code_block=ast.body[index]
		}
	} 
	return code
}