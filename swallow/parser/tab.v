module parser
// Original author: Saptak Bhoumik
pub fn tab_handler(mut json Ast,mut code_block Body,mut previus_code_block Body,right bool,last_left_code_block Body) (Ast,string){
	mut error := ""
	if code_block.keyword!="r" && code_block.keyword!="f"{
		if right==false{
			if code_block.keyword!=r"\n"{
				if code_block.tab==previus_code_block.tab{
					if previus_code_block.direction=="right" && last_left_code_block.direction=="left" && last_left_code_block.tab==code_block.tab{
						code_block.relative_to=last_left_code_block.relative_to
					}
					else{
						code_block.relative_to=previus_code_block.relative_to
					}
				}
				else if code_block.tab==previus_code_block.tab-1{
					code_block.relative_to=code_block.id
				}
				else if code_block.tab==previus_code_block.tab+1{
					if previus_code_block.direction=="right"{
						code_block.relative_to=previus_code_block.relative_to
					}
					else{
						code_block.relative_to=previus_code_block.id
					}
				}
				else{
					error="$code_block.keyword\n^ Undefined intent"
				}
			}
			json.body<<code_block
		}
		else{
			if code_block.keyword!=r"\n"{
				if json.body.last().direction=="right"{
					code_block.relative_to=previus_code_block.relative_to
				}
				else if json.body.last().direction!="right"{
					code_block.relative_to=json.body.last().id
				}
			}
			json.body<<code_block
		}
	}
	return json,error
}