module parser
// Original author: Saptak Bhoumik
pub fn function(item string,is_func_def bool,previus_item string, json Ast,argument bool)(Body , string,bool){
	mut is_argument:=argument
	mut previous_item:=previus_item
	mut code_block:=Body{}
	if item!=" " && is_func_def==true && previus_item=="def" && item!="def"{
		code_block=Body{ast_type:"function_define"
						keyword : item
						length :item.len}
						previous_item=item
	}

	else if json.body.len>0{
		if item=="(" && is_func_def==true && json.body[json.body.len-1].left.len==0 && is_argument==false{
			previous_item="("
			is_argument= true
			}
		else if item==")" && is_func_def==true && json.body[json.body.len-1].left.len==0 && is_argument==true{
			previous_item=")"
			is_argument=false

		}
	else if item!=" " && is_argument==true && item!=r"\n" && item!="(" && item!=")" && item!="," && item!=":" && is_func_def==true && json.body[json.body.len-1].left.len==0 && previous_item=="("{
			code_block=Body{ast_type:"required_argument"
							keyword : item
							length :item.len}
		}
	}
	return code_block ,  previous_item ,is_argument
}