module parser
// Original author: Saptak Bhoumik
fn present_in_type(keyword string) bool{
	mut result:=false
	swallow_type:=["int","bool","str","list","dictionary","float","void"]
	if keyword in swallow_type{
		result=true
	}
	return result
}
pub fn tab_handler(mut json Ast,mut code_block Body,mut previus_code_block Body,right bool) Ast{
	if code_block.keyword!="r" && code_block.keyword!="f" && code_block.keyword!='const'&& present_in_type(code_block.keyword)==false{
		if right==false{
			if code_block.keyword!=r"\n"{
				if code_block.tab==previus_code_block.tab{
					code_block.relative_to=previus_code_block.relative_to
				}
				else if code_block.tab<previus_code_block.tab{
					code_block.relative_to=code_block.id
				}
				else if code_block.tab>previus_code_block.tab{
					if previus_code_block.direction=="right"{
						code_block.relative_to=previus_code_block.relative_to
					}
					else{
						code_block.relative_to=previus_code_block.id
					}
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
	return json
}