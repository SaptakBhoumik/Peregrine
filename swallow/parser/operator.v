module parser
// Original author: Saptak Bhoumik
pub fn parse_operator(operator bool,previus_item string,item string,tab f64,is_constant bool,mut previous_code_block Body) (Body,bool,bool){
	binary_operator:=['+','-','*','/','^','//','%']
	mut replace_previous:=false
	mut is_operator:=operator
	mut code_block:=Body{}
	mut temp_code_block:=Body{}
	mut code:=Body{}
	if item=="=" && is_constant==false{
		code_block=Body{ast_type:"assign"
						keyword : item
						length :item.len
						tab : tab
						left:[Body{ast_type:"variable"
									keyword : previus_item
									length :previus_item.len
									tab : tab}]
						}
	}
	else if item in binary_operator{
		temp_code_block=Body{ast_type:"binary_operator"
						keyword : item
						length :item.len
						tab : tab
						left:[Body{ast_type:know_type(previus_item)
									keyword : previus_item
									length :previus_item.len
									tab : tab}]
						}
		previous_code_block.right<<temp_code_block
		code_block=previous_code_block
		replace_previous=true
	}
	else if previus_item in binary_operator{
		temp_code_block=Body{ast_type:know_type(item)
									keyword : item
									length : item.len
									tab : tab}
		previous_code_block.right.last().right<<temp_code_block
		code_block=previous_code_block
		replace_previous=true
	}
	else if item==r"\n"{
		replace_previous=false
		is_operator=false
	}
	else{
		code=Body{ast_type:know_type(item)
						keyword : item
						length :item.len
						tab : tab}
		previous_code_block.right<<code
		code_block=previous_code_block
		replace_previous=true
	}
	return code_block,is_operator,replace_previous
}