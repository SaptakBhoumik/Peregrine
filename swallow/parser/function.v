module parser
// Original author: Saptak Bhoumik
pub fn function(item string,is_func_def bool,previus_item string, json Ast,right bool,tab_len f64)(Body , string,bool){
	mut is_argument:=right
	mut previous_item:=previus_item
	mut code_block:=Body{}

	if item!=" " && is_func_def==true && previus_item=="def" && item!="def"{
		code_block=Body{ast_type:"function_define"
						keyword : item
						length :item.len}
						previous_item=item
	}

	else if item=="(" && json.body.last().ast_type=="function_define"{
		is_argument= true
		previous_item=item
	} 
	else if item==")"{
		previous_item=item
		is_argument=false
	}

	return code_block ,previous_item ,is_argument
}