module parser
// Original author: Saptak Bhoumik
pub fn parse_operator(operator bool,item string,tab f64,is_constant bool,mut previous_code_block Body) (Body,bool){
	binary_operator:=['+','-','*','/','^','//','%']
	mut is_operator:=operator
	mut code_block:=Body{}
	if item=="=" {
		code_block=Body{ast_type:"assign"
						keyword : item
						length :item.len
						tab : tab
						}
	}
	else if item in binary_operator{
		code_block=Body{ast_type:"binary_operator"
					keyword : item
					length :item.len
					tab : tab
					}


	}
	else if item==r"\n"{
		is_operator=false
	}
	else if item!=")" && item!=":"{
		code_block=Body{ast_type:know_type(item)
					keyword : item
					length :item.len
					tab : tab
					}
	}
	return code_block,is_operator
}