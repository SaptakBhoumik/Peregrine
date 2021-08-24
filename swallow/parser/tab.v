module parser
// Original author: Saptak Bhoumik
pub fn tab_handler(mut json Ast,mut code_block Body,mut previus_code_block Body,right bool) Ast{
	if right==false{
		if code_block.keyword!=r"\n"{
			if code_block.tab==previus_code_block.tab{
				code_block.relative_to=previus_code_block.relative_to
			}
			else if code_block.tab<previus_code_block.tab{
				code_block.relative_to=code_block.id
			}
			else if code_block.tab>previus_code_block.tab{
				code_block.relative_to=previus_code_block.id
			}
		}
		json.body<<code_block
	}
	else{
		if code_block.keyword!=r"\n"{
			if previus_code_block.direction=="right"{
				code_block.relative_to=previus_code_block.relative_to
			}
			else if previus_code_block.direction!="right"{
				code_block.relative_to=previus_code_block.id
			}
		}
		json.body<<code_block
	}
	return json
}