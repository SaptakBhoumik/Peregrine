// Original author: Saptak Bhoumik

module codegen

import parser

pub fn codegen(ast parser.Ast) []string{
	required_arg:=["str_variable_required_argument","int_variable_required_argument","bool_variable_required_argument","list_variable_required_argument","dictionary_variable_required_argument","float_variable_required_argument","void_variable_required_argument"]
	variable_ast:=["str_variable","int_variable","bool_variable","list_variable","dictionary_variable","float_variable","void_variable"]
	mut ast_type:=["function_call","function_define","binary_operator","assign"]
	mut is_function_call:=false
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
		if previous_code_block.ast_type=="Ccode" && item.line!=previous_code_block.line && previous_code_block.tab>item.tab && item.keyword!=r"\n" &&  next_item.keyword!=r"\n" &&  is_operator==false && is_function_call==false {
			code<<"\n}\n"
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
		//codegen starts here
		if item.keyword=="pass"{
			//do nothing
		}
		else if item.line!=next_item.line && is_return==true{
			code<<"$keyword ;\n"
			is_return=false
		}
		else if is_return==true && item==next_item{
			code<<"$keyword ;\n"
			is_return=false
		}
		else if is_return==true && item.keyword==r"\n"{
			code<<";\n"
			is_return=false
		}
		else if item.tab!=next_item.tab && is_return==true{
			code<<"$keyword ;\n"
			is_return=false
		}
		else if item.ast_type=="function_define"{
			c_function,free=func_return(item.keyword,ast)
			code<<c_function
			is_func_def=true
		}
		else if (is_function_call==true && (next_item.line!=item.line || next_item==item || next_item.keyword==r"\n") && item.keyword==")")||( is_operator==true && item.keyword==r"\n")||( is_operator==true && item==next_item){
			if keyword==")"{
				code<<");\n"
			}
			else if is_operator==true && item==next_item{
				code<<"$keyword;\n"
			}
			else if previous_code_block.keyword==")" && item.keyword==r"\n"{
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
		else if is_operator==true && item.keyword!=r"\n"{
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
		else if is_return==true && item.keyword!=r"\n"{
			code[code.len-1]+="$keyword "
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
		if (next_item.ast_type=="function_define" || item==next_item) && free.len!=0{
			for free_var in free{
				code<<"\nfree($free_var);\n$free_var=NULL;\n"
			} 
			code<<"\n}\n"
			free=[]string{}
		}
		else if item==next_item || keyword=="pass"{
			code<<"\n}\n"
		}
		else if is_operator==false && is_function_call==false && previous_code_block.line!=item.line && is_func_def==false  && item.keyword!="(" && (item.ast_type in ast_type)==false{
			if previous_code_block.tab>item.tab && keyword!=r"\n" {
				code<<"\n}\n"
			}	
		} 
		
		else if next_item.ast_type=="function_define"{
			code<<"\n}\n"
		}
		else if item==next_item && item!=parser.Body{}{
			code<<"\n}\n"
			}
		if keyword!=r"\n"{
			previous_code_block=ast.body[index]
		}
	} 
	return code
}