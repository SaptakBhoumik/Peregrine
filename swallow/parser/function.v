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

	else if json.body.len>0{
		if item=="(" && is_func_def==true && json.body.last().ast_type=="function_define" && is_argument==false{
			is_argument= true
			previous_item=item
			}
		else if item==")" && is_argument==true && is_func_def==true {
			previous_item=item
			is_argument=false

		}
	else if item!=" " && is_argument==true && item!=r"\n" && item!="(" && item!=")" && item!="," {
		previous_item=item
		code_block=Body{ast_type:"required_argument"
							keyword : item
							length :item.len
							tab : tab_len}
		}
	}
	return code_block ,previous_item ,is_argument
}